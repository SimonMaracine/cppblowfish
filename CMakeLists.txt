cmake_minimum_required(VERSION 3.20)

option(CPPBLOWFISH_BUILD_TESTS "Build a tests executable alongside the library" OFF)

project("cppblowfish"
    VERSION 0.6.0
    LANGUAGES CXX
)

set(HEADERS
    "include/cppblowfish/details/blowfish_context.h"
    "include/cppblowfish/details/buffer.h"
    "include/cppblowfish/details/errors.h"
    "include/cppblowfish/details/platform.h"
    "include/cppblowfish/details/version.h"
    "include/cppblowfish/cppblowfish.h"
)

add_library(cppblowfish STATIC
    "src/blowfish_context.cpp"
    "src/buffer.cpp"
    ${HEADERS}
)

if(${CPPBLOWFISH_BUILD_TESTS})
    add_executable(test "tests/main.cpp")
endif()

if(UNIX)
    target_compile_options(cppblowfish PRIVATE -Wall -Wextra)

    if(${CPPBLOWFISH_BUILD_TESTS})
        target_compile_options(test PRIVATE -Wall -Wextra)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(cppblowfish PRIVATE /W3)

    if(${CPPBLOWFISH_BUILD_TESTS})
        target_compile_options(test PRIVATE /W3)
    endif()
else()
    message(WARNING "Compiler is not MSVC or platform is not supported; warnings were not enabled")
endif()

target_include_directories(cppblowfish PUBLIC "include")

if(${CPPBLOWFISH_BUILD_TESTS})
    target_link_libraries(test PRIVATE cppblowfish)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(cppblowfish PRIVATE "NDEBUG")
    message(STATUS "Compiling cppblowfish in release mode")
endif()
