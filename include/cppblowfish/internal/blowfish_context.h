#ifndef _BLOWFISH_CONTEXT_H_
#define _BLOWFISH_CONTEXT_H_

#include <string>

#include "buffer.h"

namespace cppblowfish {
    class BlowfishContext {
    public:
        BlowfishContext() = default;
        explicit BlowfishContext(const std::string& key);
        ~BlowfishContext() = default;

        void initialize(const std::string& key);
        void encrypt(Buffer& input, Buffer& cipher);  // This can modify the input by adding padding
        void decrypt(const Buffer& cipher, Buffer& output);
    private:
        void _encrypt(uint32_t* left, uint32_t* right);
        void _decrypt(uint32_t* left, uint32_t* right);
        uint32_t f(uint32_t x);

        uint32_t P_array[18];
        uint32_t S_boxes[4][256];
        bool initialized = false;
    };
}

#endif  // _BLOWFISH_CONTEXT_H_
