#include "cppblowfish/details/buffer.hpp"

#include <cstring>
#include <cassert>
#include <ostream>

namespace cppblowfish {
    namespace internal {
        Uint32 repr_uint32(std::uint32_t x) {
            Uint32 buffer;

            for (std::size_t i {0}; i < 4; i++) {
                buffer.data[i] = static_cast<unsigned char>(x >> i * 8);
            }

            return buffer;
        }
    }

    Buffer::Buffer() {
        data = new unsigned char[BUFFER_OFFSET];
        std::memset(data, 0, BUFFER_OFFSET);
        capacity = BUFFER_OFFSET;
    }

    Buffer::Buffer(std::size_t size) {
        assert(size > 0);

        data = new unsigned char[size + BUFFER_OFFSET];

        std::memset(data, 0, BUFFER_OFFSET);

        capacity = size + BUFFER_OFFSET;
        buffer_data_and_padding = size;
    }

    Buffer::Buffer(const void* data, std::size_t size) {
        assert(data != nullptr);
        assert(size > 0);

        this->data = new unsigned char[size + BUFFER_OFFSET];
        std::memcpy(this->data + BUFFER_OFFSET, data, size);

        std::memset(this->data, 0, BUFFER_OFFSET);

        capacity = size + BUFFER_OFFSET;
        buffer_data_and_padding = size;
    }

    Buffer::~Buffer() {
        delete[] data;
    }

    Buffer::Buffer(const Buffer& other) {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_and_padding = other.buffer_data_and_padding;

        data = new unsigned char[other.capacity];
        std::memcpy(data, other.data, other.buffer_data_and_padding + BUFFER_OFFSET);
    }

    Buffer& Buffer::operator=(const Buffer& other) {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_and_padding = other.buffer_data_and_padding;

        data = new unsigned char[other.capacity];
        std::memcpy(data, other.data, other.buffer_data_and_padding + BUFFER_OFFSET);

        return *this;
    }

    Buffer::Buffer(Buffer&& other) noexcept {
        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_and_padding = other.buffer_data_and_padding;

        other.data = nullptr;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        delete[] data;

        data = other.data;
        capacity = other.capacity;
        buffer_padding = other.buffer_padding;
        buffer_data_and_padding = other.buffer_data_and_padding;

        other.data = nullptr;

        return *this;
    }

    Buffer& Buffer::operator+=(internal::Uint32 uint32) {
        static constexpr std::size_t additional {sizeof(internal::Uint32)};

        if (buffer_data_and_padding + BUFFER_OFFSET + additional > capacity) {
            reserve(buffer_data_and_padding + additional);
        }

        // Uint32 is trivially copyable
        std::memcpy(data + buffer_data_and_padding + BUFFER_OFFSET, &uint32, additional);

        buffer_data_and_padding += additional;

        return *this;
    }

    const unsigned char* Buffer::get() const {
        return data + BUFFER_OFFSET;
    }

    unsigned char* Buffer::steal() {
        unsigned char* pointer {data + BUFFER_OFFSET};

        // Reset only the pointer, the other member data remain intact
        data = nullptr;

        return pointer;
    }

    void Buffer::reserve(std::size_t size) {
        assert(size > buffer_data_and_padding);

        unsigned char* new_data {new unsigned char[size + BUFFER_OFFSET]};

        if (data != nullptr) {
            std::memcpy(new_data, data, buffer_data_and_padding + BUFFER_OFFSET);
        }

        delete[] data;

        data = new_data;
        capacity = size + BUFFER_OFFSET;
    }

    Buffer Buffer::read_whole_data(const void* whole_data, std::size_t whole_size) {
        assert(whole_data != nullptr);
        assert(whole_size >= BUFFER_OFFSET);

        Buffer buffer {whole_size - BUFFER_OFFSET};

        std::memcpy(buffer.data, whole_data, whole_size);
        buffer.capacity = whole_size;
        std::memcpy(&buffer.buffer_padding, whole_data, sizeof(std::size_t));
        buffer.buffer_data_and_padding = whole_size - BUFFER_OFFSET;

        return buffer;
    }

    void Buffer::write_whole_data(std::ostream& stream) const {
        Buffer::write_to_stream(stream, buffer_data_and_padding + BUFFER_OFFSET, data);
    }

    void Buffer::write_whole_data(unsigned char* out) const {
        assert(out != nullptr);

        std::memcpy(out, data, buffer_data_and_padding + BUFFER_OFFSET);
    }

    void Buffer::padd(std::size_t padd_count, unsigned char character) {
        assert(padd_count > 0);

        if (buffer_data_and_padding + padd_count + BUFFER_OFFSET > capacity) {
            reserve(buffer_data_and_padding + padd_count);
        }

        for (std::size_t i {0}; i < padd_count; i++) {
            data[buffer_data_and_padding + i + BUFFER_OFFSET] = character;
        }

        std::memcpy(data, &padd_count, sizeof(std::size_t));
        buffer_padding = padd_count;
        buffer_data_and_padding += padd_count;
    }

    void Buffer::write_to_stream(std::ostream& stream, std::size_t size, const unsigned char* data) {
        assert(size > 0);
        assert(data != nullptr);

        // This is safe
        stream.write(reinterpret_cast<const char*>(data), size);
    }

    std::ostream& operator<<(std::ostream& stream, const Buffer& buffer) {
        Buffer::write_to_stream(
            stream,
            buffer.buffer_data_and_padding - buffer.buffer_padding,
            buffer.data + BUFFER_OFFSET
        );

        return stream;
    }
}
