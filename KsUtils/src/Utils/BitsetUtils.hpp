#ifndef BITSET_UTILS_H
#define BITSET_UTILS_H

#include <algorithm>
#include <string>
#include <bitset>
#include "Compilator/CompilatorValueTypes.hpp"
#include "Globals.hpp"
#include <vector>
#include <sstream>
#include <array>
#include <cstdint>
#include <cstring>

namespace BitsetUtils
{
    inline std::string ToListOfCppValues
    (
        const std::bitset<Globals::byte_to_bits>& bits,
        const std::vector<std::string>& labels
    )
    {
        std::stringstream ss;

        for (size_t i = 0; i < Globals::byte_to_bits; ++i)
            ss << CompilatorValueTypes::c_1bitmask_type_str << " " << labels[i] << " = " << bits[i] << ";" << std::endl;

        return ss.str();
    }

    static uint32_t ConvertToU32(const std::array< std::bitset<Globals::byte_to_bits>, sizeof(uint32_t)>& arr)
    {
        std::array<uint8_t, sizeof(uint32_t)> u8_array = {0};

        const size_t arr_size = arr.size();
        for(std::size_t i = 0 ; i < arr_size ; ++i)
        {
            u8_array[i] = static_cast<uint8_t>( arr[i].to_ullong() );
        }

        uint32_t result = 0;
        std::memcpy(&result, u8_array.data(), sizeof(result));
        return result;
    }

    inline std::string DecByteToReversedBin(uint8_t number)
    {
        std::string s = std::bitset<Globals::byte_to_bits>(number).to_string();
        std::reverse(s.begin(), s.end());
        return s;
    }
};

#endif // !BITSET_UTILS_H
