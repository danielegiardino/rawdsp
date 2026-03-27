/**
 * @file test_conv.cpp
 * @author Daniele Giardino
 * @brief Test the convolution function in Primitive.cpp
 */

#include "rawdsp.hpp"

int main(int argc, char const *argv[])
{
    // Test convolution with a simple impulse response
    std::vector<float> impulse = {1.0f, 1.0f, 0.0f, 0.0f}; // Impulse signal
    std::vector<float> coefficients = {0.25f, 0.25f, 0.25f, 0.25f}; // Simple moving average filter

    std::vector<float> output_noLatency = rawdsp::conv(impulse, coefficients);
    std::vector<float> output_withLatency = rawdsp::conv(impulse, coefficients, true);
    std::cout << "Convolution Output for Impulse Input:" << std::endl;
    
    printf("Output without latency removal:\n");
    for (size_t i = 0; i < output_noLatency.size(); i++)
        printf("i=%lu: output=%.4f\n", i, output_noLatency[i]);

    printf("\nOutput with latency removal:\n");
    for (size_t i = 0; i < output_withLatency.size(); i++)
        printf("i=%lu: output=%.4f\n", i, output_withLatency[i]);

    return 0;
}