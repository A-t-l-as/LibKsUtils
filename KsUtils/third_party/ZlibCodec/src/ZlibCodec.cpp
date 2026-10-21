#include "ZlibCodec.hpp"
#include "SafeFileWriter.hpp"

#include <zlib.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <utility>

namespace
{

namespace fs = std::filesystem;

constexpr std::size_t chunk_size = 64 * 1024;

// Returns the number of bytes read, 0 = end of data, -1 = error.
using Reader = std::function<std::ptrdiff_t(uint8_t*, std::size_t)>;
using Writer = std::function<bool(const uint8_t*, std::size_t)>;

void SetError(std::string* error, std::string message)
{
    if (error)
    {
        *error = std::move(message);
    }
}

// Path as UTF-8 text for error messages (works in both C++17 and C++20).
std::string PathText(const fs::path& path)
{
    const auto utf8 = path.u8string();
    return std::string(utf8.begin(), utf8.end());
}

int WindowBitsFor(ZlibCodec::Format format, bool for_decompression)
{
    switch (format)
    {
        case ZlibCodec::Format::Zlib:
            return MAX_WBITS;

        case ZlibCodec::Format::Gzip:
            return MAX_WBITS + 16;

        case ZlibCodec::Format::Raw:
            return -MAX_WBITS;

        case ZlibCodec::Format::Auto:
            return for_decompression ? MAX_WBITS + 32 : MAX_WBITS;
    }

    return MAX_WBITS;
}

ZlibCodec::Format FormatFor(ZlibCodec::Content content)
{
    return content == ZlibCodec::Content::Gzip ? ZlibCodec::Format::Gzip
                                               : ZlibCodec::Format::Zlib;
}

// ---- zlib streaming core ----

bool DeflateStream
(
    const Reader& read,
    const Writer& write,
    int level,
    int window_bits,
    std::string* error
)
{
    z_stream stream{};
    int result = deflateInit2(&stream, level, Z_DEFLATED, window_bits, 8, Z_DEFAULT_STRATEGY);

    if (result != Z_OK)
    {
        SetError(error, "deflateInit2 failed (code " + std::to_string(result) + ")");
        return false;
    }

    std::vector<uint8_t> input_buffer(chunk_size);
    std::vector<uint8_t> output_buffer(chunk_size);
    int flush = Z_NO_FLUSH;

    do
    {
        const std::ptrdiff_t bytes_read = read(input_buffer.data(), chunk_size);

        if (bytes_read < 0)
        {
            deflateEnd(&stream);
            SetError(error, "Failed to read input data");
            return false;
        }

        flush = (bytes_read == 0) ? Z_FINISH : Z_NO_FLUSH;
        stream.next_in = input_buffer.data();
        stream.avail_in = static_cast<uInt>(bytes_read);

        do
        {
            stream.next_out = output_buffer.data();
            stream.avail_out = static_cast<uInt>(chunk_size);
            result = deflate(&stream, flush);

            if (result == Z_STREAM_ERROR)
            {
                deflateEnd(&stream);
                SetError(error, "Deflate stream error");
                return false;
            }

            const std::size_t bytes_ready = chunk_size - stream.avail_out;

            if (bytes_ready > 0 && !write(output_buffer.data(), bytes_ready))
            {
                deflateEnd(&stream);
                SetError(error, "Failed to write output data");
                return false;
            }
        }
        while (stream.avail_out == 0);
    }
    while (flush != Z_FINISH);

    deflateEnd(&stream);
    return true;
}

bool InflateStream
(
    const Reader& read,
    const Writer& write,
    int window_bits,
    std::string* error
)
{
    z_stream stream{};
    int result = inflateInit2(&stream, window_bits);

    if (result != Z_OK)
    {
        SetError(error, "inflateInit2 failed (code " + std::to_string(result) + ")");
        return false;
    }

    std::vector<uint8_t> input_buffer(chunk_size);
    std::vector<uint8_t> output_buffer(chunk_size);

    do
    {
        const std::ptrdiff_t bytes_read = read(input_buffer.data(), chunk_size);

        if (bytes_read < 0)
        {
            inflateEnd(&stream);
            SetError(error, "Failed to read input data");
            return false;
        }

        if (bytes_read == 0)
        {
            break; // input ended before the end of the stream
        }

        stream.next_in = input_buffer.data();
        stream.avail_in = static_cast<uInt>(bytes_read);

        do
        {
            stream.next_out = output_buffer.data();
            stream.avail_out = static_cast<uInt>(chunk_size);
            result = inflate(&stream, Z_NO_FLUSH);

            switch (result)
            {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                case Z_STREAM_ERROR:
                {
                    const std::string message = stream.msg ? stream.msg
                                                           : "code " + std::to_string(result);
                    inflateEnd(&stream);
                    SetError(error, "Decompression error: " + message);
                    return false;
                }

                default:
                break;
            }

            const std::size_t bytes_ready = chunk_size - stream.avail_out;

            if (bytes_ready > 0 && !write(output_buffer.data(), bytes_ready))
            {
                inflateEnd(&stream);
                SetError(error, "Failed to write output data");
                return false;
            }
        }
        while (stream.avail_out == 0 && result != Z_STREAM_END);
    }
    while (result != Z_STREAM_END);

    inflateEnd(&stream);

    if (result != Z_STREAM_END)
    {
        SetError(error, "Compressed data is incomplete (truncated)");
        return false;
    }

    return true;
}

// ---- data sources and sinks ----

Reader MemoryReader(const uint8_t* data, std::size_t size, std::size_t& position)
{
    return [data, size, &position](uint8_t* buffer, std::size_t max_size) -> std::ptrdiff_t
    {
        const std::size_t count = std::min(max_size, size - position);

        if (count > 0)
        {
            std::copy(data + position, data + position + count, buffer);
            position += count;
        }

        return static_cast<std::ptrdiff_t>(count);
    };
}

Reader StreamReader(std::istream& input_stream)
{
    return [&input_stream](uint8_t* buffer, std::size_t max_size) -> std::ptrdiff_t
    {
        input_stream.read(reinterpret_cast<char*>(buffer), static_cast<std::streamsize>(max_size));

        if (input_stream.bad())
        {
            return -1;
        }

        return static_cast<std::ptrdiff_t>(input_stream.gcount());
    };
}

Writer VectorWriter(ZlibCodec::Bytes& output_data)
{
    return [&output_data](const uint8_t* buffer, std::size_t length)
    {
        output_data.insert(output_data.end(), buffer, buffer + length);
        return true;
    };
}

Writer StreamWriter(std::ostream& output_stream)
{
    return [&output_stream](const uint8_t* buffer, std::size_t length)
    {
        output_stream.write(reinterpret_cast<const char*>(buffer), static_cast<std::streamsize>(length));
        return static_cast<bool>(output_stream);
    };
}

// ---- files ----

bool OpenInput(std::ifstream& input_file, const fs::path& path, std::string* error)
{
    input_file.open(path, std::ios::binary);

    if (!input_file)
    {
        SetError(error, "Cannot open " + PathText(path));
        return false;
    }

    return true;
}

} // namespace

