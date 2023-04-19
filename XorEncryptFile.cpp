#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

/*!
 * encrypt|decrypt files with XOR operation with a password.
 * \param strInputFilename filepath of input file
 * \param strInputFilename filepath of output file
 * \param password xor key
 * \return The calculated kg based on the parameter.
 * \warning Input file and output file should be dfferent.
 */
int EncryptXOR(const std::filesystem::path &strInputFilename, const std::filesystem::path &outputFilename, const std::string &password)
{
    int nRet = -1;
    FILE *inputFile, *outputFile;

    do
    {
        std::size_t nBlockSize = password.length();
        unsigned char bufferRead[nBlockSize];

        // files
        inputFile = fopen(strInputFilename.c_str(), "rb");
        outputFile = fopen(outputFilename.c_str(), "wb");

        if (nullptr == inputFile)
        {
            std::cerr << "Input file (" << strInputFilename << ") open error" << std::endl;
            break;
        }

        if (nullptr == outputFile)
        {
            std::cerr << "Output file (" << outputFilename << ") open error" << std::endl;
            break;
        }

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
                std::cout << '\r' << "progress: [" << crt_progress << "%] : " << strInputFilename << " ... " << std::flush;
                progress = crt_progress;
            }

        } while (nBytesRead == nBlockSize);

        nRet = 0;
        std::cout << "Done" << std::endl;

    } while (0);

    // clean up
    if(inputFile)
        fclose(inputFile);

    if(outputFile)
    fclose(outputFile);

    return nRet;
}
