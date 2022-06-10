#include <string>
#include <iostream>
#include <fstream>

#include <cppblowfish.h>

#include "unit_test.h"

DEFINE_TEST(basic_usage)
    std::string key = "SimonCriesRightNow1234";
    std::string message = "Hello, Jasmine. I miss you.";

    cppblowfish::BlowfishContext blowfish (key);

    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    blowfish.encrypt(input, cipher);

    ASSERT_EQ(cipher.get()[0], 'L')
    ASSERT_EQ(cipher.get()[1], 'N')
    ASSERT_EQ(cipher.get()[2], 242)
    ASSERT_EQ(cipher.get()[3], 31)
    ASSERT_EQ(cipher.get()[4], 130)
    ASSERT_EQ(cipher.get()[5], 116)
    ASSERT_EQ(cipher.get()[12], '?')
    ASSERT_EQ(cipher.get()[13], 'M')
    ASSERT_EQ(cipher.get()[14], 'O')
    ASSERT_EQ(cipher.get()[28], 'T')

    ASSERT_EQ(cipher.size(), 32)

    blowfish.decrypt(cipher, output);

    ASSERT_EQ(output.get()[0], 'H')
    ASSERT_EQ(output.get()[1], 'e')
    ASSERT_EQ(output.get()[2], 'l')
    ASSERT_EQ(output.get()[3], 'l')
    ASSERT_EQ(output.get()[4], 'o')
    ASSERT_EQ(output.get()[5], ',')
    ASSERT_EQ(output.get()[6], ' ')
    ASSERT_EQ(output.get()[12], 'n')
    ASSERT_EQ(output.get()[13], 'e')
    ASSERT_EQ(output.get()[14], '.')
    ASSERT_EQ(output.get()[28], 'J')

    ASSERT_EQ(output.size(), 32)
END_DEFINE_TEST()

DEFINE_TEST(writing_cipher_to_file)
    std::string key = "SimonCriesRightNow1234";
    std::string message = "Hello, Jasmine. I miss you.";

    cppblowfish::BlowfishContext blowfish (key);

    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    blowfish.encrypt(input, cipher);

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
END_DEFINE_TEST()

int main() {
    INITIALIZE_UNIT_TEST()
    TEST(basic_usage)
    TEST(writing_cipher_to_file)
    END_UNIT_TEST()

    std::string key = "SimonCriesRightNow1234";
    std::string message = "Hello, Jasmine. I miss you.";

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
