/**
 * @file test_autocorr.cpp
 * @author Daniele Giardino
 * @brief Test the auto-correlation function in Primitive.cpp
 */

#include "rawdsp.hpp"

int main(int argc, char const *argv[])
{
    // Number of samples
    uint64_t N = 10; // samples for testing

    // Test Auto Correlation with a simple impulse response
    std::vector<float> noise_real(N);
    std::vector<std::complex<float>> noise_complex(N);

    // Fill the noise vectors with known values for testing (optional, can be random)
    printf("Generated Noise Samples:\n");
    for (size_t i = 0; i < N; i++)
    {
        noise_real[i] = static_cast<float>(i + 1);                                                        // 1.0, 2.0, 3.0, 4.0
        noise_complex[i] = std::complex<float>(static_cast<float>(i + 1), static_cast<float>(2 * i + 1)); // (1.0, 2.0), (2.0, 5.0), (3.0, 7.0), (4.0, 9.0)
        printf("i=%02lu: noise_real=%+02.4f, noise_complex=(%+02.4f,%+02.4f)\n", i, noise_real[i], noise_complex[i].real(), noise_complex[i].imag());
    }

    // Auto correlation
    size_t numLags = 0;                                                                          // Number of lags to compute
    std::vector<float> autocorr_real = rawdsp::autocorr(noise_real, numLags);                     // Auto-correlate real noise with itself
    std::vector<std::complex<float>> autocorr_complex = rawdsp::autocorr(noise_complex, numLags); // Auto-correlate complex noise with itself

    // Create the lag
    std::vector<int64_t> lag = rawdsp::xcorr_lag<int64_t>(N); // Lag vector for real noise

    // Print results
    size_t samplesToPrint = N; // Print all lags (or adjust as needed)
    std::cout << "\nAuto Correlation of Real Noise (first 10 samples):" << std::endl;
    for (size_t i = 0; i < samplesToPrint; i++)
        printf("i=%02lu: autocorr_real=%+02.4f\n", i, autocorr_real[i]);

    std::cout << "\nAuto Correlation of Complex Noise (first 10 samples):" << std::endl;
    for (size_t i = 0; i < samplesToPrint; i++)
        printf("i=%02lu: autocorr_complex=(%+02.4f,%+02.4f), abs=%+02.4f\n", i, autocorr_complex[i].real(), autocorr_complex[i].imag(), std::abs(autocorr_complex[i]));

    std::cout << "\nLag vector (first 10 samples):" << std::endl;
    for (size_t i = 0; i < samplesToPrint; i++)
        printf("i=%02lu: lag=%li\n", i, lag[i]);

    //

    return 0;
}