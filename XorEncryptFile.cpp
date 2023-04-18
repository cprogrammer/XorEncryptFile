#include <fstream>
#include <iostream>
#include <string>

/*	encrypt|decrypt files with XOR operation
    with a password
*/
void EncryptXOR(const char *inputFilename, const char *outputFilename, const char *password)
{
    FILE *inputFile, *outputFile;
    std::size_t nBlockSize = strlen(password);
    unsigned char bufferRead[nBlockSize];

    // files
    inputFile = fopen(inputFilename, "rb");
    outputFile = fopen(outputFilename, "wb");

    fseek(inputFile, 0L, SEEK_END);
    std::size_t fileSizeBytes = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);

    std::size_t nBytesCrt = 0;

    std::size_t progress = -1;

    std::size_t nBytesRead;
    do
    {
        nBytesRead = fread(bufferRead, 1, nBlockSize, inputFile);

        // XOR byte by byte
        for (int i = 0; i < nBytesRead; i++)
            bufferRead[i] ^= password[i];

        fwrite(bufferRead, 1, nBytesRead, outputFile);

        nBytesCrt += nBytesRead;

        std::size_t crt_progress = nBytesCrt * 100 / fileSizeBytes;
        if (crt_progress != progress)
        {
            std::cout << '\r' << "progress: [" << crt_progress << "%] : " << inputFilename << " ... " << std::flush;
            progress = crt_progress;
        }

    } while (nBytesRead == nBlockSize);

    std::cout << "Done" << std::endl;

    // clean up
    fclose(inputFile);
    fclose(outputFile);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "usage: xor <source_file> <destination_file> <keyphrase>\n";
        return 1;
    }

    EncryptXOR(argv[1], argv[2], argv[3]);

    return 0;
}