# cppblowfish

### A small C++ encryption library implementing the blowfish algorithm

I use this library for myself. If it works for me, then maybe it works for you as well.
But use it at your own risk.

**Basic usage:**

    // Define the key and some arbitrary data 
    std::string key = "mySECRETkey1234";
    std::string message = "Hello, world. Why are you sad?";

    // Create the context using the key
    cppblowfish::BlowfishContext blowfish (key);

    // Define the buffers used
    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    // Encrypt the data in the input buffer and output it in the cipher buffer
    blowfish.encrypt(input, cipher);

    std::cout << "cipher: " << cipher << std::endl;
    std::cout << "cipher size: " << cipher.size() << std::endl;

    // Maybe do some other stuff

    // Decrypt the data in the cipher and output it in the output buffer
    blowfish.decrypt(cipher, output);

    std::cout << "output: " << output << std::endl;
    std::cout << "output size: " << output.size() << std::endl;

    // Get the data
    unsigned char* data = new unsigned char[output.size() - output.padding()];
    memcpy(data, output.get(), output.size() - output.padding());

**Writing cipher to file and reading back:**

    std::string key = "ThisIsMyKey19S";
    std::string message = "And this is a long message. Have a nice day!... Maybe it works. If you read this, then it works.";

    cppblowfish::BlowfishContext blowfish (key);

    cppblowfish::Buffer input (message.c_str(), message.size());
    cppblowfish::Buffer cipher;
    cppblowfish::Buffer output;

    blowfish.encrypt(input, cipher);

    {
        std::ofstream file ("cipher.txt", std::ios::binary | std::ios::trunc);
        if (!file.is_open()) { exit(1); }

        // Write **all** the contents of the buffer into the file
        cipher.write_whole_data(file);

        // You can also write to a buffer created by you
        // unsigned char* buffer = new unsigned char[cipher.size() + cppblowfish::BUFFER_OFFSET];
        // cipher.write_whole_data(buffer);
    }

    // Maybe do other stuff

    cppblowfish::Buffer cipher2;

    {
        std::ifstream file ("cipher.txt", std::ios::binary);
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

    unsigned char* data = new unsigned char[output.size() - output.padding()];
    memcpy(data, output.get(), output.size() - output.padding());
