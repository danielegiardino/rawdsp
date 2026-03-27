/**
 * @file rawdsp.hpp
 * @author Daniele Giardino
 * @brief Main header file for the rawdsp library, including all necessary components for DSP applications.
 */

#ifndef LIB_RAWDSP_HPP
#define LIB_RAWDSP_HPP

#include "FileIO.hpp"
#include "Filters.hpp"
#include "General.hpp"
#include "Primitive.hpp"
#include "Primitive_simd.hpp"
#include "SignalGen.hpp"
#include "Types.hpp"

using rawdsp::c_int8_t;
using rawdsp::c_int16_t;
using rawdsp::c_int32_t;
using rawdsp::c_float_t;
using rawdsp::c_double_t;
using rawdsp::c_long_double_t;

#endif // RAWDSP_HPP