#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstddef>
#include <cstdint>
namespace Globals
{
    constexpr char indent_8_str[] = "        ";
    constexpr std::size_t indent_8_str_len = sizeof(Globals::indent_8_str) - 1;

    constexpr int one_quad = 1;
    constexpr int two_quads = 2;
    constexpr int three_quads = 3;

    constexpr int one_bit = 1;
	constexpr int byte_to_bits = 8;
    constexpr int tetrad_to_bits = 4;

    constexpr uint8_t left_tetrad  = 0xF0;
    constexpr uint8_t right_tetrad = 0x0F;

    constexpr std::size_t int_to_bits = 32;

	constexpr int bin_base = 2;
	constexpr int hex_base = 16;

	constexpr bool success_code = false;
	constexpr bool error_code = true;


    constexpr char horizontal_line[] = "-----------------------------------------------------------";

};

#endif // !GLOBALS_H
