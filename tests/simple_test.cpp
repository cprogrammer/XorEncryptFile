#include "../XorEncryptFile.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <stdio.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <tuple>

namespace Helper
{

    std::string ReadFile(std::filesystem::path filePath)
    {
        std::ifstream t(filePath);
        return std::string((std::istreambuf_iterator<char>(t)),
                           std::istreambuf_iterator<char>());
    }

    std::filesystem::path CreateTempFile(std::string strFileContent = "")
    {
        std::string templatePath = std::filesystem::temp_directory_path() / "xor-file-XXXXXX";
        int fd = mkstemp(templatePath.data()); // Creates and opens a new temp file r/w.
                                               // Xs are replaced with a unique number.
        if (!strFileContent.empty() && fd != -1)
        {
            size_t nWriteCount = write(fd, strFileContent.c_str(), strFileContent.length());
            if (nWriteCount != strFileContent.length())
            {
                throw std::runtime_error("Error writing to temp file");
            }
            
        }
        close(fd);

        return templatePath;
    }

} // namespace Helper

class TestOnRandomFile : public ::testing::TestWithParam<std::string>
{
protected:

    std::filesystem::path mTempFileInput;
    std::filesystem::path mTempFileCheck;
    std::filesystem::path mTempFileXor;

    TestOnRandomFile()
    {
    }

    ~TestOnRandomFile()
    {
    }

    virtual void SetUp()
    {
        mTempFileInput = Helper::CreateTempFile();
        mTempFileXor = Helper::CreateTempFile();
        mTempFileCheck = Helper::CreateTempFile();
    }

    virtual void TearDown()
    {
        std::filesystem::remove(mTempFileInput);
        mTempFileInput.clear();

        std::filesystem::remove(mTempFileXor);
        mTempFileXor.clear();

        std::filesystem::remove(mTempFileCheck);
        mTempFileCheck.clear();
    }
};

INSTANTIATE_TEST_SUITE_P(
    TEST_SUITE,
    TestOnRandomFile,
    ::testing::Values(
        "Short",
        "password",
        "Test string that longer than input buffer but not extremly long.",
        "Cum nu avea nimic altceva de făcut,"
        "Alice a început iar să vorbească de una singură: Cred că"
        "Dinah o să-mi simtă grozav lipsa astă-seară. (Dinah era"
        "pisica ei.) Sper că o să-şi amintească să-i dea farfurioara"
        "cu lapte la ora ceaiului. Dinah, scumpa mea! Ce n-aş da"
        "să fii şi tu aici, jos, împreună cu mine! Mă tem că nu prea"
        "sunt şoareci prin preajmă, dar ai putea să prinzi un liliac,"
        "ştii, liliecii seamănă mult cu şoarecii. Dar pisicile mănâncă"
        "oare lilieci?“ Atunci, Alice a simţit că o prinde somnul,"
        "dar a continuat să se întrebe, aproape ca în vis: „Pisicile"
        "mănâncă lilieci? Pisicile mănâncă lilieci? şi une ori: Liliecii "
        "mănâncă pisici?“, pentru că, vedeţi dum nea voastră,"
        "cum nu putea răspunde la niciuna dintre cele două întrebări,"
        "nu avea mare importanţă felul în care le formula."
        "Simţea că moţăie şi tocmai începuse să viseze că se plimba"
        "mână în mână cu Dinah şi că o întreba foarte serios:"
        "Dinah, spune-mi sincer, tu ai mâncat vreodată un liliac?,"
        "când, buf!, s-a pomenit pe un morman de crengi şi de"
        "frunze uscate, şi lunga cădere a luat sfârşit."));

TEST_P(TestOnRandomFile, TestSimple1)
{
    // get args
    std::string strContent = GetParam();

    // write to input file
    std:: ofstream myfile;
    myfile.open(mTempFileInput);
    myfile << strContent;
    myfile.close();

    // encrypt
    int nRet = EncryptXOR(mTempFileInput, mTempFileXor, "password");
    ASSERT_EQ(nRet, 0);

    // decrypt
    nRet = EncryptXOR(mTempFileXor, mTempFileCheck, "password");
    ASSERT_EQ(nRet, 0);

    // check double xor is nop
    std::string strCheck = Helper::ReadFile(mTempFileCheck);
    ASSERT_EQ(strContent, strCheck);
}