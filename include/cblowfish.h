#ifndef CBLOWFISH_H_
#define CBLOWFISH_H_

#include <string>
#include <stdexcept>

namespace cblowfish {
    class Buffer {
    public:
        Buffer() = default;
        ~Buffer() = default;

        unsigned char* get() const { return data; }
    private:
        unsigned char* data = nullptr;
        size_t size = 0;
    };

    class BlowfishContext {
    public:
        BlowfishContext() = default;
        explicit BlowfishContext(const std::string& key);
        ~BlowfishContext() = default;
        BlowfishContext(const BlowfishContext& other) = delete;

        void initialize(const std::string& key);
        void encrypt(const Buffer& buffer);
        void decrypt(Buffer& buffer);
    private:
        void _encrypt(uint32_t* left, uint32_t* right);
        void _decrypt(uint32_t* left, uint32_t* right);
        uint32_t f(uint32_t x);

        uint32_t P_array[18];
        uint32_t S_boxes[4][256];
        bool initialized = false;
    };

    class AlreadyInitializedError : public std::logic_error {
    public:
        AlreadyInitializedError(const std::string& message)
            : std::logic_error(message) {}
        AlreadyInitializedError(const char* message)
            : std::logic_error(message) {}
    };
}

#endif  // CBLOWFISH_H_
