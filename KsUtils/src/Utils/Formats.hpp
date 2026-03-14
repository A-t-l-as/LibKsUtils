#ifndef FORMATS_HPP
#define FORMATS_HPP

#include <cstddef>

namespace Formats
{

    enum format
    {
        f_invalid,
        f_cpp,
        f_png,
        f_hpp
    };

    constexpr char tex_format_str[] = "tex";
    constexpr char cpp_format_str[] = "cpp";
    constexpr char hpp_format_str[] = "hpp";
    constexpr char txt_format_str[] = "txt";
    constexpr char png_format_str[] = "png";
    constexpr char par_format_str[] = "par";
    constexpr char wpk_format_str[] = "wpk";

    constexpr char tex_format_str_upper[] = "TEX";
    constexpr char cpp_format_str_upper[] = "CPP";
    constexpr char hpp_format_str_upper[] = "HPP";
    constexpr char txt_format_str_upper[] = "TXT";
    constexpr char png_format_str_upper[] = "PNG";
    constexpr char par_format_str_upper[] = "PAR";
    constexpr char wpk_format_str_upper[] = "WPK";

    constexpr char tex_format_str_with_dot[] = ".tex";
    constexpr char cpp_format_str_with_dot[] = ".cpp";
    constexpr char hpp_format_str_with_dot[] = ".hpp";
    constexpr char txt_format_str_with_dot[] = ".txt";

    constexpr char png_format_str_with_dot[] = ".png";
    constexpr std::size_t png_format_str_with_dot_len = sizeof(Formats::png_format_str_with_dot) - 1;

    constexpr char par_format_str_with_dot[] = ".par";
    constexpr char wpk_format_str_with_dot[] = ".wpk";

    constexpr char tex_format_str_upper_with_dot[] = ".TEX";
    constexpr char cpp_format_str_upper_with_dot[] = ".CPP";
    constexpr char hpp_format_str_upper_with_dot[] = ".HPP";
    constexpr char txt_format_str_upper_with_dot[] = ".TXT";
    constexpr char png_format_str_upper_with_dot[] = ".PNG";
    constexpr char par_format_str_upper_with_dot[] = ".PAR";
    constexpr char wpk_format_str_upper_with_dot[] = ".WPK";

};

#endif // !FORMATS_HPP
