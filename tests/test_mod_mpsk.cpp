/**
 * @file test_mod_mpsk.cpp
 * @author Daniele Giardino
 * @brief Test for MPSK modulation, including symbol generation, filtering with raised cosine and root raised cosine filters, and writing results to binary files.
 */

#include "rawdsp.hpp"

int main(int argc, char const *argv[])
{
    // MPSK Symbol Generation
    uint64_t M = 4;  // Modulation order (e.g., 4-PSK)
    uint64_t N = 64; // Number of symbols to generate
    std::vector<std::complex<float>> symbols = rawdsp::mpsk_symbols<float>(M, N);
    for (size_t i = 0; i < symbols.size(); i++)
        printf("Symbol %lu: (%+02.4f, %+02.4f)\n", i, symbols[i].real(), symbols[i].imag());

    // Raised cosine filter parameters
    float rolloff = 0.25f; // Roll-off factor
    uint64_t span = 8;     // Filter span in symbols
    uint64_t sps = 4;      // Samples per symbol
    std::vector<float> filter_rc = rawdsp::rcosdesign(rolloff, span, sps, "Normal");
    std::vector<float> filter_rrc = rawdsp::rcosdesign(rolloff, span, sps, "Root");

    // Print filter coefficients
    std::cout << "Raised Cosine Filter Coefficients:\n";
    for (size_t i = 0; i < filter_rc.size(); i++)
        printf("i=%lu | %f\n", i, filter_rc[i]);
    std::cout << "\n\nRoot Raised Cosine Filter Coefficients:\n";
    for (size_t i = 0; i < filter_rrc.size(); i++)
        printf("i=%lu | %f\n", i, filter_rrc[i]);

    // Filtering the MPSK symbols with the raised cosine filter
    std::vector<std::complex<float>> x_up = rawdsp::upsample(symbols, sps);                // Upsample the symbols
    std::vector<std::complex<float>> x_filt_rc = rawdsp::conv(x_up, filter_rc);            // Filter with raised cosine
    std::vector<std::complex<float>> x_filt_rrc = rawdsp::conv(x_up, filter_rrc);          // Filter with root raised cosine
    std::vector<std::complex<float>> x_filt_rrc_x2 = rawdsp::conv(x_filt_rrc, filter_rrc); // Filter with root raised cosine to have the raised cosine response

    // Write binary file
    rawdsp::writeBinFile("mod_qpsk_coeffs_rc.dat", filter_rc, filter_rc.size());
    rawdsp::writeBinFile("mod_qpsk_coeffs_rrc.dat", filter_rrc, filter_rrc.size());
    rawdsp::writeBinFile("mod_qpsk_symbols.dat", symbols, symbols.size());
    rawdsp::writeBinFile("mod_qpsk_x_up.dat", x_up, x_up.size());
    rawdsp::writeBinFile("mod_qpsk_filt_rc.dat", x_filt_rc, x_filt_rc.size());
    rawdsp::writeBinFile("mod_qpsk_filt_rrc.dat", x_filt_rrc, x_filt_rrc.size());
    rawdsp::writeBinFile("mod_qpsk_filt_rrc_x2.dat", x_filt_rrc_x2, x_filt_rrc_x2.size());
}
