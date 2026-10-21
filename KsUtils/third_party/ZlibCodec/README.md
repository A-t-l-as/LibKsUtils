# ZlibCodec

zlib / gzip compression and decompression for C++17. No dependencies other
than zlib, which is downloaded and compiled into your target automatically.

## Layout

```
ZlibCodec/
  include/ZlibCodec.hpp   public header
  src/ZlibCodec.cpp       implementation
  src/SafeFileWriter.hpp  internal helper: safe (atomic) file writing
  cmake/ZlibCodec.cmake   CMake helper: zlibcodec_add_to_target()
  example/main.cpp        test program
  CMakeLists.txt          builds the test program only
```

## Adding it to your library

Copy the `ZlibCodec` folder into your project, then:

```cmake
include(path/to/ZlibCodec/cmake/ZlibCodec.cmake)

add_library(MyLib STATIC ...)
zlibcodec_add_to_target(MyLib)
```

ZlibCodec.cpp and zlib are compiled as part of `MyLib`, so the resulting
`.lib` / `.a` is self-contained: users of your library need no zlib1.dll,
no zlib headers and no extra libraries to link. zlib is built with
`Z_PREFIX` (symbols are `z_inflate`, `z_deflate`, ...), so it never clashes
with another zlib in the same program.

Requirements: CMake 3.18+, a C++17 compiler, internet access on the first
configure (to download zlib 1.3.1).

## Usage

```cpp
#include "ZlibCodec.hpp"

std::vector<uint8_t> data;
ZlibCodec::Content content;   // Plain, Zlib or Gzip
std::string error;

// zlib/gzip -> decompressed, anything else -> read as is
if (!ZlibCodec::ReadFile("Interface2_32_3.BIN", data, &content, &error))
    std::cerr << error << '\n';

// ... modify data ...

// write back in the original format
ZlibCodec::WriteFile("Interface2_32_3.BIN", data, content);
```

Other functions: `Detect`, `Compress` / `Decompress` (in memory),
`ReadCompressedFile` / `WriteCompressedFile`, `CompressFile` / `DecompressFile`.

## Test program

```
cmake -S . -B build
cmake --build build
build/ZlibTest <file>
```
