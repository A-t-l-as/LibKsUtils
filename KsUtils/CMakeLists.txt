cmake_minimum_required(VERSION 3.16)

project(KsUtils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(KsUtils STATIC
  ksutils.hpp
  ksutils.cpp
  src/BasicDataStructures/OrderedSet.hpp src/Compilator/CompilatorBase.cpp src/Compilator/CompilatorBase.hpp src/Compilator/CompilatorStrings.hpp src/Compilator/CompilatorValueTypes.hpp src/Compilator/Enums.hpp src/Compilator/HashMapsBase.hpp src/Console/Console.cpp src/Console/Console.hpp src/Console/MessPrinter.hpp src/Console/MessScanner.hpp src/File/BinFile.cpp src/File/BinFile.hpp src/File/MyFile.hpp src/File/OrderFileBase.hpp src/File/TextFile.hpp src/System/HybridApp.hpp src/System/IApp.hpp src/System/MySystem.hpp src/System/Platform.hpp src/Utils/BitsetUtils.hpp src/Utils/CppWriter.hpp src/Utils/Formats.hpp src/Utils/Globals.hpp src/Utils/Misc.hpp
  src/File/CppFile.hpp
  src/Compilator/SimpleCompilator.hpp
  src/BasicDataStructures/TwoTypeArray.hpp
  src/Utils/StringUtils.hpp
  src/Utils/TxtUtils.hpp
  src/System/MyGuid.hpp
  src/System/GuidUtils/CrossGuidStruct.hpp src/System/GuidUtils/MyGuidStruct.hpp src/System/GuidUtils/Win32GuidStruct.hpp








)

target_include_directories(KsUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(KsUtils PRIVATE KSUTILS_LIBRARY)
