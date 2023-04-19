#include "XorEncryptFile.hpp"

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
int EncryptXOR(const std::filesystem::path &strInputFilename, const std::filesystem::path &strOutputFilename, const std::string &password)
{
    int nRet = 0;

    std::ifstream inputStream(strInputFilename, std::ios::binary);
    std::ofstream outputStream(strOutputFilename, std::ios::binary);

    if (!inputStream || !outputStream)
    {
        return -1;
    }

    // get length of file:
    inputStream.seekg(0, inputStream.end);
    std::size_t nFileSize = inputStream.tellg();
    inputStream.seekg(0, inputStream.beg);

    nRet = EncryptXOR(inputStream, nFileSize, outputStream, password);

    inputStream.close();
    outputStream.close();

    return nRet;
}

/*!
 * encrypt|decrypt files with XOR operation with a password.
 * \param inputStream input stream
 * \param nInputSize input stream. because not all streams are seekable (For example, imagine an istream on a network socket)
 *  - The return type from tellg() is not necessarily numeric (the only requirement is that it can be passed back to seekg() to return to the same position)
 *  - Even if it is numeric, it is not necessarily a number of bytes. For example, it could be a "magic" value meaning "at the end"
 *  - For fstreams, issues like case and linefeed conversion can screw things up
 *  - seekg(0, ios:end) might not work for large files http://stackoverflow.com/questions/32057750/how-to-get-the-filesize-for-large-files-in-c
 * \param outputStream output stream
 * \param password xor key
 * \return The calculated kg based on the parameter.
 * \warning Input file and output file should be dfferent.
 */
int EncryptXOR(std::istream &inputStream, std::size_t nInputSize, std::ostream &outputStream, const std::string &password)
{
    static const std::size_t nBlockSize = 64;

    char bufferRead[nBlockSize] = {};

    std::size_t nBytesCrt = 0;

    do
    {
        inputStream.read(bufferRead, nBlockSize);
        std::size_t nBytesRead = inputStream.gcount();

        for (int i = 0; i < nBytesRead; ++i)
        {
            bufferRead[i] ^= password[i % password.length()];
        }

        outputStream.write(bufferRead, nBytesRead);
        std::ios_base::iostate write_state = outputStream.rdstate();
        if (write_state)
        {
            throw std::runtime_error("Error writing to file");
        }

        nBytesCrt += nBytesRead;

        if (0 != nInputSize)
        {
            static std::size_t progress = -1;
            std::size_t crt_progress = nBytesCrt * 100 / nInputSize;
            if (crt_progress != progress)
            {
                Helper::PrintProgress(float(crt_progress) / 100);
                progress = crt_progress;
            }
        }

    } while (inputStream);

    if (0 != nInputSize)
    {
        std::cout << std::endl;
    }

    return 0;
}

namespace Helper
{
    void PrintProgress(float fProgress)
    {
        static const std::size_t barWidth = 80;

        std::cout << "[";
        std::size_t pos = barWidth * fProgress;
        for (std::size_t i = 0; i < barWidth; ++i)
        {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }
        std::cout << "] " << int(fProgress * 100) << " %\r";
        std::cout.flush();
    }

} // namespace Helper