// Should be included only in blowfish_context.cpp

#ifndef PLATFORM_H
#define PLATFORM_H

#include <cstdint>

// Support only 64-bit architectures
#if SIZE_MAX != 0xFFFFFFFFFFFFFFFFul
    #error "Unsupported platform"
#endif

inline bool is_little_endian() {
    volatile const std::uint32_t whatever = 0b0001u;
    return reinterpret_cast<volatile const unsigned char*>(&whatever)[0u] == 1u;
}

#endif  // PLATFORM_H
