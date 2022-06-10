#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <ostream>

namespace cppblowfish {
    class Buffer {
    public:
        Buffer() = default;
        Buffer(const void* data, size_t size);
        ~Buffer();
        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);
        Buffer(Buffer&& other);
        Buffer& operator=(Buffer&& other);

        Buffer& operator+=(unsigned char character);
        Buffer& operator+=(const Buffer& other);

        unsigned char* get() const;

        // The size of the actual data (without padding) is size() - padding()
        size_t size() const { return buffer_size; }
        size_t padding() const;

        void reserve(size_t capacity);

        static Buffer create_static();
        static Buffer create_static(const void* data, size_t size);
    private:
        void padd(size_t padd_count, unsigned char character);
        static Buffer from_uint32(uint32_t x);

        // The order of members matters
        unsigned char* data = nullptr;
        size_t capacity = 0;
        size_t buffer_padding = 0;  // The size in bytes of the padding

        bool is_static = false;
        size_t buffer_size = 0;  // The size in bytes of the whole buffer + padding

        friend class BlowfishContext;
        friend std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
    };

    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
}

#endif  // _BUFFER_H_
