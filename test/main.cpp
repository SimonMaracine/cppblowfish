#include <string>
#include <iostream>

#include <cppblowfish.h>

int main() {
    std::string key = "SimonCriesRightNow1234";
    std::string message = "Hello, Jasmine. I miss you.";

    cppblowfish::BlowfishContext blowfish (key);

    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;

    blowfish.encrypt(input, cipher);

    std::cout << cipher << std::endl;
}
