#ifndef ZLIB_CODEC_HPP
#define ZLIB_CODEC_HPP

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

// zlib (and gzip / raw deflate) compression and decompression.
// Pure C++17 + zlib, no other dependencies.
//
// All functions return true on success. On failure they return false and,
// if an error pointer is given, store a human-readable message in it.
//
// Note on byte-exact round-trips: the compressed output is identical to the
// original if the original was produced by zlib with the same parameters
// (level, windowBits, memLevel = 8, default strategy). A 78 9C header means
// the default level (6), i.e. level = -1 or 6 in this class.
class ZlibCodec
{
public:
    using Bytes = std::vector<uint8_t>;
    using Path = std::filesystem::path;

    // Stream format used for compression / decompression.
    enum class Format
    {
        Zlib,   // zlib header (RFC 1950) - default
        Gzip,   // gzip header (RFC 1952), e.g. .gz files
        Raw,    // raw deflate stream without a header (RFC 1951)
        Auto    // decompression only: detects zlib or gzip automatically
    };

    // What a file / buffer contains, detected from its header.
    enum class Content
    {
        Plain,  // not compressed (no zlib or gzip header)
        Zlib,   // zlib stream (e.g. 78 9C ...)
        Gzip    // gzip stream (1F 8B 08 ...)
    };

    // ---------- format detection ----------

    // Checks only the header bytes (fast, does not decompress anything).
    static Content Detect(const uint8_t* data, std::size_t size);
    static Content Detect(const Bytes& data);

    // ---------- "read anything" / "write back the same way" ----------

    // Reads a file: if it is zlib/gzip it gets decompressed, otherwise the
    // bytes are returned as they are. *content (optional) says which one it was.
    // Fails if the file cannot be read, or if it has a valid zlib/gzip header
    // but the compressed data is damaged.
    static bool ReadFile
    (
        const Path& path,
        Bytes& output_data,
        Content* content = nullptr,
        std::string* error = nullptr
    );

    // Writes data in the given form: Plain = as is, Zlib/Gzip = compressed.
    // Pass the Content you got from ReadFile() to keep the original format.
    static bool WriteFile
    (
        const Path& path,
        const Bytes& data,
        Content content,
        int level = -1,
        std::string* error = nullptr
    );

    // ---------- in memory ----------

    // level: 0 (none) .. 9 (best), -1 = default (6)
    static bool Compress
    (
        const uint8_t* data,
        std::size_t size,
        Bytes& output_data,
        int level = -1,
        Format format = Format::Zlib,
        std::string* error = nullptr
    );

    static bool Compress
    (
        const Bytes& data,
        Bytes& output_data,
        int level = -1,
        Format format = Format::Zlib,
        std::string* error = nullptr
    );

    static bool Decompress
    (
        const uint8_t* data,
        std::size_t size,
        Bytes& output_data,
        Format format = Format::Auto,
        std::string* error = nullptr
    );

    static bool Decompress
    (
        const Bytes& data,
        Bytes& output_data,
        Format format = Format::Auto,
        std::string* error = nullptr
    );

    // ---------- file <-> memory (always compressed) ----------

    static bool ReadCompressedFile
    (
        const Path& path,
        Bytes& output_data,
        Format format = Format::Auto,
        std::string* error = nullptr
    );

    static bool WriteCompressedFile
    (
        const Path& path,
        const Bytes& data,
        int level = -1,
        Format format = Format::Zlib,
        std::string* error = nullptr
    );

    // ---------- file <-> file (streaming, never loads the whole file) ----------

    static bool CompressFile
    (
        const Path& input_path,
        const Path& output_path,
        int level = -1,
        Format format = Format::Zlib,
        std::string* error = nullptr
    );

    static bool DecompressFile
    (
        const Path& input_path,
        const Path& output_path,
        Format format = Format::Auto,
        std::string* error = nullptr
    );
};

#endif // ZLIB_CODEC_HPP
