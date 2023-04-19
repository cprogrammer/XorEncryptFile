#ifndef F5B4D02C_41E6_443F_B3B6_1658CB351F42
#define F5B4D02C_41E6_443F_B3B6_1658CB351F42

#include <filesystem>
#include <string>
#include <istream>
#include <ostream>


int EncryptXOR(const std::filesystem::path &strInputFilename, const std::filesystem::path &strOutputFilename, const std::string& password);

int EncryptXOR(std::istream& inputStream, std::size_t nInputSize, std::ostream& outputStream, const std::string& password);

namespace Helper
{
    void PrintProgress(float fProgress);

} //namespace Helper

#endif /* F5B4D02C_41E6_443F_B3B6_1658CB351F42 */
