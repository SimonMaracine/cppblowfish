cmake_minimum_required(VERSION 3.20)

set(LIB "cppblowfish")
set(TEST "test")

option(CPPBLOWFISH_BUILD_TESTS "Build a tests executable alongside library" ON)

project(cppblowfish
    VERSION 0.1.2
    LANGUAGES CXX
)

set(HEADERS
    "include/cppblowfish/internal/blowfish_context.h"
    "include/cppblowfish/internal/buffer.h"
    "include/cppblowfish/internal/errors.h"
    "include/cppblowfish/cppblowfish.h"
)

add_library(${LIB} STATIC
    "src/blowfish_context.cpp"
    "src/buffer.cpp"
    ${HEADERS}
)

if(CPPBLOWFISH_BUILD_TESTS)
    add_executable(${TEST} "tests/main.cpp")
endif()

if(UNIX)
    target_compile_options(${LIB} PRIVATE -Wall -Wextra)

    if(CPPBLOWFISH_BUILD_TESTS)
        target_compile_options(${TEST} PRIVATE -Wall -Wextra)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    target_compile_options(${LIB} PRIVATE /W3)

    if(CPPBLOWFISH_BUILD_TESTS)
        target_compile_options(${TEST} PRIVATE /W3)
    endif()
else()
    message(WARNING "Compiler is not MSVC or GCC and warnings were not enabled")
endif()

target_include_directories(${LIB} PUBLIC "include")

if(CPPBLOWFISH_BUILD_TESTS)
    target_link_libraries(${TEST} PRIVATE ${LIB})
endif()
