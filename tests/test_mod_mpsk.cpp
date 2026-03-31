/**
 * @file test_mod_mpsk.cpp
 * @author Daniele Giardino
 * @brief Test for MPSK modulation, including symbol generation, filtering with raised cosine and root raised cosine filters, and writing results to binary files.
 */

#include "rawdsp.hpp"

// int main(int argc, char const *argv[])
// {
//     // MPSK Symbol Generation
//     uint64_t M = 4;  // Modulation order (e.g., 4-PSK)
//     uint64_t N = 64; // Number of symbols to generate
//     std::vector<std::complex<float>> symbols = rawdsp::mpsk_symbols<float>(M, N);
//     for (size_t i = 0; i < symbols.size(); i++)
//         printf("Symbol %lu: (%+02.4f, %+02.4f)\n", i, symbols[i].real(), symbols[i].imag());

//     // Raised cosine filter parameters
//     float rolloff = 0.25f; // Roll-off factor
//     uint64_t span = 8;     // Filter span in symbols
//     uint64_t sps = 4;      // Samples per symbol
//     std::vector<float> filter_rc = rawdsp::rcosdesign(rolloff, span, sps, "Normal");
//     std::vector<float> filter_rrc = rawdsp::rcosdesign(rolloff, span, sps, "Root");

//     // Print filter coefficients
//     std::cout << "Raised Cosine Filter Coefficients:\n";
//     for (size_t i = 0; i < filter_rc.size(); i++)
//         printf("i=%lu | %f\n", i, filter_rc[i]);
//     std::cout << "\n\nRoot Raised Cosine Filter Coefficients:\n";
//     for (size_t i = 0; i < filter_rrc.size(); i++)
//         printf("i=%lu | %f\n", i, filter_rrc[i]);

//     // Filtering the MPSK symbols with the raised cosine filter
//     std::vector<std::complex<float>> x_up = rawdsp::upsample(symbols, sps);                // Upsample the symbols
//     std::vector<std::complex<float>> x_filt_rc = rawdsp::conv(x_up, filter_rc);            // Filter with raised cosine
//     std::vector<std::complex<float>> x_filt_rrc = rawdsp::conv(x_up, filter_rrc);          // Filter with root raised cosine
//     std::vector<std::complex<float>> x_filt_rrc_x2 = rawdsp::conv(x_filt_rrc, filter_rrc); // Filter with root raised cosine to have the raised cosine response

//     // Write binary file
//     rawdsp::writeBinFile("mod_qpsk_coeffs_rc.dat", filter_rc, filter_rc.size());
//     rawdsp::writeBinFile("mod_qpsk_coeffs_rrc.dat", filter_rrc, filter_rrc.size());
//     rawdsp::writeBinFile("mod_qpsk_symbols.dat", symbols, symbols.size());
//     rawdsp::writeBinFile("mod_qpsk_x_up.dat", x_up, x_up.size());
//     rawdsp::writeBinFile("mod_qpsk_filt_rc.dat", x_filt_rc, x_filt_rc.size());
//     rawdsp::writeBinFile("mod_qpsk_filt_rrc.dat", x_filt_rrc, x_filt_rrc.size());
//     rawdsp::writeBinFile("mod_qpsk_filt_rrc_x2.dat", x_filt_rrc_x2, x_filt_rrc_x2.size());
// }

int main(int argc, char const *argv[])
{
    float Fs = 200e3;                             // Sampling Frequency
    float Fsym = 25e3;                            // Symbol Rate
    float BurstDuration = 1.0;                    // Burst Duration
    uint64_t N = std::ceil(BurstDuration * Fsym); // Number of symbols

    // MPSK Symbol Generation
    uint64_t M = 4; // Modulation order (e.g., 4-PSK)
    std::vector<std::complex<float>> symbols = rawdsp::mpsk_symbols<float>(M, N);
    for (size_t i = 0; i < 2500; i++)
        symbols[i] = std::complex(0, 0);

    for (size_t i = 2500; i < 7500; i++)
    {
        float phase = M_PI / 4;
        if (i % 2 == 1)
            phase += M_PI;

        symbols[i] = std::complex(std::cos(phase), std::sin(phase));
    }

    // Raised cosine filter parameters
    float rolloff = 0.25f;    // Roll-off factor
    uint64_t span = 12;        // Filter span in symbols
    uint64_t sps = Fs / Fsym; // Samples per symbol
    std::vector<float> filter_rc = rawdsp::rcosdesign(rolloff, span, sps, "Root");

    // Print filter coefficients
    std::cout << "Raised Cosine Filter Coefficients:\n";
    for (size_t i = 0; i < filter_rc.size(); i++)
        printf("i=%lu | %f\n", i, filter_rc[i]);

    // Filtering the MPSK symbols with the raised cosine filter
    std::vector<std::complex<float>> x_up = rawdsp::upsample(symbols, sps);     // Upsample the symbols
    std::vector<std::complex<float>> x_filt_rc = rawdsp::conv(x_up, filter_rc); // Filter with raised cosine


    // Add noise
    std::vector<c_float_t> noise = rawdsp::randn_complex(x_filt_rc.size(), 0.0f, 1e-3f);
    for (size_t i = 0; i < x_filt_rc.size(); i++)
        x_filt_rc[i] += noise[i];    

    // Write binary file
    // rawdsp::writeBinFile("mod_qpsk_coeffs_rc.dat", filter_rc, filter_rc.size());
    // rawdsp::writeBinFile("mod_qpsk_symbols.dat", symbols, symbols.size());
    // rawdsp::writeBinFile("mod_qpsk_x_up.dat", x_up, x_up.size());
    // rawdsp::writeBinFile("mod_qpsk_filt_rc.dat", x_filt_rc, x_filt_rc.size());

    // Find the max of x_filt_rrc
    float x_max = 0.0;
    for (size_t i = 0; i < x_filt_rc.size(); i++)
    {
        float x_re = x_filt_rc[i].real();
        float x_im = x_filt_rc[i].imag();
        x_re = std::abs(x_re);
        x_im = std::abs(x_im);

        float x_max_tmp = std::max(x_re, x_im);
        x_max = std::max(x_max, x_max_tmp);
    }

    // Scale the amplitude
    float G = pow(2, 14) / x_max;
    for (size_t i = 0; i < x_filt_rc.size(); i++)
    {
        float x_re = std::round(x_filt_rc[i].real() * G);
        float x_im = std::round(x_filt_rc[i].imag() * G);
        x_filt_rc[i] = std::complex<float>(x_re, x_im);
    }

    // Float to int16
    std::vector<c_int16_t> x_int16(x_filt_rc.size());
    for (size_t i = 0; i < x_filt_rc.size(); i++)
        x_int16[i] = c_int16_t(x_filt_rc[i].real(), x_filt_rc[i].imag());
    
    rawdsp::writeBinFile("b210_qpsk_test.dat", x_int16, x_int16.size());

}
