/**
 * @file SignalGen.hpp
 * @author Daniele Giardino
 * @brief Signal generation functions for DSP applications.
 */
#ifndef LIB_RAWDSP_SIGNALGEN_H
#define LIB_RAWDSP_SIGNALGEN_H

#include "Types.hpp"
#include <cmath>
#include <random>

namespace rawdsp
{
    /**
     * @brief Cosine wave generator
     * @param A Amplitude
     * @param fc Frequency of the cosine wave [Hz]
     * @param Fs Sampling frequency in [Hz]
     * @param phi Phase offset in [rad]
     * @param numSps Number of samples
     * @return a vector of samples
     */
    template <std::floating_point T>
    std::vector<T> wave_cosine(T A, T fc, T Fs, T phi, uint64_t numSps);

    /**
     * @brief Sine wave generator
     * @param A Amplitude
     * @param fc Frequency of the sine wave [Hz]
     * @param Fs Sampling frequency in [Hz]
     * @param phi Phase offset in [rad]
     * @param numSps Number of samples
     * @return a vector of samples
     */
    template <std::floating_point T>
    std::vector<std::complex<T>> wave_sine(T A, T fc, T Fs, T phi, uint64_t numSps);

    /**
     * @brief Complex CW generator
     * @param A Amplitude
     * @param fc Frequency of the CW [Hz]
     * @param Fs Sampling frequency in [Hz]
     * @param phi Phase offset in [rad]]
     * @param numSps Number of samples
     * @return a vector of samples
     */
    template <std::floating_point T>
    std::vector<std::complex<T>> complex_wave(T A, T fc, T Fs, T phi, uint64_t numSps);

    /**
     * @brief Generate N random MPSK symbols
     * @param M Modulation order
     * @param N Number of symbols to generate
     * @return a vector of symbols
     */
    template <std::floating_point T>
    std::vector<std::complex<T>> mpsk_symbols(uint64_t M, uint64_t N);

} // namespace rawdsp

#include "../src/SignalGen.cpp"

#endif