// ---------- format detection ----------

ZlibCodec::Content ZlibCodec::Detect(const uint8_t* data, std::size_t size)
{
    if (size >= 3 && data[0] == 0x1F && data[1] == 0x8B && data[2] == 0x08)
    {
        return Content::Gzip;
    }

    if (size >= 2)
    {
        const unsigned cmf = data[0];
        const unsigned flg = data[1];

        const bool deflate_method = (cmf & 0x0F) == 8;           // CM = 8 (deflate)
        const bool valid_window   = (cmf >> 4) <= 7;             // CINFO <= 7 (max 32 KB)
        const bool checksum_ok    = ((cmf << 8) | flg) % 31 == 0;
        const bool no_dictionary  = (flg & 0x20) == 0;           // FDICT not set

        if (deflate_method && valid_window && checksum_ok && no_dictionary)
        {
            return Content::Zlib;
        }
    }

    return Content::Plain;
}

ZlibCodec::Content ZlibCodec::Detect(const Bytes& data)
{
    return Detect(data.data(), data.size());
}

// ---------- "read anything" / "write back the same way" ----------

bool ZlibCodec::ReadFile
(
    const Path& path,
    Bytes& output_data,
    Content* content,
    std::string* error
)
{
    output_data.clear();

    std::ifstream input_file;

    if (!OpenInput(input_file, path, error))
    {
        return false;
    }

    input_file.seekg(0, std::ios::end);
    const std::streamoff file_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    if (file_size < 0)
    {
        SetError(error, "Cannot read " + PathText(path));
        return false;
    }

    Bytes file_data(static_cast<std::size_t>(file_size));

    if (file_size > 0 && !input_file.read(reinterpret_cast<char*>(file_data.data()), file_size))
    {
        SetError(error, "Cannot read " + PathText(path));
        return false;
    }

    const Content detected_content = Detect(file_data);

    if (content)
    {
        *content = detected_content;
    }

    if (detected_content == Content::Plain)
    {
        output_data = std::move(file_data);
        return true;
    }

    std::string decompress_error;

    if (!Decompress(file_data, output_data, FormatFor(detected_content), &decompress_error))
    {
        SetError(error, PathText(path) + " has a "
                        + (detected_content == Content::Gzip ? "gzip" : "zlib")
                        + " header but cannot be decompressed (" + decompress_error + ")");
        return false;
    }

    return true;
}

