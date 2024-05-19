// Should be included only in blowfish_context.cpp

#ifndef PLATFORM_H
#define PLATFORM_H

#include <cstdint>

// Support only 64-bit architectures
#if SIZE_MAX != 0xFFFFFFFFFFFFFFFF
    #error "Unsupported platform"
#endif

inline bool is_little_endian() {
    const volatile std::uint32_t whatever {0b0001};
    return reinterpret_cast<const volatile unsigned char*>(&whatever)[0] == 1;
}

#endif  // PLATFORM_H
