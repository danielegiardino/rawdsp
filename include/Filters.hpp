/**
 * @file FIRFilter.hpp
 * @author Daniele Giardino
 * @brief FIR Filter generation functions for DSP applications.
 *
 * This header file declares the FIRFilter class and related functions for generating various impulse responses of FIR filters
 */

#ifndef LIB_RAWDSP_FIRFILTER_H
#define LIB_RAWDSP_FIRFILTER_H

#include "Types.hpp"
#include "General.hpp"

#include <iostream>
#include <vector>
#include <numeric>
#include <concepts>

namespace rawdsp
{
    /**
     * Windows generator.
     * @note [ ] {"Bartlett-Hann window"            ,  "barthannwin"   },
     * @note [ ] {"Bartlett window"                 ,  "bartlett"      },
     * @note [ ] {"Blackman window"                 ,  "blackman"      },
     * @note [ ] {"Blackman-Harris window"          ,  "blackmanharris"},
     * @note [ ] {"Bohman window"                   ,  "bohmanwin"     },
     * @note [ ] {"Chebyshev window"                ,  "chebwin"       },
     * @note [ ] {"Flat Top window"                 ,  "flattopwin"    },
     * @note [ ] {"Gaussian window"                 ,  "gausswin"      },
     * @note [ ] {"Hamming window"                  ,  "hamming"       },
     * @note [x] {"Hann window"                     ,  "hann"          },
     * @note [ ] {"Kaiser window"                   ,  "kaiser"        },
     * @note [x] {"Nuttall's Blackman-Harris window",  "nuttallwin"    },
     * @note [ ] {"Parzen window"                   ,  "parzenwin"     },
     * @note [ ] {"Rectangular window"              ,  "rectwin"       },
     * @note [ ] {"Tapered cosine window"           ,  "tukeywin"      },
     * @note [ ] {"Triangular window"               ,  "triang"        },
     * @param len     is the length of the vector
     * @param winName is the name of the window
     * @return pointer
     */
    template <std::floating_point T>
    std::vector<T> genWindow(uint32_t len, std::string windowName);

    /**
     * @brief Generate a sinc filter impulse response
     *
     * @param fn Normalized cutoff frequency (0 to 0.5)
     * @param order Filter order (number of coefficients - 1)
     * @return a vector of filter coefficients
     */
    template <std::floating_point T>
    std::vector<T> sinc(uint32_t ord, T fn);

    /**
     * Window-based FIR filter design fir1
     * @param ord     is the order of the filter
     * @param Fn      is the normalized frequency (i.e., Fn = Fc / (Fs/2))
     * @param window  is the window to apply
     */
    template <std::floating_point T>
    std::vector<T> fir1(uint32_t ord, T Fn, std::string windowName);

    /**
     * Window-based FIR filter design fir1
     * @param ord     is the order of the filter
     * @param Fc      is the cutoff frequency
     * @param Fs      is the sampling frequency
     * @param window  is the window to apply
     */
    template <std::floating_point T>
    std::vector<T> fir1(uint32_t ord, T Fc, T Fs, std::string windowName);

    /**
     * @brief Design a raised cosine filter with a normal or root shape. Note that the filter is not normalized to 0 dB at DC.
     *
     * @param beta Roll-off factor (0 to 1)
     * @param span Number of symbols the filter spans
     * @param sps Samples per symbol
     * @param shape Filter shape ("normal" or "root")
     * @return
     */
    template <std::floating_point T>
    std::vector<T> rcosdesign(T beta, uint32_t span, uint32_t sps, const std::string &shape = "Normal");

} // namespace rawdsp

#include "../src/Filters.cpp"
#endif