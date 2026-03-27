/**
 * @file Primitive.cpp
 * @author Daniele Giardino
 * @brief Implementation of primitive signal processing functions for rawdsp.
 */
#include "Primitive.hpp"

namespace rawdsp
{
    template <std::floating_point T>
    std::vector<T> randn(size_t N, T mean, T stddev)
    {
        std::vector<T> output(N);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<T> dist(mean, stddev);

        for (size_t i = 0; i < N; i++)
            output[i] = dist(gen);

        return output;
    }

    template <std::floating_point T>
    std::vector<std::complex<T>> randn_complex(size_t N, T mean, T stddev)
    {
        std::vector<std::complex<T>> output(N);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<T> dist(mean, stddev);

        for (size_t i = 0; i < N; i++)
        {
            T re = dist(gen);
            T im = dist(gen);
            output[i] = std::complex<T>(re, im);
        }
        return output;
    }

    template <RealOrComplex T, RealOrComplex U>
    std::vector<T> gain(const std::vector<T> &input, U G)
    {
        std::vector<T> output(input.size());

        for (uint64_t i = 0; i < input.size(); i++)
            output[i] = input[i] * G;

        return output;
    }

    template <RealOrComplex T, RealOrComplex U>
    std::vector<T> conv(const std::vector<T> &input, const std::vector<U> &taps, bool removeLatency)
    {
        const size_t nX = input.size();                          // Number of input samples
        const size_t nT = taps.size();                           // Number of filter taps
        const size_t nY = nX + nT - 1;                           // Output size depends on whether latency is removed
        const size_t latency = removeLatency ? (nT - 1) / 2 : 0; // Latency for centered convolution
        std::vector<T> output(nY, T(0));                         // Initialize output with zeros

        /* Input-side algorithm
         * Perform convolution implemented using the matrix multiplication shown as follows:
         *  ┌─  ─┐   ┌─           ─┐ ┌─  ─┐
         *  │ y0 │   │ h0  0  0  0 │ │ x0 │
         *  │ y1 │   │ h1 h0  0  0 │ │ x1 │
         *  │ y2 │ = │ h2 h1 h0  0 │ │ x2 │
         *  │ y3 │   │  0 h2 h1 h0 │ │ x3 │
         *  │ y4 │   │  0  0 h2 h1 │ └─  ─┘
         *  │ y5 │   │  0  0  0 h2 │
         *  └─  ─┘   └─           ─┘
         */
        for (uint64_t ix = 0; ix < nX; ix++)
            for (uint64_t ih = 0; ih < nT; ih++)
                output[ix + ih] = output[ix + ih] + input[ix] * taps[ih];

        // If removeLatency is true, we return only the valid part of the convolution (centered output)
        if (removeLatency)
        {
            std::vector<T> output_NoLatency(nX);
            // std::copy(output.begin() + latency, output.begin() + latency + nX, output_NoLatency.begin());
            for (size_t i = 0; i < nX; ++i)
                output_NoLatency[i] = output[i + latency];
            return output_NoLatency;
        }
        return output;
    }

    template <RealOrComplex T>
    std::vector<T> autocorr(const std::vector<T> &x, size_t numLags)
    {
        const int64_t nX = static_cast<int64_t>(x.size());
        if (nX == 0)
            return {};

        size_t fullSize = 2 * nX - 1;
        if (numLags == 0 || numLags > fullSize)
            numLags = fullSize;

        std::vector<T> output(numLags, T(0));

        // To center numLags around lag 0:
        // We want the middle of our 'output' to be lag 0.
        int64_t halfLags = static_cast<int64_t>(numLags) / 2;

        for (size_t i = 0; i < numLags; ++i)
        {
            // This ensures that when i = halfLags, lag = 0
            int64_t lag = static_cast<int64_t>(i) - halfLags;

            T sum = T(0);
            for (int64_t n = 0; n < nX; ++n)
            {
                int64_t m = n - lag;
                if (m >= 0 && m < nX)
                {
                    // During the compilation, the conjugate will be optimized out for real types,
                    // so we can use it directly for both real and complex cases.
                    sum += x[n] * raw_conj(x[m]);
                }
            }
            output[i] = sum;
        }
        return output;
    }

