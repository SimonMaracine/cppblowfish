#include <ostream>
#include <string>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#include "cppblowfish/internal/buffer.h"
#include "cppblowfish/internal/errors.h"

#define MAX_STATIC_SIZE offsetof(Buffer, static_)

namespace cppblowfish {
    Buffer::Buffer(Staticity static_)
        : static_(static_) {
        if (!static_) {
            data = new unsigned char[BUFFER_OFFSET];
            capacity = BUFFER_OFFSET;
            memset(data, 0, BUFFER_OFFSET);
        }
    }

    Buffer::Buffer(const void* data, size_t size, Staticity static_)
        : static_(static_), buffer_size(size) {
        if (static_) {
            if (size > MAX_STATIC_SIZE) {
                throw AllocationError(
                    "Static size is limited to " + std::to_string(MAX_STATIC_SIZE) + " bytes"
                );
            }

            unsigned char* self = reinterpret_cast<unsigned char*>(this);

            memcpy(self, data, size);
        } else {
            this->data = new unsigned char[size + BUFFER_OFFSET];

            if (data != nullptr) {
                memcpy(this->data + BUFFER_OFFSET, data, size);
            }

            memset(this->data, 0, BUFFER_OFFSET);

            capacity = size + BUFFER_OFFSET;
        }
    }

    Buffer::~Buffer() {
        if (!static_) {
            delete[] data;
        }
    }

    Buffer::Buffer(const Buffer& other) {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        static_ = other.static_;
        buffer_size = other.buffer_size;

        if (!other.static_) {
            data = new unsigned char[other.capacity];
            memcpy(data, other.data, other.buffer_size + other.buffer_padding);
        }
    }

    Buffer& Buffer::operator=(const Buffer& other) {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        static_ = other.static_;
        buffer_size = other.buffer_size;

        if (!other.static_) {
            data = new unsigned char[other.capacity];
            memcpy(data, other.data, other.buffer_size + other.buffer_padding);
        }

        return *this;
    }

    Buffer::Buffer(Buffer&& other) {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        static_ = other.static_;
        buffer_size = other.buffer_size;

        other.data = nullptr;
    }

    Buffer& Buffer::operator=(Buffer&& other) {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        static_ = other.static_;
        buffer_size = other.buffer_size;

        other.data = nullptr;

        return *this;
    }

    Buffer& Buffer::operator+=(unsigned char character) {
        if (static_) {
            if (buffer_size + 1 > MAX_STATIC_SIZE) {
                throw AllocationError(
                    "Buffer size exceeded the maximum static size: " + std::to_string(MAX_STATIC_SIZE)
                );
            }

            unsigned char* self = reinterpret_cast<unsigned char*>(this);

            self[buffer_size] = character;
            buffer_size++;
        } else {
            if (buffer_size + 1 + BUFFER_OFFSET > capacity) {
                reserve(buffer_size + 1 + BUFFER_OFFSET);
            }

            data[buffer_size + BUFFER_OFFSET] = character;
            buffer_size++;
        }

        return *this;
    }

    Buffer& Buffer::operator+=(const Buffer& other) {
        if (static_) {
            if (buffer_size + other.buffer_size > MAX_STATIC_SIZE) {
                throw AllocationError(
                    "Buffer size exceeded the maximum static size: " + std::to_string(MAX_STATIC_SIZE)
                );
            }

            unsigned char* self = reinterpret_cast<unsigned char*>(this);

            if (other.static_) {
                memcpy(self + buffer_size, &other, other.buffer_size);
            } else {
                memcpy(self + buffer_size, other.data + BUFFER_OFFSET, other.buffer_size);
            }

            buffer_size += other.buffer_size;
        } else {
            if (buffer_size + other.buffer_size + BUFFER_OFFSET > capacity) {
                reserve(buffer_size + other.buffer_size + BUFFER_OFFSET);
            }

            if (other.static_) {
                memcpy(data + buffer_size + BUFFER_OFFSET, &other, other.buffer_size);
            } else {
                memcpy(data + buffer_size + BUFFER_OFFSET, other.data + BUFFER_OFFSET, other.buffer_size);
            }

            buffer_size += other.buffer_size;
        }

        return *this;
    }

    unsigned char* Buffer::get() const {
        if (static_) {
            return reinterpret_cast<unsigned char*>(const_cast<Buffer*>(this));
        } else {
            return data + BUFFER_OFFSET;
        }
    }

    size_t Buffer::padding() const {
        assert(!static_ && "Buffer::padding not available for static buffer");

        return buffer_padding;
    }

    void Buffer::reserve(size_t capacity) {
        assert(!static_ && "Buffer::reserve not available for static buffer");

        if (capacity < buffer_size) {
            throw AllocationError("The new capacity is smaller than the current buffer size");
        }

        unsigned char* new_data = new unsigned char[capacity];

        if (data != nullptr) {
            memcpy(new_data, data, buffer_size + BUFFER_OFFSET);
        }

        delete[] data;

        data = new_data;
        this->capacity = capacity;
    }

    Buffer Buffer::from_whole_data(const void* whole_data, size_t whole_size) {
        Buffer buffer;

        buffer.data = new unsigned char[whole_size];

        assert(whole_data != nullptr);
        assert(whole_size >= BUFFER_OFFSET);

        memcpy(buffer.data, whole_data, whole_size);

        buffer.buffer_size = whole_size - BUFFER_OFFSET;
        buffer.capacity = whole_size;
        buffer.buffer_padding = *reinterpret_cast<size_t*>(const_cast<void*>(whole_data));

        return buffer;
    }

    void Buffer::write_whole_data(std::ostream& stream) const {
        if (static_) {
            assert(buffer_padding == 0);
            stream.write(reinterpret_cast<const char*>(this), buffer_size);
        } else {
            stream.write(reinterpret_cast<const char*>(data), buffer_size + BUFFER_OFFSET);
        }
    }

    void Buffer::write_whole_data(unsigned char* out) const {
        if (static_) {
            assert(buffer_padding == 0);
            memcpy(out, reinterpret_cast<unsigned char*>(const_cast<Buffer*>(this)), buffer_size);
        } else {
            memcpy(out, reinterpret_cast<const char*>(data), buffer_size + BUFFER_OFFSET);
        }
    }

    constexpr size_t Buffer::max_static_size() {
        return MAX_STATIC_SIZE;
    }

    void Buffer::padd(size_t padd_count, unsigned char character) {
        assert(!static_ && "Buffer::padd not available for static buffer");  // TODO maybe use exceptions instead

        if (buffer_size + padd_count + BUFFER_OFFSET > capacity) {
            reserve(buffer_size + padd_count + BUFFER_OFFSET);
        }

        for (size_t i = 0; i < padd_count; i++) {
            data[buffer_size + i + BUFFER_OFFSET] = character;
        }

        buffer_padding = padd_count;
        buffer_size += padd_count;
        memcpy(data, &padd_count, sizeof(padd_count));
    }

    Buffer Buffer::from_uint32(uint32_t x) {
        Buffer buffer (Static);

        for (size_t i = 0; i < 4; i++) {
            buffer += static_cast<unsigned char>(x >> i * 8);
        }

        return buffer;
    }

    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer) {
        if (buffer.static_) {
            assert(buffer.buffer_padding == 0);
            stream.write(reinterpret_cast<const char*>(&buffer), buffer.buffer_size);
        } else {
            stream.write(reinterpret_cast<const char*>(buffer.data + BUFFER_OFFSET),
                    buffer.buffer_size - buffer.buffer_padding);
        }

        return stream;
    }
}
