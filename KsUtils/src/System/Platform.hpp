#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#ifdef _WIN32
    #include <stdlib.h>
    #define bswap16(x) _byteswap_ushort(x)
    #define bswap32(x) _byteswap_ulong(x)
#else
    #define bswap16(x) __builtin_bswap16(x)
    #define bswap32(x) __builtin_bswap32(x)
#endif

#endif // !PLATFORM_HPP
