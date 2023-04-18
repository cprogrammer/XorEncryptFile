
cmake_minimum_required(VERSION 3.12)

project(XorEncryptFile)

set(CMAKE_CXX_STANDARD 17)


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#include(CheckSymbolExists)
#if (WIN32)
#  check_symbol_exists(open io.h HAVE_OPEN)
#else ()
#  check_symbol_exists(open fcntl.h HAVE_OPEN)
#endif ()

# ######################################################################################################
# LIBRARY ##############################################################################################
# ######################################################################################################

SET(XOR_SRC)
SET(XOR_INC)

file(GLOB XOR_SRC "${PROJECT_SOURCE_DIR}/XorEncryptFile.cpp")

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/sources
    ${CMAKE_SOURCE_DIR}/include
)

ADD_EXECUTABLE(XorEncryptFile ${XOR_SRC})

# ######################################################################################################
# INSTALL ##############################################################################################
# ######################################################################################################

INSTALL(TARGETS XorEncryptFile DESTINATION ${CMAKE_INSTALL_BINDIR})

