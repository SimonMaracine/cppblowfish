#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <ostream>
#include <cstdint>

/*
    The buffer is as follows:

            _________________________________________________------ capacity
            |                                               |
            |        _____________________________----------------- buffer_data_padding
            |        |                           |          |
            |        |                  __________----------------- buffer_padding
            |        |                  |        |          |
    *data = 00000000 XXXXXXXXXXXX...XXX PPPP...PPP YYYY...YYY
            ^        ^                  ^          ^
            0        1                  2          3

    0. The padding size, a size_t representing the amount of padding that is at the end (number of trailing bytes).
    1. The actual data, variable amount.
    2. The padding bytes, variable amount. buffer_padding and the first 8 bytes of the buffer represent this amount.
    3. Allocated, but unused memory.
*/

namespace cppblowfish {
    inline constexpr size_t BUFFER_OFFSET = sizeof(size_t);  // Padding size

    namespace internal {
        struct Uint32 {
            unsigned char data[4];
        };

        Uint32 repr_uint32(uint32_t x);
    }

    class Buffer {
    public:
        Buffer();
        Buffer(size_t size);
        Buffer(const void* data, size_t size);
        ~Buffer();

        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);
        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;

        Buffer& operator+=(internal::Uint32 uint32);

        // Pointer to the actual data (with an offset of BUFFER_OFFSET)
        const unsigned char* get() const;

        // Get a pointer to the underlying data and invalidate the buffer (reset the pointer)
        // To access the actual data, add BUFFER_OFFSET to it
        unsigned char* steal();

        // Actual data size
        size_t size() const { return buffer_data_padding - buffer_padding; }

        size_t padding() const { return buffer_padding; }

        void reserve(size_t capacity);

        // Create a new buffer from a previous buffer's whole data (write_whole_data())
        static Buffer from_whole_data(const void* whole_data, size_t whole_size);

        // Writes all the data (padding size + data + padding); not the additional allocated memory
        void write_whole_data(std::ostream& stream) const;
        void write_whole_data(unsigned char* out) const;  // out is a pointer to a buffer allocated by you
                                                          // and should have the size as size() + padding() + BUFFER_OFFSET
    private:
        void padd(size_t padd_count, unsigned char character);
        static void write_to_stream(std::ostream& stream, size_t size, const unsigned char* data);

        unsigned char* data = nullptr;

        size_t capacity = 0;  // The number of bytes allocated (padding size + buffer data padding + unused bytes)
        size_t buffer_padding = 0;  // The size in bytes of the actual padding (also stored at the beginning of the buffer)
        size_t buffer_data_padding = 0;  // The size in bytes of the actual data + the actual padding

        friend class BlowfishContext;
        friend std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
    };

    // Writes only the actual data (no padding and no padding size)
    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
}

#endif  // _BUFFER_H_
