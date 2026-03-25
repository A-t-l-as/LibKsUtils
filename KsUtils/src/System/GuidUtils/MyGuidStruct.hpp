#ifndef MY_GUID_STRUCT_HPP
#define MY_GUID_STRUCT_HPP

#include <cstdint>
#include <random>

struct MyGuidStruct
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;

    constexpr static const int SizeOfData4 = 8;
    uint8_t  Data4[SizeOfData4];

    static void Generate(MyGuidStruct& guid)
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dist64;
        std::uniform_int_distribution<uint32_t> dist32;
        std::uniform_int_distribution<uint16_t> dist16;

        guid.Data1 = dist32(gen);
        guid.Data2 = dist16(gen);
        // Wersja 4 UUID: najwyzsze 4 bity Data3 = 0100
        guid.Data3 = (dist16(gen) & 0x0FFF) | 0x4000;

        uint64_t data4 = dist64(gen);
        // Variant bits: najwyzsze 2 bity = 10
        data4 = (data4 & 0x3FFFFFFFFFFFFFFF) | 0x8000000000000000;

        for (int i = 0; i < SizeOfData4; ++i)
            guid.Data4[i] = static_cast<uint8_t>((data4 >> (56 - i * 8)) & 0xFF);
    }
};

#endif // !MY_GUID_STRUCT_HPP
