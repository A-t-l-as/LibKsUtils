// Test program for ZlibCodec.
//
// Usage:  ZlibTest <file>
//
// 1. Reads the file: zlib/gzip files are decompressed, other files are read as is
// 2. Saves the (decompressed) data as <file>.raw
// 3. Writes the data back in the same format as <file>.rewritten
// 4. Checks whether the rewritten file is identical to the original

#include "ZlibCodec.hpp"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Reads a whole file into memory (empty vector if it cannot be opened).
static std::vector<uint8_t> ReadAll(const fs::path& path)
{
    std::ifstream input_file(path, std::ios::binary);

    return std::vector<uint8_t>(std::istreambuf_iterator<char>(input_file),
                                std::istreambuf_iterator<char>());
}

// Path as UTF-8 text for printing (works in both C++17 and C++20).
static std::string PathText(const fs::path& path)
{
    const auto utf8 = path.u8string();
    return std::string(utf8.begin(), utf8.end());
}

static const char* ContentName(ZlibCodec::Content content)
{
    switch (content)
    {
        case ZlibCodec::Content::Zlib:
            return "zlib (decompressed)";

        case ZlibCodec::Content::Gzip:
            return "gzip (decompressed)";

        case ZlibCodec::Content::Plain:
            return "not compressed (read as is)";
    }

    return "?";
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ZlibTest <file>\n";
        return 1;
    }

    const fs::path input_path = argv[1];

    if (!fs::is_regular_file(input_path))
    {
        std::cout << "Error: file not found: " << PathText(input_path) << "\n";
        return 1;
    }

    fs::path raw_path = input_path;
    raw_path += ".raw";

    fs::path rewritten_path = input_path;
    rewritten_path += ".rewritten";

    std::string error;

    // 1. Read: decompress if it is zlib/gzip, otherwise just read
    std::vector<uint8_t> data;
    ZlibCodec::Content content = ZlibCodec::Content::Plain;

    if (!ZlibCodec::ReadFile(input_path, data, &content, &error))
    {
        std::cout << "Error: " << error << "\n";
        return 2;
    }

    std::cout << "File type:       " << ContentName(content) << "\n";
    std::cout << "Data:            " << data.size() << " bytes\n";

    // Preview of the first 32 bytes
    std::cout << "First bytes:     ";

    for (std::size_t i = 0; i < data.size() && i < 32; ++i)
    {
        char hex[4];
        std::snprintf(hex, sizeof hex, "%02X ", data[i]);
        std::cout << hex;
    }

    std::cout << "\n";

    // 2. Save the raw data (to inspect in a hex editor)
    if (ZlibCodec::WriteFile(raw_path, data, ZlibCodec::Content::Plain, -1, &error))
    {
        std::cout << "Saved:           " << PathText(raw_path) << "\n";
    }
    else
    {
        std::cout << "Error: " << error << "\n";
    }

    // 3. Write back in the same format as the original
    if (!ZlibCodec::WriteFile(rewritten_path, data, content, -1, &error))
    {
        std::cout << "Error: " << error << "\n";
        return 3;
    }

    std::cout << "Saved:           " << PathText(rewritten_path) << "\n";

    // 4. Compare with the original
    const bool identical = ReadAll(input_path) == ReadAll(rewritten_path);

    std::cout << "Size:            original " << fs::file_size(input_path)
              << " B, rewritten " << fs::file_size(rewritten_path) << " B\n";

    std::cout << (identical
                      ? "RESULT: files are byte-for-byte identical\n"
                      : "RESULT: files DIFFER (same data, different compression settings)\n");

    return identical ? 0 : 4;
}
