/**
 * @file FIRFilter.hpp
 * @author Daniele Giardino
 * @brief FIR Filter generation functions for DSP applications.
 *
 * This header file declares the FIRFilter class and related functions for generating various impulse responses of FIR filters
 */

#ifndef LIB_RAWDSP_PRIMITIVE_H
#define LIB_RAWDSP_PRIMITIVE_H

#include "Types.hpp"
#include "General.hpp"

#include <iostream>
#include <vector>
#include <random>

#include <numeric>
#include <algorithm>

#include <complex>
#include <type_traits>

namespace rawdsp
{

    /**
     * @brief The raw_conj function is a compile-time utility designed to provide a uniform interface
     * for calculating the conjugate of both scalar (real) and complex types within generic templates.
     */

    // This structure helps us determine if a type is a specialization of std::complex
    template <typename T>
    struct is_complex : std::false_type
    {
    };

    template <typename T>
    struct is_complex<std::complex<T>> : std::true_type
    {
    };

    template <typename T>
    constexpr auto raw_conj(const T &v)
    {
        if constexpr (is_complex<T>::value)
        {
            // If T is a specialization of std::complex
            return std::conj(v);
        }
        else
        {
            // If T is not a specialization of std::complex
            return v;
        }
    }

    /**
     * @brief Generate a vector of N random numbers drawn from a normal distribution with specified mean and standard deviation.
     *
     * @param N The number of random samples to generate.
     * @param mean The mean of the normal distribution (default is 0.0f).
     * @param stddev The standard deviation of the normal distribution (default is 1.0f).
     * @return A vector of random samples drawn from the specified normal distribution.
     */
    template <std::floating_point T>
    std::vector<T> randn(uint64_t N, T mean = 0.0f, T stddev = 1.0f);

    /**
     * @brief Generate a vector of N random numbers drawn from a normal distribution with specified mean and standard deviation.
     *
     * @param N The number of random samples to generate.
     * @param mean The mean of the normal distribution (default is 0.0f).
     * @param stddev The standard deviation of the normal distribution (default is 1.0f).
     * @return A c_float_t vector of random samples drawn from the specified normal distribution.
     */
    template <std::floating_point T>
    std::vector<std::complex<T>> randn_complex(size_t N, T mean = 0.0f, T stddev = 1.0f);

    /**
     * @brief Apply a gain factor to the input signal.
     *
     * @param input Input signal samples
     * @param G Gain to apply
     * @return a vector of amplified samples
     */
    template <RealOrComplex T, RealOrComplex U>
    std::vector<T> gain(const std::vector<T> &input, U G);

    /**
     * @brief Convolve the input signal with the real filter taps
     *
     * @param input Real input signal samples
     * @param taps Filter coefficients
     * @param removeLatency Whether to remove latency (i.e., return only the valid part of the convolution)
     * @return a vector of filtered samples
     */
    template <RealOrComplex T, RealOrComplex U>
    std::vector<T> conv(const std::vector<T> &input, const std::vector<U> &taps, bool removeLatency = false);

    /**
     * @brief Compute the auto-correlation of a real signal.
     *
     * @param x Real input signal samples
     * @param numLags Number of lags to compute (if 0, computes full signal length)
     * @return a vector of auto-correlation samples
     *
     * @note Example (real case to illustrate the concept):
     * For an input vector with 4 samples, printed auto-correlation values might be:
     *     i=00: autocorrelation[0]=4.0
     *     i=01: autocorrelation[1]=11.0
     *     i=02: autocorrelation[2]=20.0
     *     i=03: autocorrelation[3]=30.0  <-- peak at index 3 corresponds to lag 0
     *
     * The maximum value (peak) occurs at the lag corresponding to zero shift, which
     * for this example is index 3.
     */
    template <RealOrComplex T>
    std::vector<T> autocorr(const std::vector<T> &x, size_t numLags = 0);

    /**
     * @brief Compute the cross-correlation between two signals
     *
     * @param x First input signal samples
     * @param y Second input signal samples
     * @param numLags Number of lags to compute (if 0, computes full signal length)
     * @return a vector of cross-correlation samples
     */
    template <RealOrComplex T>
    std::vector<T> xcorr(const std::vector<T> &x, const std::vector<T> &y, size_t numLags = 0);

    /**
     * @brief Compute the lag vector of the cross-correlation between two signals
     *
     * @param N The number of samples in the lag vector
     * @return a vector of lag values corresponding to the cross-correlation samples
     */
    template <std::signed_integral T>
    std::vector<T> xcorr_lag(uint64_t N);

    /**
     * @brief Upsample the input signal by a factor of L (i.e., insert L-1 zeros between each sample)
     *
     * @param input Input signal samples
     * @param L Upsampling factor
     * @return a vector of upsampled samples
     */
    template <RealOrComplex T>
    std::vector<T> upsample(const std::vector<T> &input, uint64_t L);

    /**
     * @brief Downsample the input signal by a factor of M (i.e., keep every M-th sample)
     *
     * @param input Input signal samples
     * @param M Downsampling factor
     * @return a vector of downsampled samples
     */
    template <RealOrComplex T>
    std::vector<T> downsample(const std::vector<T> &input, uint64_t M);

    /**
     * @brief Perform the interpolation using the polyphase filter structure
     *
     * @param input Input signal samples
     * @param taps Filter coefficients for the polyphase filter (should be designed for the desired interpolation factor)
     * @param L Interpolation factor
     * @param removeLatency Whether to remove latency (i.e., return only the valid part of the convolution)
     * @return a vector of interpolated samples
     */
    template <RealOrComplex T, RealOrComplex U>
    std::vector<T> polyphase_interpolate(const std::vector<T> &input, const std::vector<U> &taps, uint64_t L, bool removeLatency = false);

    // /**
    //  * @brief Resample the input signal by a factor of L/M (i.e., upsample by L and then downsample by M)
    //  *
    //  * @param input Input signal samples
    //  * @param taps Optional filter coefficients for anti-aliasing (if empty, no filtering is applied)
    //  * @param L Upsampling factor
    //  * @param M Downsampling factor
    //  * @return a vector of resampled samples
    //  */
    // template <RealOrComplex T, RealOrComplex U>
    // std::vector<T> upfirdn(const std::vector<T> &input, const std::vector<U> &taps, uint64_t L, uint64_t M);

} // namespace rawdsp
#include "../src/Primitive.cpp"
#endif