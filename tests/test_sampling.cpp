/**
 * @file test_sampling.cpp
 * @author Daniele Giardino
 * @brief Test program for upsampling and downsampling functions
 */

#include "rawdsp.hpp"
#include <iostream>
#include <vector>
#include <iomanip>

// Helper to print vectors easily
template <typename T>
void print_vector(const std::string& label, const std::vector<T>& vec) {
    std::cout << std::left << std::setw(15) << label << ": [ ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i == vec.size() - 1 ? "" : ", ");
    }
    std::cout << " ] (size: " << vec.size() << ")" << std::endl;
}

int main() {
    // 1. Setup Input Data: [1, 2, 3, 4]
    std::vector<float> data_real = {1.0f, 2.0f, 3.0f, 4.0f};
    
    // Complex data: [(1,0), (2,0), (3,0), (4,0)]
    std::vector<std::complex<float>> data_complex = {
        std::complex<float>(1.0f, 0.0f), 
        std::complex<float>(2.0f, 0.0f), 
        std::complex<float>(3.0f, 0.0f), 
        std::complex<float>(4.0f, 0.0f)
    };

    std::cout << "--- Testing Real Sampling ---" << std::endl;
    print_vector("Original", data_real);

    // 2. Test Upsampling (L=3)
    // Expected: [1, 0, 0, 2, 0, 0, 3, 0, 0, 4, 0, 0]
    uint64_t L = 3;
    std::vector<float> up = rawdsp::upsample(data_real, L);
    print_vector("Upsampled (L=3)", up);

    // 3. Test Downsampling (M=2)
    // From the upsampled result: takes indices 0, 2, 4, 6, 8, 10
    // Expected: [1, 0, 0, 3, 0, 0]
    uint64_t M = 2;
    std::vector<float> down = rawdsp::downsample(data_real, M);
    print_vector("Downsampled (M=2)", down);

    std::cout << "\n--- Testing Complex Sampling ---" << std::endl;
    
    // Test complex upsampling
    auto up_c = rawdsp::upsample(data_complex, 2);
    std::cout << "Upsampled (L=2) first 4 samples:" << std::endl;
    for(size_t i = 0; i < 4; ++i) {
        printf("i=%02zu: (%+01.1f, %+01.1f)\n", i, up_c[i].real(), up_c[i].imag());
    }

    return 0;
}