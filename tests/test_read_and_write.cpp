/**
 * @file test_read_and_write.cpp
 * @author Daniele Giardino
 * @brief Test for reading and writing binary files.
 */

 #include "rawdsp.hpp"

int main(int argc, char const *argv[])
{
    // Generate random noise
    uint64_t N = 10;
    std::vector<float> noise_real = rawdsp::randn<float>(N);
    std::vector<std::complex<float>> noise_complex = rawdsp::randn_complex<float>(N);

    // Write the noise to binary files
    rawdsp::writeBinFile("noise_real.dat", noise_real, N);
    rawdsp::writeBinFile("noise_complex.dat", noise_complex, N);

    // Read the length of the binary files
    uint64_t length_real = rawdsp::lengthFile("noise_real.dat");
    uint64_t length_complex = rawdsp::lengthFile("noise_complex.dat");
    printf("Length of noise_real.dat: %lu bytes\n", length_real);
    printf("Length of noise_complex.dat: %lu bytes\n", length_complex);

    // Read the noise back from the binary files
    std::vector<float> read_noise_real = rawdsp::readBinFile<float>("noise_real.dat", N, 0);
    std::vector<std::complex<float>> read_noise_complex = rawdsp::readBinFile<std::complex<float>>("noise_complex.dat", N, 0);  

    // Compare the original and read noise
    for (size_t i = 0; i < N; i++)
    {
        if (noise_real[i] != read_noise_real[i])
        {
            fprintf(stderr, "Mismatch in real noise at index %lu: original %f, read %f\n", i, noise_real[i], read_noise_real[i]);
            return 1;
        }
        if (noise_complex[i].real() != read_noise_complex[i].real() || noise_complex[i].imag() != read_noise_complex[i].imag())
        {
            fprintf(stderr, "Mismatch in complex noise at index %lu: original (%f, %f), read (%f, %f)\n", i, noise_complex[i].real(), noise_complex[i].imag(), read_noise_complex[i].real(), read_noise_complex[i].imag());
            return 1;
        }
    }

    printf("All tests passed successfully.\n");

    return 0;
}
