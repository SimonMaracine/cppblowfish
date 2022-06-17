#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <ostream>

namespace cppblowfish {
    class Buffer {
    public:
        Buffer();
        Buffer(const void* data, size_t size);
        ~Buffer();
        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);
        Buffer(Buffer&& other);
        Buffer& operator=(Buffer&& other);

        Buffer& operator+=(unsigned char character);
        Buffer& operator+=(const Buffer& other);

        // Pointer to the data with an offset of BUFFER_OFFSET (8 bytes)
        unsigned char* get() const;

        // The size of the actual data (without padding) is size() - padding()
        size_t size() const { return buffer_size; }
        size_t padding() const;

        void reserve(size_t capacity);

        static Buffer create_static();
        static Buffer create_static(const void* data, size_t size);

        static Buffer from_raw_data(const void* data, size_t size);

        // Writes all the data (padding + padding size)
        void write_raw_data(std::ostream& stream);
    private:
        void padd(size_t padd_count, unsigned char character);
        static Buffer from_uint32(uint32_t x);

        // The order of members matters
        unsigned char* data = nullptr;
        size_t capacity = 0;  // The number of bytes allocated (buffer_size + buffer_padding + BUFFER_OFFSET)
        size_t buffer_padding = 0;  // The size in bytes of the padding (it is also stored at the beginning of the buffer)

        bool is_static = false;
        size_t buffer_size = 0;  // The size in bytes of the buffer + padding - BUFFER_OFFSET (8 bytes)

        friend class BlowfishContext;
        friend std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
    };

    // Writes only the actual data (without padding and without padding size)
    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
}

#endif  // _BUFFER_H_
