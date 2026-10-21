# ZlibCodec.cmake
#
# Adds ZlibCodec together with zlib straight into one of your targets
# (a static library, a shared library or an executable).
#
# Usage in your CMakeLists.txt:
#
#     include(path/to/ZlibCodec/cmake/ZlibCodec.cmake)
#
#     add_library(MyLib STATIC ...)
#     zlibcodec_add_to_target(MyLib)
#
# What it does:
#   * downloads the zlib 1.3.1 sources once (checked with SHA256),
#   * compiles ZlibCodec.cpp and the needed zlib .c files AS PART OF your target,
#     with your target's compiler flags (runtime library, optimisation, ...),
#   * builds zlib with Z_PREFIX, so its symbols are named z_inflate, z_deflate...
#     They can never clash with another zlib linked into the same program
#     (system libz, Qt's zlib, a zlib from another library, ...).
#
# Result: everything ends up inside your .lib / .a / .exe. Nobody who uses
# your library needs zlib1.dll, zlib headers or to link zlib separately.

include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.18)   # TARGET_DIRECTORY in set_source_files_properties

enable_language(C)                      # zlib is written in C

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)       # use extraction time for file timestamps
endif()

include(FetchContent)

# SOURCE_SUBDIR points to a folder without a CMakeLists.txt, so only the
# sources are downloaded - zlib's own CMake project is NOT added to the build.
FetchContent_Declare(
    zlibcodec_zlib
    URL           https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
    URL_HASH      SHA256=9a93b2b7dfdac77ceba5a558a580e74667dd6fede4585b91eefb60f03b72df23
    SOURCE_SUBDIR do-not-add-zlib-cmake-project
)
FetchContent_MakeAvailable(zlibcodec_zlib)

set(ZLIBCODEC_ROOT     "${CMAKE_CURRENT_LIST_DIR}/.."            CACHE INTERNAL "")
set(ZLIBCODEC_ZLIB_DIR "${zlibcodec_zlib_SOURCE_DIR}"            CACHE INTERNAL "")

function(zlibcodec_add_to_target target)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "zlibcodec_add_to_target: '${target}' is not a target")
    endif()

    # Only the parts of zlib needed for in-memory deflate/inflate
    # (the gz* file API is not used).
    set(zlib_sources
        adler32.c compress.c crc32.c deflate.c infback.c inffast.c
        inflate.c inftrees.c trees.c zutil.c)
    list(TRANSFORM zlib_sources PREPEND "${ZLIBCODEC_ZLIB_DIR}/")

    set(codec_source "${ZLIBCODEC_ROOT}/src/ZlibCodec.cpp")

    # SafeFileWriter.hpp is internal (included only by ZlibCodec.cpp); listed
    # here so that IDEs such as Qt Creator show it in the project tree.
    target_sources(${target} PRIVATE
        ${codec_source}
        "${ZLIBCODEC_ROOT}/src/SafeFileWriter.hpp"
        "${ZLIBCODEC_ROOT}/include/ZlibCodec.hpp"
        ${zlib_sources})

    # ZlibCodec.hpp is the only public header.
    target_include_directories(${target} PUBLIC "${ZLIBCODEC_ROOT}/include")
    target_compile_features(${target} PUBLIC cxx_std_17)

    # zlib headers and Z_PREFIX only for the files that need them, so they
    # don't leak into the rest of your code.
    set_property(SOURCE ${codec_source} ${zlib_sources}
        TARGET_DIRECTORY ${target}
        APPEND PROPERTY INCLUDE_DIRECTORIES "${ZLIBCODEC_ZLIB_DIR}")
    set_property(SOURCE ${codec_source} ${zlib_sources}
        TARGET_DIRECTORY ${target}
        APPEND PROPERTY COMPILE_DEFINITIONS Z_PREFIX)

    if(MSVC)
        set_property(SOURCE ${zlib_sources}
            TARGET_DIRECTORY ${target}
            APPEND PROPERTY COMPILE_DEFINITIONS
                _CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_DEPRECATE)
    else()
        # zlib is third-party code - don't let it trip your warning flags.
        set_property(SOURCE ${zlib_sources}
            TARGET_DIRECTORY ${target}
            APPEND PROPERTY COMPILE_OPTIONS -w)
    endif()

    # GCC 8 (e.g. MinGW 8.1) needs a separate library for std::filesystem.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(${target} PUBLIC stdc++fs)
    endif()
endfunction()
