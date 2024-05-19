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
        void encrypt(const Buffer& input, Buffer& cipher) const;
        void decrypt(const Buffer& cipher, Buffer& output) const;

        // Low level API
        void encrypt_data(std::uint32_t* left, std::uint32_t* right) const;
        void decrypt_data(std::uint32_t* left, std::uint32_t* right) const;
    private:
        std::uint32_t f(std::uint32_t x) const;

        std::uint32_t P_array[18] {};
        std::uint32_t S_boxes[4][256] {};
        bool initialized {false};
    };
}

#endif  // BLOWFISH_CONTEXT_H
