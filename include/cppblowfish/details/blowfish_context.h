#ifndef BLOWFISH_CONTEXT_H
#define BLOWFISH_CONTEXT_H

#include <string>
#include <cstdint>
#include <cstddef>

namespace cppblowfish {
    class Buffer;

    class BlowfishContext {
    public:
        BlowfishContext() = default;
        explicit BlowfishContext(const std::string& key);
        explicit BlowfishContext(const void* key, size_t size);
        ~BlowfishContext() = default;

        BlowfishContext(const BlowfishContext&) = default;
        BlowfishContext& operator=(const BlowfishContext&) = default;
        BlowfishContext(BlowfishContext&&) noexcept = default;
        BlowfishContext& operator=(BlowfishContext&&) noexcept = default;

        void initialize(const void* key, size_t size);
        void encrypt(const Buffer& input, Buffer& cipher);
        void decrypt(const Buffer& cipher, Buffer& output);

        // Low level API
        void encrypt_data(uint32_t* left, uint32_t* right);
        void decrypt_data(uint32_t* left, uint32_t* right);
    private:
        uint32_t f(uint32_t x) noexcept;

        uint32_t P_array[18];
        uint32_t S_boxes[4][256];
        bool initialized = false;
    };
}

#endif  // BLOWFISH_CONTEXT_H
