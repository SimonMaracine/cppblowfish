#include <ostream>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#include "internal/buffer.h"
#include "internal/errors.h"

#define MAX_STATIC_SIZE offsetof(Buffer, is_static)

namespace cppblowfish {
    Buffer::Buffer(const void* data, size_t size)
        : buffer_size(size), capacity(size) {
        this->data = new unsigned char[size];

        if (data != nullptr) {
            memcpy(this->data, data, size);
        }
    }

    Buffer::~Buffer() {
        if (!is_static) {
            delete[] data;
        }
    }

    Buffer::Buffer(const Buffer& other) {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        is_static = other.is_static;
        buffer_size = other.buffer_size;

        if (!other.is_static) {
            data = new unsigned char[other.capacity];
            memcpy(data, other.data, other.buffer_size + other.buffer_padding);
        }
    }

    Buffer& Buffer::operator=(const Buffer& other) {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        is_static = other.is_static;
        buffer_size = other.buffer_size;

        if (!other.is_static) {
            data = new unsigned char[other.capacity];
            memcpy(data, other.data, other.buffer_size + other.buffer_padding);
        }
    }

    Buffer::Buffer(Buffer&& other) {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        is_static = other.is_static;
        buffer_size = other.buffer_size;

        other.data = nullptr;
    }

    Buffer& Buffer::operator=(Buffer&& other) {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        is_static = other.is_static;
        buffer_size = other.buffer_size;

        other.data = nullptr;
    }

    Buffer& Buffer::operator+=(unsigned char character) {
        if (is_static) {
            if (buffer_size + 1 > MAX_STATIC_SIZE) {
                throw AllocationError(
                    "Buffer size exceeded the maximum static size: " + std::to_string(MAX_STATIC_SIZE)
                );
            }

            unsigned char* self = reinterpret_cast<unsigned char*>(this);

            self[buffer_size] = character;
            buffer_size++;
        } else {
            if (buffer_size + 1 > capacity) {
                reserve(buffer_size + 1);
            }

            data[buffer_size] = character;
            buffer_size++;
        }

        return *this;
    }

    Buffer& Buffer::operator+=(const Buffer& other) {
        if (is_static) {
            if (buffer_size + other.buffer_size > MAX_STATIC_SIZE) {
                throw AllocationError(
                    "Buffer size exceeded the maximum static size: " + std::to_string(MAX_STATIC_SIZE)
                );
            }

            unsigned char* self = reinterpret_cast<unsigned char*>(this);

            if (other.is_static) {
                memcpy(self + buffer_size, &other, other.buffer_size);
            } else {
                memcpy(self + buffer_size, other.data, other.buffer_size);
            }

            buffer_size += other.buffer_size;
        } else {
            if (buffer_size + other.buffer_size > capacity) {
                reserve(buffer_size + other.buffer_size);
            }

            if (other.is_static) {
                memcpy(data + buffer_size, &other, other.buffer_size);
            } else {
                memcpy(data + buffer_size, other.data, other.buffer_size);
            }

            buffer_size += other.buffer_size;
        }

        return *this;
    }

    unsigned char* Buffer::get() const {
        assert(!is_static && "Buffer::get not available for static buffer");

        return data;
    }

    size_t Buffer::padding() const {
        assert(!is_static && "Buffer::padding not available for static buffer");

        return buffer_padding;
    }

    void Buffer::reserve(size_t capacity) {
        assert(!is_static && "Buffer::reserve not available for static buffer");

        if (capacity < buffer_size) {
            throw AllocationError(
                "The new capacity is smaller than the current buffer size"
            );
        }

        unsigned char* new_data = new unsigned char[capacity];

        if (data != nullptr) {
            memcpy(new_data, data, this->buffer_size);
        }

        delete[] data;

        data = new_data;
        this->capacity = capacity;
    }

    Buffer Buffer::create_static() {
        Buffer buffer;

        buffer.is_static = true;

        return buffer;
    }

    Buffer Buffer::create_static(const void* data, size_t size) {
        if (size > MAX_STATIC_SIZE) {
            throw AllocationError(
                "Static size is limited to " + std::to_string(MAX_STATIC_SIZE) + " bytes"
            );
        }

        Buffer buffer;

        buffer.is_static = true;
        buffer.buffer_size = size;

        memcpy(&buffer, data, size);

        return buffer;
    }

    void Buffer::padd(size_t padd_count, unsigned char character) {
        assert(!is_static && "Buffer::padd not available for static buffer");  // TODO maybe use exceptions instead

        if (buffer_size + padd_count > capacity) {
            reserve(buffer_size + padd_count);
        }

        for (size_t i = 0; i < padd_count; i++) {
            data[buffer_size + i] = character;
        }

        buffer_padding = padd_count;
        buffer_size += padd_count;
    }

    Buffer Buffer::from_uint32(uint32_t x) {
        Buffer buffer = Buffer::create_static();

        for (size_t i = 0; i < 4; i++) {
            buffer += static_cast<unsigned char>(x >> i * 8);
        }

        return buffer;
    }

    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer) {
        if (buffer.is_static) {
            stream.write(reinterpret_cast<const char*>(&buffer), buffer.buffer_size);
        } else {
            stream.write(reinterpret_cast<const char*>(buffer.data), buffer.buffer_size);
        }

        return stream;
    }
}
