#ifndef WIN_32_GUID_STRUCT_HPP
#define WIN_32_GUID_STRUCT_HPP

#ifdef _WIN32
#include <cstdint>
#include <Windows.h>
#include "Console/Console.hpp"

struct Win32GuidStruct
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;

    constexpr static const int SizeOfData4 = 8;
    uint8_t  Data4[SizeOfData4];

    static void Generate(Win32GuidStruct& result)
    {
        GUID temp_guid;

        if (CoCreateGuid(&temp_guid) != S_OK)
        {
            CONSOLE_ERR.PrintLn("_WIN32: Failed to generate GUID!");
            throw;
        }

        result.Data1 = temp_guid.Data1;
        result.Data2 = temp_guid.Data2;
        result.Data3 = temp_guid.Data3;

        for (int i = 0; i < SizeOfData4; ++i)
            result.Data4[i] = temp_guid.Data4[i];
    }
};

#endif

#endif // !WIN_32_GUID_STRUCT_HPP
