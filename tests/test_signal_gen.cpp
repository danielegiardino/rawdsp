/**
 * @file test_signal_gen.cpp
 * @author Daniele Giardino
 * @brief Test for signal generation functions, including complex wave and MPSK symbol generation, and writing results to binary files.
 */

#include "rawdsp.hpp"

int main(int argc, char const *argv[])
{
    // Test complex_wave function
    float A = 1.0f;          // Amplitude
    float fc = 1000.0f;      // Frequency of the CW in Hz
    float Fs = 10000.0f;     // Sampling frequency in Hz
    float phi = 0.0f;        // Phase offset in radians
    uint64_t numSps = 10000; // Number of samples

    std::vector<c_float_t> cw = rawdsp::complex_wave(A, fc, Fs, phi, numSps);
    std::cout << "Complex CW Samples:" << std::endl;

    rawdsp::writeBinFile("cw_samples.dat", cw, cw.size());

    std::vector<c_float_t> cw_read = rawdsp::readBinFile<c_float_t>("cw_samples.dat", numSps, 0);
    std::cout << "\nRead Complex CW Samples from file:" << std::endl;
    for (size_t i = 0; i < 10; i++)
        printf("i=%lu: cw=(%.4f,%.4f) | cw_read=(%.4f,%.4f)\n", i, cw[i].real(), cw[i].imag(), cw_read[i].real(), cw_read[i].imag());

    // Test mpsk_symbols function
    uint64_t M = 8;  // Modulation order (e.g., 8-PSK)
    uint64_t N = 10; // Number of symbols to generate

    std::vector<std::complex<float>> symbols = rawdsp::mpsk_symbols<float>(M, N);
    std::cout << "\nMPSK Symbols:" << std::endl;
    for (const auto &symbol : symbols)
        printf("i=%lu: symbol=(%.4f,%.4f)\n", &symbol - &symbols[0], symbol.real(), symbol.imag());
}
