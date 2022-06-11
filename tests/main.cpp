#include <string>
#include <iostream>
#include <fstream>

#include <cppblowfish.h>

#include "unit_test.h"

DEFINE_TEST(basic_usage)
    std::string key = "mySECRETkey1234";
    std::string message = "Hello, world. Why are you sad?";

    cppblowfish::BlowfishContext blowfish (key);

    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    blowfish.encrypt(input, cipher);

    ASSERT_EQ(cipher.get()[0], 0x5D)
    ASSERT_EQ(cipher.get()[1], 0x77)
    ASSERT_EQ(cipher.get()[2], 0x54)
    ASSERT_EQ(cipher.get()[3], 0xBC)
    ASSERT_EQ(cipher.get()[4], 0xD4)
    ASSERT_EQ(cipher.get()[5], 0xB4)
    ASSERT_EQ(cipher.get()[12], 0x65)
    ASSERT_EQ(cipher.get()[13], 0x25)
    ASSERT_EQ(cipher.get()[29], 0x92)

    ASSERT_EQ(cipher.size(), 32)

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

    ASSERT_EQ(output.size(), 32)

    ASSERT_EQ(output.size() - output.padding(), message.size())
END_DEFINE_TEST()

DEFINE_TEST(writing_cipher_to_file)
    std::string key = "ThisIsMyKey19S";
    std::string message = "And this is a long message. Have a nice day!... Maybe it works. If you read this, then it works.";

    cppblowfish::BlowfishContext blowfish (key);

    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    blowfish.encrypt(input, cipher);

    ASSERT_EQ(cipher.get()[0], 0x25)
    ASSERT_EQ(cipher.get()[1], 0xB7)
    ASSERT_EQ(cipher.get()[2], 0x94)
    ASSERT_EQ(cipher.get()[3], 0x0C)
    ASSERT_EQ(cipher.get()[12], 0x2D)
    ASSERT_EQ(cipher.get()[13], 0xAD)
    ASSERT_EQ(cipher.get()[94], 0x74)
    ASSERT_EQ(cipher.get()[95], 0xD1)

    ASSERT_EQ(cipher.size(), 104)

    {
        std::ofstream file ("cipher.txt", std::ios::binary | std::ios::trunc);
        if (!file.is_open()) { ASSERT(false) }
        file << cipher;
    }

    {
        std::ifstream file ("cipher.txt", std::ios::binary);
        if (!file.is_open()) { ASSERT(false) }
        file.seekg(0, file.end);
        const size_t length = file.tellg();
        file.seekg(0, file.beg);
        char* buff = new char[length];
        file.read(buff, length);
        cipher = cppblowfish::Buffer(buff, length);
        delete[] buff;
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

    ASSERT_EQ(output.size(), 104)

    ASSERT_EQ(output.size() - output.padding(), message.size())
END_DEFINE_TEST()

int main() {
    INITIALIZE_UNIT_TEST()
    TEST(basic_usage)
    TEST(writing_cipher_to_file)
    END_UNIT_TEST()

    std::string key = "mySECRETkey1234";
    std::string message = "Hello, world. Why are you sad?";

    cppblowfish::BlowfishContext blowfish (key);

    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    blowfish.encrypt(input, cipher);

    std::cout << "Cipher: " << cipher << std::endl;
    std::cout << "cipher size: " << cipher.size() << std::endl;

    blowfish.decrypt(cipher, output);

    std::cout << "Output: " << output << std::endl;
    std::cout << "output size: " << output.size() << std::endl;
}
