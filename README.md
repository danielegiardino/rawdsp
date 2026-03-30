# RAWDSP ⚡

**Low-level. No fluff. Pure signal.**

RAWDSP is a C++ toolkit for digital signal processing that doesn't get in your way.
No abstractions you didn't ask for, no bloated dependencies — just sharp algorithms, a clean API, and full control over your signal pipeline.

Built for hackers, embedded engineers, and anyone who actually knows what a Nyquist frequency is.

- [RAWDSP ⚡](#rawdsp-)
  - [What's Inside](#whats-inside)
  - [API: Functions (by header)](#api-functions-by-header)
  - [Build It](#build-it)
    - [`T_fun` — default precision type](#t_fun--default-precision-type)
  - [Use It](#use-it)
    - [CMakeLists.txt example](#cmakeliststxt-example)
  - [GUI Data Visualizer](#gui-data-visualizer)
  - [Bash script to run all test](#bash-script-to-run-all-test)
  - [Contribute](#contribute)


---

## What's Inside

| Module                          | What it does                                                        |
| ------------------------------- | ------------------------------------------------------------------- |
| 🎛️ **Signal Generation**         | Sine, square — raw waveforms, ready to pipe                         |
| 🔪 **Filtering**                 | Low-pass, high-pass, band-pass, band-stop — cut what you don't need |
| 🔁 **Convolution & Correlation** | The primitives DSP is built on, exposed cleanly                     |
| 🪟 **Window Functions**          | Hamming, Hanning, Blackman — shape your spectrum                    |
<!-- | 📡 **FFT / IFFT**                | Fast Fourier Transform, forward and inverse. No magic, just math    | -->


## API: Functions (by header)

| Header | Function | Description |
| --- | --- | --- |
| include/Filters.hpp | `template<floating_point T> std::vector<T> genWindow(uint32_t len, std::string windowName)` | Generate window (Hann/Nuttall/etc.) |
| include/Filters.hpp | `template<floating_point T> std::vector<T> sinc(uint32_t ord, T fn)` | Sinc kernel generator |
| include/Filters.hpp | `template<floating_point T> std::vector<T> fir1(uint32_t ord, T Fn, std::string windowName)` | FIR design (normalized freq) |
| include/Filters.hpp | `template<floating_point T> std::vector<T> fir1(uint32_t ord, T Fc, T_fun Fs, std::string windowName)` | FIR design (cutoff + sample rate) |
| include/Filters.hpp | `template<floating_point T> std::vector<T> rcosdesign(T beta, uint32_t span, uint32_t sps, const std::string &shape = "Normal")` | Raised‑cosine / root‑raised‑cosine pulse shaping |
| include/Primitive.hpp | `template<RealOrComplex T, RealOrComplex U> std::vector<T> gain(const std::vector<T> &input, U G)` | Apply gain to samples |
| include/Primitive.hpp | `template<RealOrComplex T, RealOrComplex U> std::vector<T> conv(const std::vector<T> &input, const std::vector<U> &taps, bool removeLatency = false)` | Convolution primitive, optional latency removal |
| include/Primitive.hpp | `template<RealOrComplex T> std::vector<T> upsample(const std::vector<T> &input, uint64_t L)` | Zero‑insertion upsampling by L |
| include/Primitive.hpp | `template<RealOrComplex T> std::vector<T> downsample(const std::vector<T> &input, uint64_t M)` | Decimation by M (simple downsample) |
| include/Primitive.hpp | `template<RealOrComplex T, RealOrComplex U> std::vector<T> polyphase_interpolate(const std::vector<T> &input, const std::vector<U> &taps, uint64_t L, bool removeLatency = false)` | Polyphase interpolation |
| include/Primitive.hpp | `template<floating_point T> std::vector<T> randn(uint64_t N, T mean = 0.0f, T stddev = 1.0f)` | Generate real Gaussian random samples |
| include/Primitive.hpp | `template<floating_point T> std::vector<std::complex<T>> randn_complex(size_t N, T mean = 0.0f, T stddev = 1.0f)` | Generate complex Gaussian random samples |
| include/Primitive.hpp | `template<RealOrComplex T> std::vector<T> autocorr(const std::vector<T> &x, size_t numLags = 0)` | Autocorrelation (real or complex) |
| include/Primitive.hpp | `template<RealOrComplex T> std::vector<T> xcorr(const std::vector<T> &x, const std::vector<T> &y, size_t numLags = 0)` | Cross-correlation (real or complex) |
| include/Primitive.hpp | `template<signed_integral T> std::vector<T> xcorr_lag(uint64_t N)` | Lag vector helper for cross-correlation |
| include/SignalGen.hpp | `template<floating_point T> std::vector<T> wave_cosine(T A, T fc, T Fs, T phi, uint64_t numSps)` | Cosine waveform generator |
| include/SignalGen.hpp | `template<floating_point T> std::vector<std::complex<T>> wave_sine(T A, T fc, T Fs, T phi, uint64_t numSps)` | Sine waveform generator |
| include/SignalGen.hpp | `template<floating_point T> std::vector<std::complex<T>> complex_wave(T A, T fc, T Fs, T phi, uint64_t numSps)` | Complex exponential waveform |
| include/SignalGen.hpp | `template<floating_point T> std::vector<std::complex<T>> mpsk_symbols(uint64_t M, uint64_t N)` | M‑PSK symbol generator |
| include/General.hpp | `void printerr(std::string x)` | Print red error text |
| include/General.hpp | `template<floating_point\|integral T> void plotXY(const std::vector<T> &x, const std::vector<T> &y, int width = 60, int height = 20)` | ASCII terminal plot |
| include/Types.hpp | `c_intT_t<T>` operators & methods (`+ - * /`, `print()`, `magnitude()`, `phase()`, `conjugate()`, `swap()`) | Core integer complex type utilities |
| include/Types.hpp | `c_int8_t`, `c_int16_t`, `c_int32_t` with `mul_sat` / `div_sat` | Fixed‑width saturated arithmetic helpers |
| include/Types.hpp | `c_int64_t` | 64‑bit integer complex type |
| include/Types.hpp | `c_float_t`, `c_double_t`, `c_long_double_t` (aliases for `std::complex<>`) | Floating‑point complex type aliases |
| include/Types.hpp | `RealOrComplex`, `onlyComplex` concepts | Template constraint concepts |
| include/FileIO.hpp | `uint64_t lengthFile(std::string fileName)` | File length helper |
| include/FileIO.hpp | `template<typename T> std::vector<T> readBinFile(std::string fileName, uint64_t numSamples, uint64_t seek)` | Read binary samples |
| include/FileIO.hpp | `template<typename T> void writeBinFile(std::string fileName, const std::vector<T> &p, uint64_t numSamples)` | Write binary samples |
| include/rawdsp.hpp | umbrella header | Aggregates all public headers |

---

## Build It

```bash
git clone https://github.com/danielegiardino/rawdsp.git
cd rawdsp && mkdir build && cd build
cmake .. && make
```

No package manager. No setup wizard. Just `cmake` and `make`.

### `T_fun` — default precision type

Several internal computations (window generation, signal generators, filter design) use the type alias `T_fun`, defined in [include/Types.hpp](include/Types.hpp):

```cpp
using T_fun = double;
```

By default it resolves to `double`. If you need a different precision for those internal paths, change this single typedef before building.

---

## Use It

```cpp
#include "rawdsp.hpp"

int main(int argc, char const *argv[])
{
    // Test convolution with a simple impulse response
    std::vector<float> impulse = {1.0f, 1.0f, 0.0f, 0.0f}; // Impulse signal
    std::vector<float> coefficients = {0.25f, 0.25f, 0.25f, 0.25f}; // Simple moving average filter

    std::vector<float> output_noLatency = rawdsp::conv(impulse, coefficients);
    std::vector<float> output_withLatency = rawdsp::conv(impulse, coefficients, true);
    std::cout << "Convolution Output for Impulse Input:" << std::endl;
    
    printf("Output without latency removal:\n");
    for (size_t i = 0; i < output_noLatency.size(); i++)
        printf("i=%lu: output=%.4f\n", i, output_noLatency[i]);

    printf("\nOutput with latency removal:\n");
    for (size_t i = 0; i < output_withLatency.size(); i++)
        printf("i=%lu: output=%.4f\n", i, output_withLatency[i]);

    return 0;
}
```

### CMakeLists.txt example

Suppose to have the project organized as follows:
```
.
├── CMakeLists.txt
├── lib
│   └── rawdsp-main
│       ├── CMakeLists.txt
│       ├── include
│       ├── python
│       ├── README.md
│       ├── run_all_test.sh
│       ├── src
│       └── tests
├── README.md
└── src
    └── main.cpp
```

To integrate the library in the project, you can start from the following example of  `CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.10)
project(Example)

# rawdsp library is written in C++20, so we need to set the standard for the whole project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the rawdsp library
add_library(rawdsp STATIC lib/rawdsp-main/include/rawdsp.hpp)
set_target_properties(rawdsp PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(rawdsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/rawdsp-main/include)

# Create the example executable and link it with the rawdsp library
add_executable(example src/main.cpp)
target_include_directories(example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(example PRIVATE rawdsp)
```



## GUI Data Visualizer

This repository includes a compact but powerful GUI application, [python/gui_data_visualizer.py](python/gui_data_visualizer.py#L1), which lets you open raw binary signal files and immediately explore them.
From a single window you can choose the sample `dtype` (int16/int32/float32/float64), toggle complex I/Q parsing with an optional I/Q swap, enter sampling rate and FFT sizes using simple math expressions (for example `10^6` or `2^10`), and interactively inspect time-domain traces, spectral magnitude and an optional waterfall display.
The tool provides keyboard shortcuts (Ctrl+O to open a file, Ctrl+U to update), pan controls, and supports both real and complex visualizations — making it ideal for quick inspection, debugging and live exploratory analysis of captured signals.

Run it with:

```bash
python3 python/gui_data_visualizer.py
```

## Bash script to run all test

The repository includes [run_all_test.sh](run_all_test.sh), a colorful batch runner that automatically discovers and executes every compiled test binary inside a given directory.

After building the project, run it against the `build/` folder:

```bash
# Make the script executable (once)
chmod +x run_all_test.sh

# Run all tests from the build directory
./run_all_test.sh build
```

If no directory is specified, the script defaults to the current directory (`.`).

The runner prints a colored header for each executable, reports **PASSED** / **FAILED** with the exit code and elapsed time, and finishes with a summary line showing the total pass/fail counts.

---

## Contribute

rawdsp is open source and wired for contributions. Read the [contributing guidelines](CONTRIBUTING.md), open a PR, break things responsibly.

- 📄 [MIT License](LICENSE)
- 🐛 [Issues](https://github.com/yourusername/rawdsp/issues)
- 📬 [daniele.giardino90@gmail.com](mailto:daniele.giardino90@gmail.com)

---

*If you need a GUI to do DSP, this isn't your library.*
