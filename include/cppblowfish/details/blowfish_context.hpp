#ifndef BLOWFISH_CONTEXT_H
#define BLOWFISH_CONTEXT_H

#include <string>
#include <cstdint>
#include <cstddef>

#include "cppblowfish/details/buffer.hpp"

namespace cppblowfish {
    class BlowfishContext {
    public:
        BlowfishContext() = default;
        explicit BlowfishContext(const std::string& key);
        BlowfishContext(const char* key, std::size_t size);

        // Initialize the context, if before it was default constructed
        // The key must be between 32 and 448 bits (4 and 56 bytes respectively)
        // To change the key for an already initialized context, create an another context
        void initialize(const char* key, std::size_t size);

        // Usual encryption-decryption
        void encrypt(const Buffer& input, Buffer& cipher);
        void decrypt(const Buffer& cipher, Buffer& output);

        // Low level API
        void encrypt_data(std::uint32_t* left, std::uint32_t* right);
        void decrypt_data(std::uint32_t* left, std::uint32_t* right);
    private:
        std::uint32_t f(std::uint32_t x);

        std::uint32_t P_array[18u] {};
        std::uint32_t S_boxes[4u][256u] {};
        bool initialized {false};
    };
}

#endif  // BLOWFISH_CONTEXT_H
