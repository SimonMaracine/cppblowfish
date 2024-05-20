cmake_minimum_required(VERSION 3.20)

option(CPPBLOWFISH_BUILD_TESTS "Build a tests executable alongside the library" OFF)
option(BUILD_SHARED_LIBS "Build the library as static or dynamic" OFF)

project("cppblowfish" LANGUAGES CXX)

add_library(cppblowfish
    "include/cppblowfish/details/blowfish_context.hpp"
    "include/cppblowfish/details/buffer.hpp"
    "include/cppblowfish/details/platform.hpp"
    "include/cppblowfish/details/version.hpp"
    "include/cppblowfish/cppblowfish.hpp"
    "src/blowfish_context.cpp"
    "src/buffer.cpp"
)

if(UNIX)
    target_compile_options(cppblowfish PRIVATE "-Wall" "-Wextra" "-Wpedantic" "-Wconversion")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(cppblowfish PRIVATE "/W4")
else()
    message(WARNING "Warnings are not enabled")
endif()

target_include_directories(cppblowfish PUBLIC "include")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(cppblowfish PRIVATE "NDEBUG")
    message(STATUS "cppblowfish: Building library in release mode")
endif()

target_compile_features(cppblowfish PRIVATE cxx_std_17)
set_target_properties(cppblowfish PROPERTIES CXX_EXTENSIONS OFF)

if(CPPBLOWFISH_BUILD_TESTS)
    add_subdirectory(tests)
endif()

message(STATUS "cppblowfish: Building shared library: ${BUILD_SHARED_LIBS}")
message(STATUS "cppblowfish: Building tests: ${CPPBLOWFISH_BUILD_TESTS}")
