/**
 * @file Types.hpp
 * @author Daniele Giardino
 * @brief This file contains the definition of the rawdsp namespace and the complex number classes.
 *
 * The rawdsp namespace provides a template class for representing complex numbers and specialized classes for different data types.
 *
 * The following classes are defined:
 * - complex<T>: A template class to represent complex numbers with real and imaginary components.
 * - c_int8_t: A specialized class of the template complex for int8_t type.
 * - c_int16_t: A specialized class of the template complex for int16_t type.
 * - c_int32_t: A specialized class of the template complex for int32_t type.
 * - c_float_t: A specialized class of the template complex for float type.
 *
 * Each class provides methods for basic arithmetic operations (addition, subtraction, multiplication, division),
 * as well as methods to compute the magnitude, phase, and conjugate of complex numbers.
 *
 * The specialized classes also include saturation control for their respective data types.
 *
 * @note The file is part of the dsp-kit library.
 *
 */

#ifndef LIB_RAWDSP_TYPES_H
#define LIB_RAWDSP_TYPES_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <complex>

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <concepts>

namespace rawdsp
{

    /**
     * @class c_intT_t
     * @brief A template class to represent complex numbers of type integer.
     *
     * This class provides a representation of complex numbers with real and imaginary components.
     *
     * @tparam T The type of the real and imaginary components (only integer types).
     */
    template <std::integral T> // Recognize types that are integral (e.g., int8_t, int16_t, int32_t, etc.)
    class c_intT_t
    {
    public:
        /**
           Constructs a complex number with default values
        */
        c_intT_t()
        {
            this->re = 0;
            this->im = 0;
        }

        /**
           Constructs a complex number defined by its components
           @param re Real component
           @param im Imag component
        */
        c_intT_t(T x_re, T x_im)
        {
            this->re = x_re;
            this->im = x_im;
        }

        /**
           Constructs a complex number defined by its components
           @param re Real component
           @param im Imag component
        */
        c_intT_t(T x)
        {
            this->re = x;
            this->im = 0;
        }

        /**
           Constructs a c_intT_t number defined by its magnitude and phase
           @param mag Magnitude
           @param phase Phase
        */
        c_intT_t(T mag, T phase, bool polar)
        {
            if (polar)
            {
                this->re = mag * cos(phase);
                this->im = mag * sin(phase);
            }
        }

        /**
         * @brief Returns the real part of the c_intT_t number.
         *
         * @return T The real part of the c_intT_t number.
         */
        T real() const
        {
            return this->re;
        }

        /**
         * @brief Returns the imaginary part of the c_intT_t number.
         *
         * @return T The imaginary part of the c_intT_t number.
         */
        T imag() const
        {
            return this->im;
        }

        /**
         * @brief Adds a real number to the c_intT_t number.
         *
         * @param x The real number to add.
         * @return c_intT_t8_t The sum.
         */
        c_intT_t operator+(const T x)
        {
            return c_intT_t(this->re + x, this->im);
        }

        /**
           Sum of two c_intT_t numbers
           @param x Second c_intT_t number
           @return the sum
        */
        c_intT_t operator+(c_intT_t<T> x)
        {
            return c_intT_t(this->re + x.re, this->im + x.im);
        }

        /**
         * @brief Subtracts a real number from the c_intT_t number.
         *
         * @param x The real number to subtract.
         * @return The difference.
         */
        c_intT_t operator-(T x)
        {
            return c_intT_t(this->re - x, this->im);
        }

        /**
           Difference of two c_intT_t numbers
           @param x Second c_intT_t number
           @return the difference
        */

        c_intT_t operator-(c_intT_t x)
        {
            return c_intT_t(this->re - x.re, this->im - x.im);
        }

        /**
            Product of a c_intT_t number with a real number
            @param x Real number
            @return the product
        */
        c_intT_t operator*(T x)
        {
            return c_intT_t(this->re * x, this->im * x);
        }

        /**
            Product of two complex numbers
            @param x Second complex number
            @return the product
         */
        c_intT_t operator*(c_intT_t x)
        {
            return c_intT_t(this->re * x.re - this->im * x.im, this->re * x.im + this->im * x.re);
        }

        /**
            Division of a complex number by a real number
            @param x Real number
            @return the division
        */
        c_intT_t operator/(T x)
        {
            if (x == 0)
            {
                throw std::overflow_error("Division by zero");
            }
            return c_intT_t(this->re / x, this->im / x);
        }

