#include "XorEncryptFile.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Usage: XorEncryptFile <source_file> <destination_file> <keyphrase>\n";
        return 1;
    }

    EncryptXOR(argv[1], argv[2], argv[3]);

    return 0;
}