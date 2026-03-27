/**
 * @file FileIO.hpp
 * @author Daniele Giardino
 * @brief File input/output functions for DSP applications.
 */

#ifndef LIB_RAWDSP_FILE_IO_HPP
#define LIB_RAWDSP_FILE_IO_HPP

#include <iostream>
#include <string>
#include <vector>


namespace rawdsp
{
    // ========================================================================
    // Read and write files
    // ========================================================================

    /**
     * @brief Returns the size of a file in bytes.
     *
     * @param fileName Name of the file.
     * @return uint64_t Size of the file in bytes.
     */
    uint64_t lengthFile(std::string fileName);

    /**
     * @brief Reads a binary file.
     *
     * @param fileName Name of the binary file.
     * @param numSamples Number of samples to read.
     * @param seek Offset position from the beginning of the file.
     */
    template <typename T>
    std::vector<T> readBinFile(std::string fileName, uint64_t numSamples, uint64_t seek);

    /**
     * @brief Writes a binary file.
     *
     * @param fileName Name of the binary file.
     * @param p Pointer to the complex array.
     * @param numSamples Number of samples to write.
     */
    template <typename T>
    void writeBinFile(std::string fileName, const std::vector<T> &p, uint64_t numSamples);

} // namespace rawdsp

#include "../src/FileIO.cpp"

#endif // LIB_RAWDSP_FILE_IO_HPP