bool ZlibCodec::WriteFile
(
    const Path& path,
    const Bytes& data,
    Content content,
    int level,
    std::string* error
)
{
    if (content != Content::Plain)
    {
        return WriteCompressedFile(path, data, level, FormatFor(content), error);
    }

    SafeFileWriter output_file(path);

    if (!output_file.Open(error))
    {
        return false;
    }

    output_file.GetStream().write(reinterpret_cast<const char*>(data.data()),
                                  static_cast<std::streamsize>(data.size()));

    return output_file.Commit(error);
}

// ---------- in memory ----------

bool ZlibCodec::Compress
(
    const uint8_t* data,
    std::size_t size,
    Bytes& output_data,
    int level,
    Format format,
    std::string* error
)
{
    output_data.clear();
    output_data.reserve(compressBound(static_cast<uLong>(size)) + 32);

    std::size_t position = 0;

    const bool success = DeflateStream(MemoryReader(data, size, position),
                                       VectorWriter(output_data),
                                       level, WindowBitsFor(format, false), error);

    if (!success)
    {
        output_data.clear();
    }

    return success;
}

bool ZlibCodec::Compress
(
    const Bytes& data,
    Bytes& output_data,
    int level,
    Format format,
    std::string* error
)
{
    return Compress(data.data(), data.size(), output_data, level, format, error);
}

bool ZlibCodec::Decompress
(
    const uint8_t* data,
    std::size_t size,
    Bytes& output_data,
    Format format,
    std::string* error
)
{
    output_data.clear();
    output_data.reserve(size * 4);

    std::size_t position = 0;

    const bool success = InflateStream(MemoryReader(data, size, position),
                                       VectorWriter(output_data),
                                       WindowBitsFor(format, true), error);

    if (!success)
    {
        output_data.clear();
    }

    return success;
}

bool ZlibCodec::Decompress
(
    const Bytes& data,
    Bytes& output_data,
    Format format,
    std::string* error
)
{
    return Decompress(data.data(), data.size(), output_data, format, error);
}

// ---------- file <-> memory ----------

bool ZlibCodec::ReadCompressedFile
(
    const Path& path,
    Bytes& output_data,
    Format format,
    std::string* error
)
{
    output_data.clear();

    std::ifstream input_file;

    if (!OpenInput(input_file, path, error))
    {
        return false;
    }

    const bool success = InflateStream(StreamReader(input_file),
                                       VectorWriter(output_data),
                                       WindowBitsFor(format, true), error);

    if (!success)
    {
        output_data.clear();
    }

    return success;
}

bool ZlibCodec::WriteCompressedFile
(
    const Path& path,
    const Bytes& data,
    int level,
    Format format,
    std::string* error
)
{
    SafeFileWriter output_file(path);

    if (!output_file.Open(error))
    {
        return false;
    }

    std::size_t position = 0;

    if (!DeflateStream(MemoryReader(data.data(), data.size(), position),
                       StreamWriter(output_file.GetStream()),
                       level, WindowBitsFor(format, false), error))
    {
        return false;
    }

    return output_file.Commit(error);
}

// ---------- file <-> file ----------

bool ZlibCodec::CompressFile
(
    const Path& input_path,
    const Path& output_path,
    int level,
    Format format,
    std::string* error
)
{
    std::ifstream input_file;

    if (!OpenInput(input_file, input_path, error))
    {
        return false;
    }

    SafeFileWriter output_file(output_path);

    if (!output_file.Open(error))
    {
        return false;
    }

    if (!DeflateStream(StreamReader(input_file),
                       StreamWriter(output_file.GetStream()),
                       level, WindowBitsFor(format, false), error))
    {
        return false;
    }

    return output_file.Commit(error);
}

bool ZlibCodec::DecompressFile
(
    const Path& input_path,
    const Path& output_path,
    Format format,
    std::string* error
)
{
    std::ifstream input_file;

    if (!OpenInput(input_file, input_path, error))
    {
        return false;
    }

    SafeFileWriter output_file(output_path);

    if (!output_file.Open(error))
    {
        return false;
    }

    if (!InflateStream(StreamReader(input_file),
                       StreamWriter(output_file.GetStream()),
                       WindowBitsFor(format, true), error))
    {
        return false;
    }

    return output_file.Commit(error);
}
