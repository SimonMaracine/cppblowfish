cmake_minimum_required(VERSION 3.20)

set(LIB "cppblowfish")
set(TEST "test")

project(cppblowfish
    VERSION 0.1.0
    LANGUAGES CXX
)

set(HEADERS_FOR_WINDOWS
    "include/internal/blowfish_context.h"
    "include/internal/buffer.h"
    "include/internal/errors.h"
    "include/cppblowfish.h"
)

add_library(${LIB} STATIC
    "src/blowfish_context.cpp"
    "src/buffer.cpp"
    ${HEADERS_FOR_WINDOWS}
)

add_executable(${TEST} "tests/main.cpp")

if(UNIX)
    target_compile_options(${LIB} PUBLIC
        -Wall -Wextra
    )
    target_compile_options(${TEST} PUBLIC
        -Wall -Wextra
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    target_compile_options(${LIB} PUBLIC -Wall)
    target_compile_options(${TEST} PUBLIC -Wall)
else()
    message(WARNING "Compiler is not MSVC or other and warnings were not enabled")
endif()

target_include_directories(${LIB} PRIVATE "include")
target_include_directories(${TEST} PRIVATE "include")
target_link_libraries(${TEST} PRIVATE ${LIB})
