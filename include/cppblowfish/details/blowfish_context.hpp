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

        // Constructors (and initialize) can throw exceptions: AlreadyInitializedError and KeyError
        explicit BlowfishContext(const std::string& key);
        explicit BlowfishContext(const void* key, std::size_t size);

        ~BlowfishContext() = default;

        BlowfishContext(const BlowfishContext&) = default;
        BlowfishContext& operator=(const BlowfishContext&) = default;
        BlowfishContext(BlowfishContext&&) noexcept = default;
        BlowfishContext& operator=(BlowfishContext&&) noexcept = default;

        // Initialize the context, if before it was default constructed
        void initialize(const void* key, std::size_t size);

        // Usual encryption-decryption
        void encrypt(const Buffer& input, Buffer& cipher);
        void decrypt(const Buffer& cipher, Buffer& output);

        // Low level API
        void encrypt_data(std::uint32_t* left, std::uint32_t* right);
        void decrypt_data(std::uint32_t* left, std::uint32_t* right);
    private:
        std::uint32_t f(std::uint32_t x) noexcept;

        std::uint32_t P_array[18];
        std::uint32_t S_boxes[4][256];
        bool initialized = false;
    };
}

#endif  // BLOWFISH_CONTEXT_H
