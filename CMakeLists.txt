
cmake_minimum_required(VERSION 3.12)

project(XorEncryptFile)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ######################################################################################################
# EXECUTABLE ###########################################################################################
# ######################################################################################################

SET(XOR_SRC)
SET(XOR_INC)

file(GLOB XOR_SRC
    "${PROJECT_SOURCE_DIR}/main.cpp" 
    "${PROJECT_SOURCE_DIR}/XorEncryptFile.cpp")

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/sources
    ${CMAKE_SOURCE_DIR}/include
)

ADD_EXECUTABLE(XorEncryptFile ${XOR_SRC})

# ######################################################################################################
# INSTALL ##############################################################################################
# ######################################################################################################

INSTALL(TARGETS XorEncryptFile DESTINATION ${CMAKE_INSTALL_BINDIR})

# ######################################################################################################
# TESTS ################################################################################################
# ######################################################################################################
include(CTest) 
enable_testing()

# externall tests
add_test(NAME TestInexistingFile COMMAND XorEncryptFile inexisting_file.in inexisting_file.xor password)

# internal tests
add_subdirectory(tests)