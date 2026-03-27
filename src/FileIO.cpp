/**
 * @file FileIO.cpp
 * @author Daniele Giardino
 * @brief Implementation of file input/output functions for rawdsp.
 */
#include "FileIO.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include <cstring>
#include <cstdio>

// ========================================================================
// Read and write files (POSIX implementation)
// ========================================================================

namespace rawdsp
{

    uint64_t lengthFile(std::string fileName)
    {
        int32_t fd = open(fileName.c_str(), O_RDONLY);
        if (fd == -1)
        {
            fprintf(stderr, "Unable to open '%s': %s\n", fileName.c_str(), strerror(errno));
            exit(1);
        }

        off_t off = lseek(fd, 0, SEEK_END);
        if (off == (off_t)-1)
        {
            fprintf(stderr, "lseek failed for '%s': %s\n", fileName.c_str(), strerror(errno));
            close(fd);
            exit(1);
        }

        close(fd);
        return static_cast<uint64_t>(off);
    }

    template <typename T>
    
    std::vector<T> readBinFile(std::string fileName, uint64_t numSamples, uint64_t seek)
    {
        int32_t fd = open(fileName.c_str(), O_RDONLY);
        if (fd == -1)
        {
            fprintf(stderr, "Unable to open '%s': %s\n", fileName.c_str(), strerror(errno));
            exit(1);
        }

        uint64_t lenFileInByte = lengthFile(fileName);
        uint64_t bytePerSample = sizeof(T);
        uint64_t startByte = seek * bytePerSample;

        if (lenFileInByte < startByte + (bytePerSample * numSamples))
        {
            fprintf(stderr, "The number of samples to read is greater than the number of samples of the signal.\n");
            close(fd);
            exit(1);
        }

        std::vector<T> samples(numSamples);
        uint64_t bytesToRead = numSamples * bytePerSample;
        char *buf = reinterpret_cast<char *>(samples.data());
        uint64_t totalRead = 0;

        while (totalRead < bytesToRead)
        {
            ssize_t r = pread(fd, buf + totalRead, static_cast<size_t>(bytesToRead - totalRead), static_cast<off_t>(startByte + totalRead));
            if (r == -1)
            {
                if (errno == EINTR)
                    continue;
                fprintf(stderr, "Reading error '%s': %s\n", fileName.c_str(), strerror(errno));
                close(fd);
                exit(3);
            }
            if (r == 0)
                break; // EOF
            totalRead += static_cast<uint64_t>(r);
        }

        if (totalRead != bytesToRead)
        {
            fprintf(stderr, "Reading error: expected %lu bytes, got %lu\n", bytesToRead, totalRead);
            close(fd);
            exit(3);
        }

        close(fd);
        return samples;
    }

    template <typename T>
    void writeBinFile(std::string fileName, const std::vector<T> &p, uint64_t numSamples)
    {
        int32_t fd = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1)
        {
            fprintf(stderr, "Unable to open '%s' for writing: %s\n", fileName.c_str(), strerror(errno));
            exit(1);
        }

        uint64_t bytesToWrite = numSamples * sizeof(T);
        const char *buf = reinterpret_cast<const char *>(p.data());
        uint64_t totalWritten = 0;

        while (totalWritten < bytesToWrite)
        {
            ssize_t w = write(fd, buf + totalWritten, static_cast<size_t>(bytesToWrite - totalWritten));
            if (w == -1)
            {
                if (errno == EINTR)
                    continue;
                fprintf(stderr, "Writing error '%s': %s\n", fileName.c_str(), strerror(errno));
                close(fd);
                exit(3);
            }
            totalWritten += static_cast<uint64_t>(w);
        }

        close(fd);
    }

} // namespace rawdsp
