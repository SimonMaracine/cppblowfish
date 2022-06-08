cmake_minimum_required(VERSION 3.20)

set(LIB "c++blowfish")

project(c++blowfish
    VERSION 0.1.0
    LANGUAGES CXX
)

add_library(${LIB} STATIC "src/c++blowfish.cpp")

target_include_directories(${LIB} PRIVATE "include")
