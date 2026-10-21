# Standalone build of the ZlibCodec test program.
#
# To use ZlibCodec in your own library you don't need this file - see
# cmake/ZlibCodec.cmake (include it and call zlibcodec_add_to_target).

cmake_minimum_required(VERSION 3.18)

project(ZlibCodec LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/ZlibCodec.cmake)

add_executable(ZlibTest example/main.cpp)
zlibcodec_add_to_target(ZlibTest)