    template <RealOrComplex T>
    std::vector<T> xcorr(const std::vector<T> &x, const std::vector<T> &y, size_t numLags)
    {
        const int64_t nX = static_cast<int64_t>(x.size());
        const int64_t nY = static_cast<int64_t>(y.size());
        if (nX == 0 || nY == 0)
            return {};

        if (numLags == 0)
        {
            throw std::invalid_argument("Number of lags must be greater than zero");
        }

        // Ensure numLags is odd for a perfect center, or handle even gracefully
        // We want the range to be roughly [-numLags/2, +numLags/2]
        int64_t halfLags = static_cast<int64_t>(numLags) / 2;
        int64_t startLag = -halfLags;

        std::vector<T> output(numLags, T(0));

        for (size_t i = 0; i < numLags; ++i)
        {
            int64_t lag = startLag + static_cast<int64_t>(i);
            T sum = T(0);

            for (int64_t n = 0; n < nX; ++n)
            {
                int64_t m = n - lag; // Index for y
                if (m >= 0 && m < nY)
                {
                    sum += x[n] * y[m];
                }
            }
            output[i] = sum;
        }
        return output;
    }

    template <std::signed_integral T>
    std::vector<T> xcorr_lag(uint64_t N)
    {
        std::vector<T> lags(N);

        // The center (zero lag) is at index (N-1) / 2
        T center = (N - 1) / 2;

        // We calculate the starting lag value
        T startLag = -center;

        for (T i = 0; i < N; ++i)
        {
            lags[i] = startLag + i;
        }

        return lags;
    }

    template <RealOrComplex T>
    std::vector<T> upsample(const std::vector<T> &input, uint64_t L)
    {
        // Calculate the number of output samples
        uint64_t numOutputSps = input.size() * L;

        // Output vector initialized with zeros
        std::vector<T> output(numOutputSps, T(0));

        // Insert input samples into the output vector at the correct positions
        for (uint64_t i = 0; i < input.size(); i++)
            output[i * L] = input[i];

        return output;
    }
    template <RealOrComplex T>
    std::vector<T> downsample(const std::vector<T> &input, uint64_t M)
    {
        // Calculate the number of output samples
        uint64_t numOutputSps = (input.size() % M == 0) ? (input.size() / M) : (input.size() / M + 1);

        // Output vector initialized with zeros
        std::vector<T> output(numOutputSps, T(0));

        // Keep every M-th sample from the input vector
        for (uint64_t i = 0; i < output.size(); i++)
            output[i] = input[i * M];
        return output;
    }

    template <RealOrComplex T, RealOrComplex U>
    std::vector<T> polyphase_interpolate(const std::vector<T> &input,
                                         const std::vector<U> &taps,
                                         size_t L,
                                         bool removeLatency)
    {
        const size_t nX = input.size();
        const size_t nT = taps.size();
        const size_t nY = L * (nX - 1) + nT; // Correct full convolution size
        const size_t latency = (nT - 1) / 2; // Delay in output samples

        std::vector<T> output(nY, T(0));

        // Upsample and filter in one pass (direct polyphase approach)
        for (size_t ix = 0; ix < nX; ++ix)
            for (size_t ih = 0; ih < nT; ++ih)
                output[L * ix + ih] += input[ix] * taps[ih];

        if (removeLatency)
        {
            // Output has nX * L samples after removing latency
            const size_t outLen = nX * L;
            std::vector<T> output_NoLatency(outLen);
            std::copy(output.begin() + latency,
                      output.begin() + latency + outLen,
                      output_NoLatency.begin());
            return output_NoLatency;
        }

        return output;
    }

 
    // template <typename T>
    // std::vector<T> upfirdn(const std::vector<T> &input, const std::vector<float> &filterTaps, uint64_t L, uint64_t M)
    // {
    //     // Output size after upsampling, filtering, and downsampling
    //     uint64_t numOutputSps = (input.size() * L + filterTaps.size() - 1) / M;

    //     // Output vector initialized with zeros
    //     std::vector<T> output(numOutputSps, T(0));

    //     // Create polyphase matrix for the filter taps
    //     std::vector<std::vector<float>> polyphaseMatrix(L);
    //     for (size_t i = 0; i < filterTaps.size(); i++)
    //         polyphaseMatrix[i % L].push_back(filterTaps[i]);

    //     // Perform upfirdn operation
    //     for (size_t n = 0; n < numOutputSps; n++)
    //     {
    //         T sum(0);
    //         for (size_t k = 0; k < L; k++)
    //         {
    //             size_t inputIndex = (n * M + k) / L;
    //             if (inputIndex < input.size())
    //             {
    //                 for (size_t m = 0; m < polyphaseMatrix[k].size(); m++)
    //                 {
    //                     size_t tapIndex = m * L + k;
    //                     if (tapIndex < filterTaps.size())
    //                     {
    //                         sum = sum + input[inputIndex] * polyphaseMatrix[k][m];
    //                     }
    //                 }
    //             }
    //         }
    //         output[n] = sum;
    //     }
    //     return output;
    // }

}