/**
 * @file Filters.cpp
 * @author Daniele Giardino
 * @brief Implementation of filter design functions for rawdsp.
 */
#include "Filters.hpp"

namespace rawdsp
{

    template <std::floating_point T>
    std::vector<T> genWindow(uint32_t len, std::string windowName)
    {
        // Validate input parameters
        if (len == 0)
        {
            std::string errMsg = RED;
            errMsg += "Length of the window must be greater than 0. Actually, len=" + std::to_string(len) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }

        std::vector<T> window(len);
        T_fun a0, a1, a2, a3;

        if (windowName == "hann")
        {
            T_fun N = (T_fun) len;
            T_fun den = N - 1.0;
            for (uint32_t n = 0; n < N; n++)
            {
                window[n] = 0.5 * (1.0 - cos(2.0 * M_PI * (T_fun) n / N));
            }
        }
        else if (windowName == "nuttall")
        {
            uint32_t N = len;
            T_fun phi_inc = 2.0 * M_PI / (N - 1);
            a0 = 0.3635819;
            a1 = 0.4891775;
            a2 = 0.1365995;
            a3 = 0.0106411;
            T_fun phi_1 = 0;
            T_fun phi_2 = 0;
            T_fun phi_3 = 0;

            for (uint32_t n = 0; n < len; n++)
            {
                window[n] = a0 - a1 * cos(phi_1) + a2 * cos(phi_2) - a3 * cos(phi_3);
                phi_1 += phi_inc;
                phi_1 = (phi_1 < M_PI) ? phi_1 : phi_1 - 2 * M_PI;
                phi_2 += 2 * phi_inc;
                phi_2 = (phi_2 < M_PI) ? phi_2 : phi_2 - 2 * M_PI;
                phi_3 += 3 * phi_inc;
                phi_3 = (phi_3 < M_PI) ? phi_3 : phi_3 - 2 * M_PI;
            }
        }
        else
        {
            std::string errMsg = RED;
            errMsg += "The window " + windowName + " is not implemented.\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }
        return window;
    }

    template <std::floating_point T>
    std::vector<T> sinc(uint32_t ord, T fn)
    {
        // Validate input parameters
        if (fn <= 0 || fn > 0.5)
        {
            std::string errMsg = RED;
            errMsg += "The normalized frequency fn must be between 0 and 0.5. Actually, fn=" + std::to_string(fn) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }
        if (ord == 0)
        {
            std::string errMsg = RED;
            errMsg += "Filter order must be greater than 0. Actually, ord=" + std::to_string(ord) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }

        std::vector<T> taps(ord + 1);             // Vector to return
        T_fun center = (ord) / 2.0;       // Center of the sinc (the point where the main lobe is located)
        T_fun n = -center;                        // First point (extreme left) of the sinc
        T_fun phi_inc = M_PI * fn;                // Phase used to increment the phase of the sinc
        T_fun phi_rad = n * phi_inc;              // Initial phase of the sinc
        phi_rad = std::fmod(phi_rad, 2.0 * M_PI); // Limit the range between [-pi, pi)

        for (uint32_t i = 0; i < ord + 1; ++i)
        {
            if (std::abs(n) < 1e-9f)
                taps[i] = 1.0; // limit for t→0
            else
                taps[i] = std::sin(phi_rad) / (phi_inc * n);
            phi_rad += phi_inc;
            phi_rad = std::fmod(phi_rad, 2.0 * M_PI); // Limit the range between [-pi, pi)
            n++;
        }

        return taps;
    }

    template <std::floating_point T>
    std::vector<T> fir1(uint32_t ord, T Fn, std::string windowName)
    {
        // Validate input parameters
        if (ord == 0)
        {
            std::string errMsg = RED;
            errMsg += "Filter order must be greater than 0. Actually, ord=" + std::to_string(ord) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }
        if (Fn >= 1)
        {
            std::string errMsg = RED;
            errMsg += "The normalized frequency Fn must be lower than 1. Actually, Fn=" + std::to_string(Fn) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }

        uint32_t len = ord + 1;
        std::vector<T> coeffs = sinc<T>(ord, Fn); // Get the sinc coefficients
        for (uint32_t i = 0; i < len; i++)
            printf("  fir1 - sinc[%u] = %.4f\n", i, coeffs[i]);

        // Apply the window
        std::vector<T> window = genWindow<T>(len, windowName);
        for (uint32_t i = 0; i < len; i++)
            coeffs[i] = coeffs[i] * window[i];

        // Normalize to 0 dB
        T_fun energy = 0;
        for (uint32_t i = 0; i < len; i++)
            energy = energy + coeffs[i];
        T_fun energy_inv = 1.0 / energy;

        for (uint32_t i = 0; i < len; i++)
            coeffs[i] = coeffs[i] * energy_inv;

        return coeffs;
    }

    template <std::floating_point T>
    std::vector<T> fir1(uint32_t ord, T Fc, T Fs, std::string windowName)
    {
        // Validate input parameters
        if (Fc <= 0 || Fc >= Fs / 2)
        {
            std::string errMsg = RED;
            errMsg += "Cutoff frequency Fc must be between 0 and Nyquist frequency (Fs/2). Actually, Fc=" + std::to_string(Fc) + " and Fs/2=" + std::to_string(Fs / 2) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }

        T_fun Fn = Fc / (Fs / 2.0); // Normalized frequency
        return fir1<T>(ord, Fn, windowName);
    }

    template <std::floating_point T>
    std::vector<T> rcosdesign(T beta, uint32_t span, uint32_t sps, const std::string &shape)
    {
        // Validate input parameters
        if (beta < 0.0 || beta > 1.0)
        {
            std::string errMsg = RED;
            errMsg += "Roll-off factor beta must be between 0 and 1. Actually, beta=" + std::to_string(beta) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }
        if (span == 0)
        {
            std::string errMsg = RED;
            errMsg += "Span must be greater than 0. Actually, span=" + std::to_string(span) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }
        if (sps == 0)
        {
            std::string errMsg = RED;
            errMsg += "Samples per symbol (sps) must be greater than 0. Actually, sps=" + std::to_string(sps) + "\n";
            errMsg += RESET;
            throw std::invalid_argument(errMsg);
        }

        uint32_t numCoeffs = span * sps + 1;
        std::vector<T> h(numCoeffs);

        // Center the time vector around 0
        // t is normalized such that Symbol Period T = 1.0
        T_fun dt = 1.0 / static_cast<T_fun>(sps);

        for (uint32_t i = 0; i < numCoeffs; ++i)
        {
            T_fun t = (i - (numCoeffs - 1) / 2.0) * dt;

            if (shape == "Root")
            {
                // Case 1: The peak at t = 0
                if (std::abs(t) < 1e-10)
                {
                    h[i] = 1.0 - beta + (4.0 * beta / M_PIf32);
                }
                // Case 2: The singularities at t = +/- 1/(4*beta)
                else if (std::abs(std::abs(4.0 * beta * t) - 1.0) < 1e-10)
                {
                    h[i] = (beta / std::sqrt(2.0)) * ((1.0 + 2.0 / M_PIf32) * std::sin(M_PIf32 / (4.0 * beta)) +
                                                      (1.0 - 2.0 / M_PIf32) * std::cos(M_PIf32 / (4.0 * beta)));
                }
                // Case 3: Standard formula
                else
                {
                    T_fun num = std::sin(M_PIf32 * t * (1.0 - beta)) +
                                4.0 * beta * t * std::cos(M_PIf32 * t * (1.0 + beta));
                    T_fun den = M_PIf32 * t * (1.0 - std::pow(4.0 * beta * t, 2.0));
                    h[i] = num / den;
                }
            }
            else if (shape == "Normal")
            {
                // Case 1: Singularities at t = +/- 1/(2*beta)
                if (std::abs(std::abs(2.0 * beta * t) - 1.0) < 1e-10)
                {
                    h[i] = (M_PIf32 / 4.0) * (std::sin(M_PIf32 * t) / (M_PIf32 * t));
                }
                // Case 2: Standard formula (sinc(t) * cos / den)
                else
                {
                    T_fun sinc_t = (std::abs(t) < 1e-10) ? 1.0 : std::sin(M_PIf32 * t) / (M_PIf32 * t);
                    h[i] = sinc_t * std::cos(M_PIf32 * beta * t) / (1.0 - std::pow(2.0 * beta * t, 2.0));
                }
            }
            else
            {
                std::string errMsg = RED;
                errMsg += "Shape must be 'Normal' or 'Root'. Actually, shape='" + shape + "'\n";
                errMsg += RESET;
                throw std::invalid_argument(errMsg);
            }
        }

        return h;
    }

} // namespace rawdsp