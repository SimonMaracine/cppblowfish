# cppblowfish

## A small C++ encryption library implementing the blowfish algorithm

I use this library for myself. If it works for me, then maybe it works for you as well.

Check the header files for _some_ documentation.

To use this library easily, just include this repository as a submodule (or better fork the repository beforehand and use the forked version):

`git submodule add https://github.com/SimonMaracine/cppblowfish.git <path/to/submodule/folder>`

Then write this in CMakeLists.txt:

```cmake
add_subdirectory(<path/to/submodule/folder>)
target_link_libraries(<your_target> PRIVATE cppblowfish)
```

The CMake script builds the library as static.

To build with tests, include this before `add_subdirectory(...)`:

```cmake
set(CPPBLOWFISH_BUILD_TESTS ON)
```

If you have any suggestions for improvements, feel free to open up an issue.

## Basic usage

```c++
// Define the key and some arbitrary data
std::string key = "mySECRETkey1234";
std::string message = "Hello, world. Why are you sad?";

// Create the context using the key
cppblowfish::BlowfishContext blowfish {key};

// Define the buffers used
cppblowfish::Buffer input {message.c_str(), message.size()};
cppblowfish::Buffer cipher;
cppblowfish::Buffer output;

// Encrypt the data in the input buffer and output it in the cipher buffer
blowfish.encrypt(input, cipher);

std::cout << "cipher: " << cipher << std::endl;
std::cout << "cipher size: " << cipher.size() << std::endl;

// Maybe do some other stuff...

// Decrypt the data in the cipher buffer and output it in the output buffer
blowfish.decrypt(cipher, output);

std::cout << "output: " << output << std::endl;
std::cout << "output size: " << output.size() << std::endl;

// Copy the data and do whatever you want with it
unsigned char* data = new unsigned char[output.size()];
memcpy(data, output.get(), output.size());

// Or steal the data
// unsigned char* data = output.steal();
```

## Writing cipher to file and reading back

```c++
std::string key = "ThisIsMyKey19S";
std::string message = "And this is a long message. Have a nice day!... Maybe it works. If you read this, then it works.";

cppblowfish::BlowfishContext blowfish {key};

cppblowfish::Buffer input {message.c_str(), message.size()};
cppblowfish::Buffer cipher;
cppblowfish::Buffer output;

blowfish.encrypt(input, cipher);

// Write cipher to file
{
    std::ofstream file {"cipher.txt", std::ios::binary | std::ios::trunc};
    if (!file.is_open()) { exit(1); }

    // Write **all** the contents of the buffer into the file
    cipher.write_whole_data(file);

    // Or write to a buffer created by you
    // unsigned char* buffer = (
    //     new unsigned char[cipher.size() + cipher.padding() + cppblowfish::BUFFER_OFFSET]
    // );
    // cipher.write_whole_data(buffer);
}

// Maybe do other stuff...

cppblowfish::Buffer cipher2;

// Read cipher back from file
{
    std::ifstream file {"cipher.txt", std::ios::binary};
    if (!file.is_open()) { exit(1); }

    file.seekg(0, file.end);
    const size_t length = file.tellg();
    file.seekg(0, file.beg);

    char* buff = new char[length];
    file.read(buff, length);

    // Create a new buffer from **all** the contents of a previous buffer
    cipher2 = cppblowfish::Buffer::from_whole_data(buff, length);

    delete[] buff;
}

blowfish.decrypt(cipher2, output);

// Copy the data and do whatever you want with it
unsigned char* data = new unsigned char[output.size()];
memcpy(data, output.get(), output.size());

// Or steal the data
// unsigned char* data = output.steal();
```