        /**
           Division of two complex numbers
           @param x Second complex number
           @return the division
        */
        c_intT_t operator/(c_intT_t<T> x)
        {
            T den = x.re * x.re + x.im * x.im;
            return c_intT_t((this->re * x.re + this->im * x.im) / den, (this->im * x.re - this->re * x.im) / den);
        }

        /**
         * @brief Adds a real number to the complex number.
         *
         * @param x The real number to add.
         * @return The sum.
         */
        c_intT_t operator+=(T x)
        {
            this->re += x;
            return *this;
        }

        /**
           Sum of two complex numbers
           @param x Second complex number
           @return The sum
        */
        c_intT_t operator+=(c_intT_t<T> x)
        {
            this->re += x.re;
            this->im += x.im;
            return *this;
        }

        /**
         * @brief Subtracts a real number from the complex number.
         *
         * @param x The real number to subtract.
         * @return The difference.
         */
        c_intT_t operator-=(T x)
        {
            this->re -= x;
            return *this;
        }

        /**
           Difference of two complex numbers
           @param x Second complex number
           @return The difference
        */
        c_intT_t operator-=(c_intT_t<T> x)
        {
            this->re -= x.re;
            this->im -= x.im;
            return *this;
        }

        /**
            Product of a complex number with a real number
            @param x Real number
            @return The product
        */
        c_intT_t operator*=(T x)
        {
            this->re *= x;
            this->im *= x;
            return *this;
        }

        /**
            Product of two complex numbers
            @param x Second complex number
            @return The product
         */
        c_intT_t operator*=(c_intT_t<T> x)
        {
            T re_temp = this->re * x.re - this->im * x.im;
            T im_temp = this->re * x.im + this->im * x.re;
            this->re = re_temp;
            this->im = im_temp;
            return *this;
        }

        /**
         * @brief Overloads the << operator to print the complex number.
         *
         * @param os The output stream.
         * @param x The complex number to print.
         * @return std::ostream& The output stream.
         */
        friend std::ostream &operator<<(std::ostream &os, const c_intT_t &x)
        {
            os << "(" << x.re << ", " << x.im << ")";
            return os;
        }

        /**
         * @brief Prints the complex number to the standard output.
         */
        void print() const
        {
            std::cout << "(" << re << ", " << im << ")" << std::endl;
        }

        /**
         * @brief Computes the absolute value (magnitude) of the complex number.
         *
         * @return float The absolute value of the complex number.
         */
        float abs() const
        {
            return sqrt((float)re * (float)re + (float)im * (float)im);
        }

        /**
         * @brief Computes the magnitude of the complex number.
         *
         * @return float The magnitude of the complex number.
         */
        float magnitude() const
        {
            return sqrt((float)re * (float)re + (float)im * (float)im);
        }

        /**
         * @brief Computes the phase of the complex number.
         *
         * @return float The phase of the complex number in radians.
         */
        float phase() const
        {
            return atan2((float)im, (float)re);
        }

        /**
         * @brief Conjugates the complex number.
         *
         * @return The conjugated complex number.
         */
        c_intT_t conjugate() const
        {
            return c_intT_t(re, -im);
        }

        /**
         * @brief Swap real and imag part
         *
         * @return The swap complex number
         */
        c_intT_t swap(c_intT_t<T> x) const
        {
            return c_intT_t(x.im, x.re);
        }

        T re;
        T im;
    };

    /**
     * @class c_int8_t
     * @brief A specialized class of the template complex for int8_t type.
     */
    class c_int8_t : public c_intT_t<int8_t>
    {
    public:
        using c_intT_t<int8_t>::c_intT_t;

        c_int8_t operator=(const c_intT_t<int8_t> &x);

        /**
         * @brief Multiplies the complex number by a real number with saturation control.
         *
         * @param x The real number to multiply.
         * @return c_int8_t The product with saturation control.
         */
        c_int8_t mul_sat(int8_t x) const;

        /**
         * @brief Multiplies two complex numbers with saturation control.
         *
         * @param other The other complex number to multiply.
         * @return c_int8_t The product with saturation control.
         */
        c_int8_t mul_sat(const c_int8_t &other) const;

        /**
         * @brief Divides the complex number by a real number with saturation control.
         *
         * @param x The real number to divide by.
         * @return c_int8_t The quotient with saturation control.
         * @throws std::overflow_error if division by zero is attempted.
         */
        c_int8_t div_sat(int8_t x) const;

        /**
         * @brief Divides two complex numbers with saturation control.
         *
         * @param other The other complex number to divide by.
         * @return c_int8_t The quotient with saturation control.
         */
        c_int8_t div_sat(const c_int8_t &other) const;
    };

