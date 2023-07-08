cmake_minimum_required(VERSION 3.20)

add_executable(test "main.cpp")

target_link_libraries(test PRIVATE cppblowfish)

if(UNIX)
    target_compile_options(test PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(test PRIVATE /W3)
else()
    message(WARNING "Compiler is not MSVC or platform is not supported; warnings were not enabled")
endif()
