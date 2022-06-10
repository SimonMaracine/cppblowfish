cmake_minimum_required(VERSION 3.20)

set(LIB "cppblowfish")
set(TEST "test")

project(cppblowfish
    VERSION 0.1.0
    LANGUAGES CXX
)

add_library(${LIB} STATIC
    "src/blowfish.cpp"
    "src/buffer.cpp"
)

add_executable(${TEST} "tests/main.cpp")

target_include_directories(${LIB} PRIVATE "include")
target_include_directories(${TEST} PRIVATE "include")
target_link_libraries(${TEST} PRIVATE ${LIB})