    /**
     * @class c_int16_t
     * @brief A specialized class of the template complex for int16_t type.
     */
    class c_int16_t : public c_intT_t<int16_t>
    {
    public:
        using c_intT_t<int16_t>::c_intT_t;

        /**
         * @brief Assignment operator for c_int16_t.
         *
         * @param x The complex number to assign from.
         * @return c_int16_t The assigned complex number.
         */
        c_int16_t operator=(const c_intT_t<int16_t> &x);

        /**
         * @brief Multiplies the complex number by a real number with saturation control.
         *
         * @param x The real number to multiply.
         * @return c_int16_t The product with saturation control.
         */
        c_int16_t mul_sat(int16_t x) const;

        /**
         * @brief Multiplies two complex numbers with saturation control.
         *
         * @param other The other complex number to multiply.
         * @return c_int16_t The product with saturation control.
         */
        c_int16_t mul_sat(const c_int16_t &other) const;

        /**
         * @brief Divides the complex number by a real number with saturation control.
         *
         * @param x The real number to divide by.
         * @return c_int16_t The quotient with saturation control.
         * @throws std::overflow_error if division by zero is attempted.
         */
        c_int16_t div_sat(int16_t x) const;

        /**
         * @brief Divides two complex numbers with saturation control.
         *
         * @param other The other complex number to divide by.
         * @return c_int16_t The quotient with saturation control.
         */
        c_int16_t div_sat(const c_int16_t &other) const;
    };

    /**
     * @class c_int32_t
     * @brief A specialized class of the template complex for int32_t type.
     */
    class c_int32_t : public c_intT_t<int32_t>
    {
    public:
        using c_intT_t<int32_t>::c_intT_t;

        /**
         * @brief Assignment operator for c_int32_t.
         *
         * @param x The complex number to assign from.
         * @return c_int32_t The assigned complex number.
         */
        c_int32_t operator=(const c_intT_t<int32_t> &x);

        /**
         * @brief Multiplies the complex number by a real number with saturation control.
         *
         * @param x The real number to multiply.
         * @return c_int32_t The product with saturation control.
         */
        c_int32_t mul_sat(int32_t x) const;

        /**
         * @brief Multiplies two complex numbers with saturation control.
         *
         * @param other The other complex number to multiply.
         * @return c_int32_t The product with saturation control.
         */
        c_int32_t mul_sat(const c_int32_t &other) const;

        /**
         * @brief Divides the complex number by a real number with saturation control.
         *
         * @param x The real number to divide by.
         * @return c_int32_t The quotient with saturation control.
         * @throws std::overflow_error if division by zero is attempted.
         */
        c_int32_t div_sat(int32_t x) const;

        /**
         * @brief Divides two complex numbers with saturation control.
         *
         * @param other The other complex number to divide by.
         * @return c_int32_t The quotient with saturation control.
         */
        c_int32_t div_sat(const c_int32_t &other) const;
    };

    /**
     * @class c_int64_t
     * @brief A specialized class of the template complex for int64_t type.
     */
    class c_int64_t : public c_intT_t<int64_t>
    {
    public:
        using c_intT_t<int64_t>::c_intT_t;

        /**
         * @brief Assignment operator for c_int64_t.
         *
         * @param x The complex number to assign from.
         * @return c_int64_t The assigned complex number.
         */
        c_int64_t operator=(const c_intT_t<int64_t> &x);
    };

    /**
     * @brief Type aliases for complex numbers of different data types.
     */
    // Complex type aliases for floating point types
    using c_float_t = std::complex<float>;
    using c_double_t = std::complex<double>;
    using c_long_double_t = std::complex<long double>;

    /**
     * @brief Concepts for type constraints in rawdsp functions.
     */
    template <typename T>
    concept RealOrComplex = 
        std::floating_point<T> || 
        requires { typename T::value_type; requires std::floating_point<typename T::value_type>; };
    template <typename T>
    concept onlyComplex = 
        requires { typename T::value_type; requires std::floating_point<typename T::value_type>; };

    /**
     * @brief Type alias for the default floating-point type used in rawdsp functions.
      * 
      * This type alias allows users to easily change the default floating-point type used in rawdsp functions by modifying this single definition.
      * By default, it is set to float, but it can be changed to double or long double if higher precision is required.
      */
    using T_fun = double;

}
#include "../src/Types.cpp"

#endif // LIB_RAWDSP_TYPES_H
