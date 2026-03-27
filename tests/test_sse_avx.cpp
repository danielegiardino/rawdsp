/**
 * @file test_sse_avx.cpp
 * @author Daniele Giardino
 * @brief Test for SSE and AVX primitives. Rawdsp is not used here to avoid circular dependencies, and the test focuses on measuring the performance of gain operations using different SIMD instruction sets.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>
#include <cstring>

const int SIGNAL_SIZE = 1lu << 20lu; // 1 million samples
const float GAIN = 2.5f;

// Measure gain using normal for loop
void gain_for_loop(const float* input, float* output, int size, float gain) {
    for (int i = 0; i < size; ++i) {
        output[i] = input[i] * gain;
    }
}

// Measure gain using SSE (processes 4 floats at a time)
void gain_sse(const __m128* input, __m128* output, int size, float gain) {
    __m128 gain_vec = _mm_set1_ps(gain);
    int i = 0;
    
    // Process 4 elements at a time
    for (; i <= size - 4; i += 4) {
        __m128 result = _mm_mul_ps(input[i], gain_vec);
        output[i] = result;
    }
    
}

// Measure gain using AVX (processes 8 floats at a time)
void gain_avx(const __m256* input, __m256* output, int size, float gain) {
    __m256 gain_vec = _mm256_set1_ps(gain);
    int i = 0;
    
    // Process 8 elements at a time
    for (; i <= size - 8; i += 8) {
        __m256 result = _mm256_mul_ps(input[i], gain_vec);
        output[i] = result;
    }
    
}

int main() {
    // Allocate aligned memory for SIMD operations
    std::vector<float> input(SIGNAL_SIZE);
    std::vector<__m128> input_sse(SIGNAL_SIZE/4); // Each __m128 holds 4 floats
    std::vector<__m256> input_avx(SIGNAL_SIZE/8); // Each __m256 holds 8 floats
    std::vector<float> output_for(SIGNAL_SIZE);
    std::vector<__m128> output_sse(SIGNAL_SIZE/4); // Each __m128 holds 4 floats
    std::vector<__m256> output_avx(SIGNAL_SIZE/8); // Each __m256 holds 8 floats
    
    // Initialize input signal with random values
    for (int i = 0; i < SIGNAL_SIZE; ++i) {
        input[i] = static_cast<float>(i % 100) * 0.1f;
    }
    for (int i = 0; i < SIGNAL_SIZE/4; ++i) {
        input_sse[i] = _mm_loadu_ps(&input[i*4]);
    }
    for (int i = 0; i < SIGNAL_SIZE/8; ++i) {
        input_avx[i] = _mm256_loadu_ps(&input[i*8]);
    }
    
    // Measure for loop performance
    auto start = std::chrono::high_resolution_clock::now();
    gain_for_loop(input.data(), output_for.data(), SIGNAL_SIZE, GAIN);
    auto end = std::chrono::high_resolution_clock::now();
    auto for_loop_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // Measure SSE performance
    start = std::chrono::high_resolution_clock::now();
    gain_sse(input_sse.data(), output_sse.data(), SIGNAL_SIZE/4, GAIN);
    end = std::chrono::high_resolution_clock::now();
    auto sse_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // Measure AVX performance
    start = std::chrono::high_resolution_clock::now();
    gain_avx(input_avx.data(), output_avx.data(), SIGNAL_SIZE/8, GAIN);
    end = std::chrono::high_resolution_clock::now();
    auto avx_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    // Print results
    std::cout << "Signal size: " << SIGNAL_SIZE << " elements\n";
    std::cout << "Gain factor: " << GAIN << "\n\n";
    std::cout << "For Loop time: " << for_loop_time << " µs\n";
    std::cout << "SSE time:      " << sse_time << " µs (speedup: " << (float)for_loop_time / sse_time << "x)\n";
    std::cout << "AVX time:      " << avx_time << " µs (speedup: " << (float)for_loop_time / avx_time << "x)\n";
    
    return 0;
}
