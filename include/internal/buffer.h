#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <ostream>

/*
    The buffer is as follows:

            _________________________________________________------ capacity
            |                                               |
            |        _____________________________----------------- buffer_size
            |        |                           |          |
            |        |                  __________----------------- buffer_padding
            |        |                  |        |          |
    *data = 00000000 XXXXXXXXXXXX...XXX PPPP...PPP YYYY...YYY
            ^        ^                  ^          ^
            0        1                  2          3

    0. This is padding size, a size_t representing the amount of padding that is at the end (the number of trailing bytes).
    1. The actual data, variable amount.
    2. The padding bytes, variable amount. buffer_padding and the first 8 bytes of the buffer represent this amount.
    3. Allocated, but unused memory.
*/

namespace cppblowfish {
    enum Staticity {
        NonStatic = 0,
        Static = 1
    };

    class Buffer {
    public:
        Buffer(Staticity static_ = NonStatic);
        Buffer(const void* data, size_t size, Staticity static_ = NonStatic);
        ~Buffer();
        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);
        Buffer(Buffer&& other);
        Buffer& operator=(Buffer&& other);

        Buffer& operator+=(unsigned char character);
        Buffer& operator+=(const Buffer& other);

        // Pointer to the data with an offset of BUFFER_OFFSET (padding size)
        unsigned char* get() const;

        size_t size() const { return buffer_size; }
        size_t padding() const;
        // The size of the actual data (without padding) is size() - padding()

        void reserve(size_t capacity);

        // Create new buffer from previous buffer's whole data
        static Buffer from_whole_data(const void* whole_data, size_t whole_size);

        // Writes all the data (data + padding + padding size)
        void write_whole_data(std::ostream& stream) const;

        static constexpr size_t max_static_size();
    private:
        void padd(size_t padd_count, unsigned char character);
        static Buffer from_uint32(uint32_t x);

        // The order of the members matters
        unsigned char* data = nullptr;
        size_t capacity = 0;  // The number of bytes allocated (buffer_size + padding size + unused bytes)
        size_t buffer_padding = 0;  // The size in bytes of the actual padding (it is also stored at the beginning of the buffer)

        Staticity static_ = NonStatic;
        size_t buffer_size = 0;  // The size in bytes of the actual data + actual padding

        friend class BlowfishContext;
        friend std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
    };

    // Writes only the actual data (no padding and no padding size)
    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer);
}

#endif  // _BUFFER_H_
