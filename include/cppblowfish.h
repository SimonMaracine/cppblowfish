#ifndef CPPBLOWFISH_H_
#define CPPBLOWFISH_H_

#include <string>
#include <stdexcept>

namespace cppblowfish {
    class Buffer {
    public:
        Buffer() = default;
        Buffer(void* data, size_t size);
        ~Buffer();
        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);
        Buffer(Buffer&& other);
        Buffer& operator=(Buffer&& other);

        unsigned char* get() const { return data; }
        size_t size() const { return buffer_size; }

        void reserve(size_t capacity);
        void padd(size_t count, unsigned char character);
    private:
        unsigned char* data = nullptr;
        size_t buffer_size = 0;
        size_t capacity = 0;
        size_t padding = 0;
    };

    class BlowfishContext {
    public:
        BlowfishContext() = default;
        explicit BlowfishContext(const std::string& key);
        ~BlowfishContext() = default;
        BlowfishContext(const BlowfishContext& other) = delete;

        void initialize(const std::string& key);
        void encrypt(const Buffer& data, Buffer& cipher);
        void decrypt(const Buffer& cipher, Buffer& data);
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

    class AllocationError : public std::logic_error {
    public:
        AllocationError(const std::string& message)
            : std::logic_error(message) {}
        AllocationError(const char* message)
            : std::logic_error(message) {}
    };
}

#endif  // CPPBLOWFISH_H_
