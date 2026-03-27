/**
 * @file test_xcorr.cpp
 * @author Daniele Giardino
 * @brief Test the cross-correlation functions in rawdsp.
 */

#include "rawdsp.hpp"
#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
    // Number of samples
    uint64_t N = 30;

    std::vector<float> x_real(N);
    std::vector<float> y_real(N);
    std::vector<std::complex<float>> x_complex(N);
    std::vector<std::complex<float>> y_complex(N);

    // Fill vectors with known values
    // x = [1, 2, 3, 4]
    // y = [1, 1, 1, 1] (A simple DC step to see the sliding sum)
    printf("Generated Samples:\n");
    for (size_t i = 0; i < N; i++)
    {
        x_real[i] = static_cast<float>(i + 1);
        y_real[i] = 1.0f;

        x_complex[i] = std::complex<float>(static_cast<float>(i + 1), 0.0f);
        y_complex[i] = std::complex<float>(1.0f, 1.0f);

        printf("i=%02lu: x_real=%+02.1f, y_real=%+02.1f | x_complex=(%+02.1f,%+02.1f)\n",
               i, x_real[i], y_real[i], x_complex[i].real(), x_complex[i].imag());
    }

    // Cross-correlation (Full range: 2*N - 1 = 7)
    size_t numLags = 2 * N - 1;
    std::vector<float> xcorr_real = rawdsp::xcorr(x_real, y_real, numLags);
    std::vector<std::complex<float>> xcorr_complex = rawdsp::xcorr(x_complex, y_complex, numLags);

    // Lag vector for the result
    std::vector<int64_t> lag = rawdsp::xcorr_lag<int64_t>(xcorr_real.size());

    // Print Results
    std::cout << "\nCross-Correlation of Real Signals:" << std::endl;
    for (size_t i = 0; i < xcorr_real.size(); i++)
        printf("lag=%+2li: val=%+02.4f\n", lag[i], xcorr_real[i]);

    std::cout << "\nCross-Correlation of Complex Signals:" << std::endl;
    for (size_t i = 0; i < xcorr_complex.size(); i++)
    {
        printf("lag=%+2li: val=(%+02.4f,%+02.4f), abs=%+02.4f\n",
               lag[i], xcorr_complex[i].real(), xcorr_complex[i].imag(), std::abs(xcorr_complex[i]));
    }

    // For visualization, we can plot the real and complex cross-correlation results using the plotXY function.
    std::vector<float> xcorr_complex_magnitude(xcorr_complex.size());
    for (size_t i = 0; i < xcorr_complex.size(); i++)
        xcorr_complex_magnitude[i] = std::abs(xcorr_complex[i]);

    // Estimate the peak and the corresponding lag for the real and complex cross-correlation results
    size_t max_index_real = 0;
    float max_value_real = xcorr_real[0];
    for (size_t i = 1; i < xcorr_real.size(); i++)
    {
        if (xcorr_real[i] > max_value_real)
        {
            max_value_real = xcorr_real[i];
            max_index_real = i;
        }
    }
    printf("\nPeak of Real Cross-Correlation: lag=%+2li, val=%+02.4f\n", lag[max_index_real], max_value_real);

    size_t max_index_complex = 0;
    float max_value_complex = std::abs(xcorr_complex[0]);
    for (size_t i = 1; i < xcorr_complex.size(); i++)
    {
        if (std::abs(xcorr_complex[i]) > max_value_complex)
        {
            max_value_complex = std::abs(xcorr_complex[i]);
            max_index_complex = i;
        }
    }
    printf("Peak of Complex Cross-Correlation: lag=%+2li, val=(%+02.4f,%+02.4f), abs=%+02.4f\n",
           lag[max_index_complex], xcorr_complex[max_index_complex].real(), xcorr_complex[max_index_complex].imag(), max_value_complex);

    // Print
    std::cout << "\nPlotting Cross-Correlation of Real Signals:" << std::endl;
    rawdsp::plotXY(std::vector<float>(lag.begin(), lag.end()), xcorr_real, 60, 20);

    std::cout << "\nPlotting Cross-Correlation of Complex Signals (Magnitude):" << std::endl;
    rawdsp::plotXY(std::vector<float>(lag.begin(), lag.end()), xcorr_complex_magnitude, 60, 20); // Plotting the magnitude

    return 0;
}