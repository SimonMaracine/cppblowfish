cmake_minimum_required(VERSION 3.20)

set(LIB "cppblowfish")
set(TEST "test")

if(NOT DEFINED CPPBLOWFISH_BUILD_TESTS)
    set(CPPBLOWFISH_BUILD_TESTS ON)
endif()

project(cppblowfish
    VERSION 0.1.1
    LANGUAGES CXX
)

set(HEADERS_FOR_WINDOWS
    "include/cppblowfish/internal/blowfish_context.h"
    "include/cppblowfish/internal/buffer.h"
    "include/cppblowfish/internal/errors.h"
    "include/cppblowfish/cppblowfish.h"
)

add_library(${LIB} STATIC
    "src/blowfish_context.cpp"
    "src/buffer.cpp"
    ${HEADERS_FOR_WINDOWS}
)

if(CPPBLOWFISH_BUILD_TESTS)
    add_executable(${TEST} "tests/main.cpp")
endif()

if(UNIX)
    target_compile_options(${LIB} PUBLIC
        -Wall -Wextra
    )
    if(CPPBLOWFISH_BUILD_TESTS)
        target_compile_options(${TEST} PUBLIC
            -Wall -Wextra
        )
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    target_compile_options(${LIB} PUBLIC -Wall)
    if(CPPBLOWFISH_BUILD_TESTS)
        target_compile_options(${TEST} PUBLIC -Wall)
    endif()
else()
    message(WARNING "Compiler is not MSVC or other and warnings were not enabled")
endif()

target_include_directories(${LIB} PRIVATE "include")
if(CPPBLOWFISH_BUILD_TESTS)
    target_include_directories(${TEST} PRIVATE "include")
    target_link_libraries(${TEST} PRIVATE ${LIB})
endif()
