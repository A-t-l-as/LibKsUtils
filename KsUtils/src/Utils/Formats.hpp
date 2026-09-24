#ifndef FORMATS_HPP
#define FORMATS_HPP

#include <array>
#include <cstddef>
#include <cstdint>

namespace Formats
{

    enum class ENFormat
    {
        invalid,
        cpp,
        png,
        hpp,
        json,
        txt
    };

    constexpr char tex_format_str[] = "tex";
    constexpr char cpp_format_str[] = "cpp";
    constexpr char hpp_format_str[] = "hpp";
    constexpr char txt_format_str[] = "txt";
    constexpr char png_format_str[] = "png";
    constexpr char par_format_str[] = "par";
    constexpr char wpk_format_str[] = "wpk";
    constexpr char json_format_str[] = "json";

    constexpr char tex_format_str_upper[] = "TEX";
    constexpr char cpp_format_str_upper[] = "CPP";
    constexpr char hpp_format_str_upper[] = "HPP";
    constexpr char txt_format_str_upper[] = "TXT";
    constexpr char png_format_str_upper[] = "PNG";
    constexpr char par_format_str_upper[] = "PAR";
    constexpr char wpk_format_str_upper[] = "WPK";
    constexpr char json_format_str_upper[] = "JSON";

    constexpr char tex_format_str_with_dot[] = ".tex";
    constexpr char cpp_format_str_with_dot[] = ".cpp";
    constexpr char hpp_format_str_with_dot[] = ".hpp";
    constexpr char txt_format_str_with_dot[] = ".txt";

    constexpr char png_format_str_with_dot[] = ".png";
    constexpr std::size_t png_format_str_with_dot_len = sizeof(Formats::png_format_str_with_dot) - 1;

    constexpr char par_format_str_with_dot[] = ".par";
    constexpr char wpk_format_str_with_dot[] = ".wpk";
    constexpr char json_format_str_with_dot[] = ".json";


    constexpr char tex_format_str_upper_with_dot[] = ".TEX";
    constexpr char cpp_format_str_upper_with_dot[] = ".CPP";
    constexpr char hpp_format_str_upper_with_dot[] = ".HPP";
    constexpr char txt_format_str_upper_with_dot[] = ".TXT";
    constexpr char png_format_str_upper_with_dot[] = ".PNG";
    constexpr char par_format_str_upper_with_dot[] = ".PAR";
    constexpr char wpk_format_str_upper_with_dot[] = ".WPK";
    constexpr char json_format_str_upper_with_dot[] = ".JSON";

    constexpr uint8_t first_byte_of_zlib_signature = 0x78;

    constexpr std::array<uint8_t, sizeof(uint32_t)> second_bytes_of_zlib_signature =
    {
        0x01,
        0x5E,
        0x9C,
        0xDA
    };

    template <typename T, std::size_t N>
    bool IsZLibSignature(const std::array<T, N>& signature)
    {
        if(signature[0] != first_byte_of_zlib_signature)
            return false;

        for(uint8_t byte : second_bytes_of_zlib_signature)
        {
            if(signature[1] == byte)
                return true;
        }

        return false;
    }

};

#endif // !FORMATS_HPP
