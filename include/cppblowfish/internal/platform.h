// Should be included only in blowfish_context.cpp

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <cstdint>

#if SIZE_MAX != 0xFFFFFFFFFFFFFFFF
    #error "Unsupported platform"
#endif

inline bool is_little_endian() {
    volatile const uint32_t whatever = 0b0001;
    return reinterpret_cast<volatile const uint8_t*>(&whatever)[0] == 1;
}

#endif  // _PLATFORM_H_
