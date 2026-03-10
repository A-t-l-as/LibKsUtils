#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstddef>
namespace Globals
{
    constexpr int one_quad = 1;
    constexpr int two_quads = 2;
    constexpr int three_quads = 3;


	constexpr int byte_to_bits = 8;
    constexpr int tetrad_to_bits = 4;
    constexpr std::size_t int_to_bits = 32;

	constexpr int bin_base = 2;
	constexpr int hex_base = 16;

	constexpr bool success_code = false;
	constexpr bool error_code = true;
};

#endif // !GLOBALS_H
