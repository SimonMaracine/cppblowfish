# cppblowfish

## A small C++ encryption library implementing the blowfish algorithm

I built this library for myself. **Note that it's not meant to be used in production environments where
security is critical.** It's more of a toy for hobbyists like me to play with. I tested it on
`GCC 13.2` and `MSVC 19.39`. It requires at least `C++17`.

Check the header files for _some_ documentation. And check out `tests/main.cpp` for a working
example.

## Usage

To use this library, just include this repository as a submodule like this:

`git submodule add -b stable -- https://github.com/SimonMaracine/cppblowfish.git <path/to/submodule>`

Then write this in CMakeLists.txt:

```cmake
add_subdirectory(<path/to/submodule>)
target_link_libraries(<your_target> PRIVATE cppblowfish)
```

To build with tests, include this before `add_subdirectory(...)`:

```cmake
set(CPPBLOWFISH_BUILD_TESTS ON)
```

The CMake script builds the library by default as static. If you want it dynamic:

```cmake
set(BUILD_SHARED_LIBS ON)
```

## Basic example

```c++
// Define the key and some arbitrary data
std::string key {"mySECRETkey1234"};
std::string message {"Hello, world. Why are you sad?"};

// Create the context using the key
const cppblowfish::BlowfishContext blowfish {key};

// Define the buffers used
cppblowfish::Buffer input {message.c_str(), message.size()};
cppblowfish::Buffer cipher;
cppblowfish::Buffer output;

// Encrypt the data in the input buffer and output it in the cipher buffer
blowfish.encrypt(input, cipher);

std::cout << "cipher: " << cipher << '\n';
std::cout << "cipher size: " << cipher.size() << '\n';

// Maybe do some other stuff...

// Decrypt the data in the cipher buffer and output it in the output buffer
blowfish.decrypt(cipher, output);

std::cout << "output: " << output << '\n';
std::cout << "output size: " << output.size() << '\n';

// Copy the data and do whatever you want with it
unsigned char* data {new unsigned char[output.size()]};
std::memcpy(data, output.get(), output.size());

// Or take ownership of the data
#if 0
unsigned char* data {output.steal()};
#endif
```

## Writing cipher to file and reading back

```c++
std::string key {"ThisIsMyKey19S"};
std::string message {"And this is a long message. Have a nice day!... Maybe it works. If you read this, then it works."};

const cppblowfish::BlowfishContext blowfish {key};

cppblowfish::Buffer input {message.c_str(), message.size()};
cppblowfish::Buffer cipher;
cppblowfish::Buffer output;

blowfish.encrypt(input, cipher);

// Write cipher to file
{
    std::ofstream stream {"cipher.txt", std::ios::binary};
    if (!stream.is_open()) { std::exit(1); }

    // Write **all** the contents of the buffer into the file
    cipher.write_whole_data(stream);

    // Or write to a buffer created by you
#if 0
    unsigned char* buffer {
        new unsigned char[cipher.size() + cipher.padding() + cppblowfish::BUFFER_OFFSET]
    };
    cipher.write_whole_data(buffer);
#endif
}

// Maybe do other stuff...

cppblowfish::Buffer cipher2;

// Read cipher back from file
{
    std::ifstream stream {"cipher.txt", std::ios::binary};
    if (!stream.is_open()) { std::exit(1); }

    stream.seekg(0, stream.end);
    const auto length {stream.tellg()};
    stream.seekg(0, stream.beg);

    char* raw_data {new char[length]};
    stream.read(raw_data, length);

    // Create a new buffer from **all** the contents of a previous buffer
    cipher2 = cppblowfish::Buffer::read_whole_data(raw_data, length);

    delete[] raw_data;
}

blowfish.decrypt(cipher2, output);

// Copy the data and do whatever you want with it
unsigned char* data {new unsigned char[output.size()]};
std::memcpy(data, output.get(), output.size());

// Or take ownership of the data
#if 0
unsigned char* data {output.steal()};
#endif
```
