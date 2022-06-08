cmake_minimum_required(VERSION 3.20)

set(LIB "cblowfish")

project(cblowfish
    VERSION 0.1.0
    LANGUAGES CXX
)

add_library(${LIB} STATIC "src/cblowfish.cpp")

target_include_directories(${LIB} PRIVATE "include")
