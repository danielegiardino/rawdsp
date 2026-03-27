/**
 * @file test_fir1.cpp
 * @author Daniele Giardino
 * @brief Test the fir1 function in Filters.cpp
 */

#include "rawdsp.hpp"
#include <vector>
#include <cstdint>
#include <string>

void test_conv_float()
{
    // Generate FIR filter coefficients using fir1
    uint64_t ord = (1lu << 6);    // Filter order
    float Fc = 150;              // Cutoff frequency [Hz]
    float Fs = 1000;             // Sampling frequency [Hz]
    std::string window = "hann"; // Window type
    std::vector<float> coeffs = rawdsp::fir1(ord, Fc, Fs, window);

    // Generate two complex waveforms: one at 0.1 normalized frequency and another at 0.3 normalized frequency
    float A = 0.25f;             // Amplitude
    float phi = 0.0f;            // Phase offset
    uint64_t numSps = 1lu << 5; // Number of samples
    std::vector<float> wave1 = rawdsp::wave_cosine(A, 100.0f, Fs, phi, numSps);
    std::vector<float> wave2 = rawdsp::wave_cosine(A, 300.0f, Fs, phi, numSps);

    // Combine the waves
    std::vector<float> combined_wave(numSps);
    for (size_t i = 0; i < numSps; i++)
        combined_wave[i] = wave1[i] + wave2[i];

    // Apply the FIR filter to the combined wave (linear convolution)
    std::vector<float> my_filtered_wave = rawdsp::conv(combined_wave, coeffs, false);
    std::vector<float> filtered_wave;
    {
        size_t N = combined_wave.size();
        size_t M = coeffs.size();
        if (N == 0 || M == 0) {
            filtered_wave.clear();
        } else {
            filtered_wave.assign(N + M - 1, 0.0f);
            for (size_t n = 0; n < N; ++n) {
                for (size_t m = 0; m < M; ++m) {
                    filtered_wave[n + m] += combined_wave[n] * coeffs[m];
                }
            }
        }
    }

    // Remove the latency of the filter (i.e., remove the first (M-1)/2 samples)
    size_t latency = (coeffs.size() - 1) / 2;
    std::vector<float> my_filtered_wave_noLatency = rawdsp::conv(combined_wave, coeffs, true);
    std::vector<float> filtered_wave_noLatency(filtered_wave.begin() + latency, filtered_wave.end() - latency);


    // Write to file combined_wave and filtered_wave as binary files
    rawdsp::writeBinFile("coeffs.dat", coeffs, coeffs.size());
    rawdsp::writeBinFile("combined_wave.dat", combined_wave, combined_wave.size());
    rawdsp::writeBinFile("filtered_wave.dat", filtered_wave, filtered_wave.size());
    rawdsp::writeBinFile("filtered_wave_noLatency.dat", filtered_wave_noLatency, filtered_wave_noLatency.size());

}

void test_conv_c_float_t()
{
    // Generate FIR filter coefficients using fir1
    uint64_t ord = (1lu << 4);    // Filter order
    float Fc = 150;              // Cutoff frequency [Hz]
    float Fs = 1000;             // Sampling frequency [Hz]
    std::string window = "hann"; // Window type
    std::vector<float> coeffs = rawdsp::fir1(ord, Fc, Fs, window);

    // Generate two complex waveforms: one at 0.1 normalized frequency and another at 0.3 normalized frequency
    float A = 0.25f;             // Amplitude
    float phi = 0.0f;            // Phase offset
    uint64_t numSps = 1lu << 5; // Number of samples
    std::vector<std::complex<float>> wave1 = rawdsp::complex_wave(A, 100.0f, Fs, phi, numSps);
    std::vector<std::complex<float>> wave2 = rawdsp::complex_wave(A, 300.0f, Fs, phi, numSps);

    // Combine the waves
    std::vector<std::complex<float>> combined_wave(numSps);
    for (size_t i = 0; i < numSps; i++)
        combined_wave[i] = wave1[i] + wave2[i];

    // Apply the FIR filter to the combined wave (linear convolution)
    std::vector<std::complex<float>> filtered_wave = rawdsp::conv(combined_wave, coeffs, true);
    
    // Write to file combined_wave and filtered_wave as binary files
    rawdsp::writeBinFile("coeffs.dat", coeffs, coeffs.size());
    rawdsp::writeBinFile("combined_wave.dat", combined_wave, combined_wave.size());
    rawdsp::writeBinFile("filtered_wave.dat", filtered_wave, filtered_wave.size());

}

int main(int argc, char const *argv[])
{
    // De-comment the function to test    
    // test_conv_float();   // Test conv for float
    test_conv_c_float_t();// Test conv for c_float_t

    return 0;
}
