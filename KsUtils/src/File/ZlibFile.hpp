#ifndef ZLIB_FILE_HPP
#define ZLIB_FILE_HPP

#include "File/BinFile.hpp"
#include "System/MySystem.hpp"
#include "ZlibCodec.hpp"

class ZlibFile
{
public:
    // Reads a file; zlib/gzip is decompressed, anything else is read as is.
    // *content (optional) tells whether the file was compressed, so it can be
    // saved back in the same format with ZlibCodec::WriteFile().
    static BinFile Load
    (
        const std::filesystem::path& arg_input_file_path,
        ZlibCodec::Content* content = nullptr
    )
    {
        MySystem::ThrowExceptionWhenFileNotExistOrIsNotRegularFile(arg_input_file_path);

        std::string error;
        std::vector<uint8_t> data;

        if (!ZlibCodec::ReadFile(arg_input_file_path, data, content, &error))
        {
            throw std::runtime_error(error);
        }

        BinFile bin_file;
        bin_file.WriteBuffer(data);
        return bin_file; // returned without a copy (NRVO / move)
    }
};


#endif // ZLIB_FILE_HPP end
