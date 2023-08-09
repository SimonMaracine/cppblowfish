/*
    I know that these tests are not as rigorous as they could be and
    they don't really test all the functionality of the library.
    But still it looks okay to me. I'm just lazy about it.
*/

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstddef>

#include <cppblowfish/cppblowfish.hpp>

#include "unit_test.h"

DEFINE_TEST(basic_usage) {
    std::string key = "mySECRETkey1234";
    std::string message = "Hello, world. Why are you sad?";

    cppblowfish::BlowfishContext blowfish {key};

    cppblowfish::Buffer input {message.c_str(), message.size()};
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    ASSERT_EQ(input.size(), 30)
    ASSERT_EQ(input.padding(), 0)

    blowfish.encrypt(input, cipher);

    ASSERT_EQ(input.size(), 30)
    ASSERT_EQ(input.padding(), 0)

    ASSERT_EQ(cipher.get()[0], 0x5D)
    ASSERT_EQ(cipher.get()[1], 0x77)
    ASSERT_EQ(cipher.get()[2], 0x54)
    ASSERT_EQ(cipher.get()[3], 0xBC)
    ASSERT_EQ(cipher.get()[4], 0xD4)
    ASSERT_EQ(cipher.get()[5], 0xB4)
    ASSERT_EQ(cipher.get()[12], 0x65)
    ASSERT_EQ(cipher.get()[13], 0x25)
    ASSERT_EQ(cipher.get()[29], 0xE7)

    ASSERT_EQ(cipher.size(), 30)
    ASSERT_EQ(cipher.padding(), 2)

    blowfish.decrypt(cipher, output);

    ASSERT_EQ(output.get()[0], 'H')
    ASSERT_EQ(output.get()[1], 'e')
    ASSERT_EQ(output.get()[2], 'l')
    ASSERT_EQ(output.get()[3], 'l')
    ASSERT_EQ(output.get()[4], 'o')
    ASSERT_EQ(output.get()[5], ',')
    ASSERT_EQ(output.get()[6], ' ')
    ASSERT_EQ(output.get()[7], 'w')
    ASSERT_EQ(output.get()[13], ' ')
    ASSERT_EQ(output.get()[14], 'W')
    ASSERT_EQ(output.get()[27], 'a')
    ASSERT_EQ(output.get()[29], '?')

    ASSERT_EQ(output.size(), 30)
    ASSERT_EQ(output.padding(), 2)

    ASSERT_EQ(output.size(), message.size())
}

DEFINE_TEST(writing_cipher_to_file) {
    std::string key = "ThisIsMyKey19S";
    std::string message = "And this is a long message. Have a nice day!... Maybe it works. If you read this, then it works.";

    cppblowfish::BlowfishContext blowfish {key};

    cppblowfish::Buffer input {message.c_str(), message.size()};
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    ASSERT_EQ(input.size(), 96)
    ASSERT_EQ(input.padding(), 0)

    blowfish.encrypt(input, cipher);

    ASSERT_EQ(input.size(), 96)
    ASSERT_EQ(input.padding(), 0)

    ASSERT_EQ(cipher.get()[0], 0x25)
    ASSERT_EQ(cipher.get()[1], 0xB7)
    ASSERT_EQ(cipher.get()[2], 0x94)
    ASSERT_EQ(cipher.get()[3], 0x0C)
    ASSERT_EQ(cipher.get()[12], 0x2D)
    ASSERT_EQ(cipher.get()[13], 0xAD)
    ASSERT_EQ(cipher.get()[94], 0x74)
    ASSERT_EQ(cipher.get()[95], 0xD1)

    ASSERT_EQ(cipher.size(), 96)
    ASSERT_EQ(cipher.padding(), 8)

    {
        std::ofstream file {"cipher.txt", std::ios::binary | std::ios::trunc};
        if (!file.is_open()) { ASSERT(false) }
        cipher.write_whole_data(file);
    }

    {
        std::ifstream file {"cipher.txt", std::ios::binary};
        if (!file.is_open()) { ASSERT(false) }
        file.seekg(0, file.end);
        const size_t length = file.tellg();
        file.seekg(0, file.beg);
        char* raw_buffer = new char[length];
        file.read(raw_buffer, length);
        cipher = cppblowfish::Buffer::from_whole_data(raw_buffer, length);
        delete[] raw_buffer;
    }

    blowfish.decrypt(cipher, output);

    ASSERT_EQ(output.get()[0], 'A')
    ASSERT_EQ(output.get()[1], 'n')
    ASSERT_EQ(output.get()[2], 'd')
    ASSERT_EQ(output.get()[3], ' ')
    ASSERT_EQ(output.get()[4], 't')
    ASSERT_EQ(output.get()[5], 'h')
    ASSERT_EQ(output.get()[13], ' ')
    ASSERT_EQ(output.get()[14], 'l')
    ASSERT_EQ(output.get()[94], 's')
    ASSERT_EQ(output.get()[95], '.')

    ASSERT_EQ(output.size(), 96)
    ASSERT_EQ(output.padding(), 8)

    ASSERT_EQ(output.size(), message.size())
}

DEFINE_TEST(buffer) {
    constexpr size_t size = 4;
    char data[size] = { 'L', 'i', 'n', 'u' };
    cppblowfish::Buffer buffer {data, size};

    ASSERT_EQ(std::memcmp(buffer.get(), data, size), 0)
    ASSERT_EQ(buffer.size(), size)
    ASSERT_EQ(buffer.padding(), 0)

    buffer += cppblowfish::internal::repr_uint32('x');

    ASSERT_EQ(buffer.get()[4], 'x')
    ASSERT_EQ(buffer.size(), size + 4)
    ASSERT_EQ(buffer.padding(), 0)
}

DEFINE_TEST(bigger_data) {
    std::string key = "some_random_not_great_key";

    cppblowfish::BlowfishContext blowfish {key};

    cppblowfish::Buffer input;

    {
        std::ifstream file {"shader.txt", std::ios::binary};
        if (!file.is_open()) { ASSERT(false) }
        file.seekg(0, file.end);
        const size_t length = file.tellg();
        file.seekg(0, file.beg);
        char* raw_buffer = new char[length];
        file.read(raw_buffer, length);
        input = cppblowfish::Buffer(raw_buffer, length);
        delete[] raw_buffer;
    }

    cppblowfish::Buffer cipher;
    blowfish.encrypt(input, cipher);

    cppblowfish::Buffer output;
    blowfish.decrypt(cipher, output);

    ASSERT_EQ(input.size(), output.size())
    ASSERT_EQ(std::memcmp(input.get(), output.get(), input.size()), 0)
}

int main() {
    INITIALIZE_UNIT_TEST()
    TEST(basic_usage)
    TEST(writing_cipher_to_file)
    TEST(buffer)
    TEST(bigger_data)
    END_UNIT_TEST()

    std::string key = "mySECRETkey1234";
    std::string message = "Hello, world. Why are you sad?";

    cppblowfish::BlowfishContext blowfish {key};

    cppblowfish::Buffer input {message.c_str(), message.size()};
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    std::cout << "input: " << input << '\n';
    std::cout << "input size: " << input.size() << '\n';

    blowfish.encrypt(input, cipher);

    std::cout << "cipher: " << cipher << '\n';
    std::cout << "cipher size: " << cipher.size() << '\n';

    blowfish.decrypt(cipher, output);

    std::cout << "output: " << output << '\n';
    std::cout << "output size: " << output.size() << '\n';
    std::cout << "input, cipher, output padding: " << input.padding()
        << ", " << cipher.padding() << ", " << output.padding() << '\n';
}
