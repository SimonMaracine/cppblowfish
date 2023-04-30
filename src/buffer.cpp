#include <ostream>
#include <string>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <cstddef>

#include "cppblowfish/internal/buffer.h"
#include "cppblowfish/internal/errors.h"

namespace cppblowfish {
    namespace internal {
        Uint32 repr_uint32(uint32_t x) {
            Uint32 buffer;

            for (size_t i = 0; i < 4; i++) {
                buffer.data[i] = static_cast<unsigned char>(x >> i * 8);
            }

            return buffer;
        }
    }

    Buffer::Buffer() {
        data = new unsigned char[BUFFER_OFFSET];
        memset(data, 0, BUFFER_OFFSET);
        capacity = BUFFER_OFFSET;
    }

    Buffer::Buffer(size_t size) {
        assert(size > 0);

        this->data = new unsigned char[size + BUFFER_OFFSET];

        memset(this->data, 0, BUFFER_OFFSET);

        capacity = size + BUFFER_OFFSET;
        buffer_data_padding = size;
    }

    Buffer::Buffer(const void* data, size_t size) {
        assert(data != nullptr);
        assert(size > 0);

        this->data = new unsigned char[size + BUFFER_OFFSET];
        memcpy(this->data + BUFFER_OFFSET, data, size);

        memset(this->data, 0, BUFFER_OFFSET);

        capacity = size + BUFFER_OFFSET;
        buffer_data_padding = size;
    }

    Buffer::~Buffer() {
        delete[] data;
    }

    Buffer::Buffer(const Buffer& other) {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_padding = other.buffer_data_padding;

        data = new unsigned char[other.capacity];
        memcpy(data, other.data, other.buffer_data_padding + BUFFER_OFFSET);
    }

    Buffer& Buffer::operator=(const Buffer& other) {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_padding = other.buffer_data_padding;

        data = new unsigned char[other.capacity];
        memcpy(data, other.data, other.buffer_data_padding + BUFFER_OFFSET);

        return *this;
    }

    Buffer::Buffer(Buffer&& other) noexcept {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_padding = other.buffer_data_padding;

        other.data = nullptr;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_padding = other.buffer_data_padding;

        other.data = nullptr;

        return *this;
    }

    Buffer& Buffer::operator+=(internal::Uint32 uint32) {
        constexpr size_t additional = sizeof(internal::Uint32);

        if (buffer_data_padding + BUFFER_OFFSET + additional > capacity) {
            reserve(buffer_data_padding + BUFFER_OFFSET + additional);
        }

        memcpy(data + buffer_data_padding + BUFFER_OFFSET, &uint32, additional);  // Uint32 is trivially copyable

        buffer_data_padding += additional;

        return *this;
    }

    const unsigned char* Buffer::get() const {
        return data + BUFFER_OFFSET;
    }

    unsigned char* Buffer::steal() {
        unsigned char* pointer = data;

        // Reset only the pointer, the other data remains intact
        data = nullptr;

        return pointer;
    }

    void Buffer::reserve(size_t capacity) {
        if (capacity < buffer_data_padding) {
            throw AllocationError("The new capacity is smaller than the current buffer size");
        }

        unsigned char* new_data = new unsigned char[capacity];

        if (data != nullptr) {
            memcpy(new_data, data, buffer_data_padding + BUFFER_OFFSET);
        }

        delete[] data;

        data = new_data;
        this->capacity = capacity;
    }

    Buffer Buffer::from_whole_data(const void* whole_data, size_t whole_size) {
        assert(whole_data != nullptr);
        assert(whole_size >= BUFFER_OFFSET);

        Buffer buffer;

        buffer.data = new unsigned char[whole_size];

        memcpy(buffer.data, whole_data, whole_size);
        buffer.capacity = whole_size;
        memcpy(&buffer.buffer_padding, whole_data, sizeof(size_t));
        buffer.buffer_data_padding = whole_size - BUFFER_OFFSET;

        return buffer;
    }

    void Buffer::write_whole_data(std::ostream& stream) const {
        Buffer::write_to_stream(stream, buffer_data_padding + BUFFER_OFFSET, data);
    }

    void Buffer::write_whole_data(unsigned char* out) const {
        assert(out != nullptr);

        memcpy(out, data, buffer_data_padding + BUFFER_OFFSET);
    }

    void Buffer::padd(size_t padd_count, unsigned char character) {
        assert(padd_count > 0);

        if (buffer_data_padding + padd_count + BUFFER_OFFSET > capacity) {
            reserve(buffer_data_padding + padd_count + BUFFER_OFFSET);
        }

        for (size_t i = 0; i < padd_count; i++) {
            data[buffer_data_padding + i + BUFFER_OFFSET] = character;
        }

        memcpy(data, &padd_count, sizeof(size_t));
        buffer_padding = padd_count;
        buffer_data_padding += padd_count;
    }

    void Buffer::write_to_stream(std::ostream& stream, size_t size, const void* data) {
        assert(size > 0);
        assert(data != nullptr);

        char* write_buffer = new char[size];
        memcpy(write_buffer, data, size);

        stream.write(write_buffer, size);

        delete[] write_buffer;
    }

    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer) {
        Buffer::write_to_stream(
            stream, buffer.buffer_data_padding - buffer.buffer_padding, buffer.data + BUFFER_OFFSET
        );

        return stream;
    }
}
