/**
 * @file SignalGen.cpp
 * @author Daniele Giardino
 * @brief Implementation of signal generation functions for rawdsp.
 */
#include "SignalGen.hpp"

namespace rawdsp
{
    template <std::floating_point T>
    std::vector<T> wave_cosine(T A, T fc, T Fs, T phi, uint64_t numSps)
    {
        std::vector<T> cw(numSps);

        T_fun fn = (T_fun)(fc) / (T_fun)(Fs);
        T_fun phi_rad = phi;
        T_fun phi_inc = 2.0f * M_PIf32 * fn;
        for (uint64_t i = 0; i < numSps; i++)
        {
            cw[i] = A * std::cos(phi_rad);
            phi_rad += phi_inc;
            phi_rad = std::fmod(phi_rad, 2.0f * M_PIf64); // Wrap phase to [0, 2*pi)
        }

        return cw;
    }

    template <std::floating_point T>
    std::vector<T> wave_sine(T A, T fc, T Fs, T phi, uint64_t numSps)
    {
        std::vector<T> cw(numSps);

        T_fun fn = (T_fun)(fc) / (T_fun)(Fs);
        T_fun phi_rad = phi;
        T_fun phi_inc = 2.0f * M_PIf32 * fn;
        for (uint64_t i = 0; i < numSps; i++)
        {
            cw[i] = A * std::sin(phi_rad);
            phi_rad += phi_inc;
            phi_rad = std::fmod(phi_rad, 2.0f * M_PIf64); // Wrap phase to [0, 2*pi)
        }

        return cw;
    }

    template <std::floating_point T>
    std::vector<std::complex<T>> complex_wave(T A, T fc, T Fs, T phi, uint64_t numSps)
    {
        std::vector<std::complex<T>> cw(numSps);

        T_fun fn = (T_fun)(fc) / (T_fun)(Fs);
        T_fun phi_rad = (T_fun)(phi);
        T_fun phi_inc = 2.0f * M_PIf32 * fn;
        for (uint64_t i = 0; i < numSps; i++)
        {
            cw[i].real(A * std::cos(phi_rad));
            cw[i].imag(A * std::sin(phi_rad));
            phi_rad += phi_inc;
            phi_rad = std::fmod(phi_rad, 2.0f * M_PIf64); // Wrap phase to [0, 2*pi)
        }

        return cw;
    }

    template <std::floating_point T>
    std::vector<std::complex<T>> mpsk_symbols(uint64_t M, uint64_t N)
    {
        std::vector<std::complex<T>> symbols(N);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, M - 1);
        constexpr float tol = 1e-6f;

        for (uint64_t i = 0; i < N; ++i)
        {
            int32_t k = dis(gen);
            double phase = (2.0f * M_PIf32 * k) / M + M_PIf32 / M; // Center constellation points in the middle of the phase sectors
            symbols[i].real(std::cos(phase));
            symbols[i].imag(std::sin(phase));

            if (std::abs(symbols[i].real()) < tol)
                symbols[i].real(0.0f);
            if (std::abs(symbols[i].imag()) < tol)
                symbols[i].imag(0.0f);
        }

        return symbols;
    }

} // namespace rawdsp