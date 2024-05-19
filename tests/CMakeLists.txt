cmake_minimum_required(VERSION 3.20)

add_executable(test "main.cpp")

target_link_libraries(test PRIVATE cppblowfish)

if(UNIX)
    target_compile_options(test PRIVATE "-Wall" "-Wextra" "-Wpedantic" "-Wconversion")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(test PRIVATE "/W3")
else()
    message(WARNING "Warnings are not enabled")
endif()

target_compile_features(test PRIVATE cxx_std_17)
set_target_properties(test PROPERTIES CXX_EXTENSIONS OFF)

file(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/shader.txt"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)
