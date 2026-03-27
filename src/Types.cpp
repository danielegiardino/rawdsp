/**
 * @file Types.cpp
 * @author Daniele Giardino
 * @brief Implementation of custom complex types for rawdsp.
 */

#include "Types.hpp"

namespace rawdsp
{
    // ========================================================================
    // c_int8_t
    // ========================================================================
    c_int8_t c_int8_t::operator=(const c_intT_t<int8_t> &x)
    {
        if (this != &x)
        {
            this->re = static_cast<int8_t>(x.re);
            this->im = static_cast<int8_t>(x.im);
        }
        return *this;
    }

    c_int8_t c_int8_t::mul_sat(int8_t x) const
    {
        int16_t real_part = static_cast<int16_t>(this->re) * static_cast<int16_t>(x);
        int16_t imag_part = static_cast<int16_t>(this->im) * static_cast<int16_t>(x);

        if (real_part > INT8_MAX)
            real_part = INT8_MAX;
        else if (real_part < INT8_MIN)
            real_part = INT8_MIN;

        if (imag_part > INT8_MAX)
            imag_part = INT8_MAX;
        else if (imag_part < INT8_MIN)
            imag_part = INT8_MIN;

        return c_int8_t(static_cast<int8_t>(real_part), static_cast<int8_t>(imag_part));
    }

    c_int8_t c_int8_t::mul_sat(const c_int8_t &other) const
    {
        int16_t real_part = static_cast<int16_t>(this->re) * static_cast<int16_t>(other.re) - static_cast<int16_t>(this->im) * static_cast<int16_t>(other.im);
        int16_t imag_part = static_cast<int16_t>(this->re) * static_cast<int16_t>(other.im) + static_cast<int16_t>(this->im) * static_cast<int16_t>(other.re);

        if (real_part > INT8_MAX)
            real_part = INT8_MAX;
        else if (real_part < INT8_MIN)
            real_part = INT8_MIN;

        if (imag_part > INT8_MAX)
            imag_part = INT8_MAX;
        else if (imag_part < INT8_MIN)
            imag_part = INT8_MIN;

        return c_int8_t(static_cast<int8_t>(real_part), static_cast<int8_t>(imag_part));
    }

    c_int8_t c_int8_t::div_sat(int8_t x) const
    {
        if (x == 0)
        {
            throw std::overflow_error("Division by zero");
        }

        int16_t real_part = static_cast<int16_t>(this->re) / static_cast<int16_t>(x);
        int16_t imag_part = static_cast<int16_t>(this->im) / static_cast<int16_t>(x);

        if (real_part > INT8_MAX)
            real_part = INT8_MAX;
        else if (real_part < INT8_MIN)
            real_part = INT8_MIN;

        if (imag_part > INT8_MAX)
            imag_part = INT8_MAX;
        else if (imag_part < INT8_MIN)
            imag_part = INT8_MIN;

        return c_int8_t(static_cast<int8_t>(real_part), static_cast<int8_t>(imag_part));
    }

    c_int8_t c_int8_t::div_sat(const c_int8_t &other) const
    {
        int16_t den = static_cast<int16_t>(other.re) * static_cast<int16_t>(other.re) + static_cast<int16_t>(other.im) * static_cast<int16_t>(other.im);
        int16_t real_part = (static_cast<int16_t>(this->re) * static_cast<int16_t>(other.re) + static_cast<int16_t>(this->im) * static_cast<int16_t>(other.im)) / den;
        int16_t imag_part = (static_cast<int16_t>(this->im) * static_cast<int16_t>(other.re) - static_cast<int16_t>(this->re) * static_cast<int16_t>(other.im)) / den;

        if (real_part > INT8_MAX)
            real_part = INT8_MAX;
        else if (real_part < INT8_MIN)
            real_part = INT8_MIN;

        if (imag_part > INT8_MAX)
            imag_part = INT8_MAX;
        else if (imag_part < INT8_MIN)
            imag_part = INT8_MIN;

        return c_int8_t(static_cast<int8_t>(real_part), static_cast<int8_t>(imag_part));
    }

    // ========================================================================
    // c_int16_t
    // ========================================================================

    c_int16_t c_int16_t::operator=(const c_intT_t<int16_t> &x)
    {
        if (this != &x)
        {
            this->re = static_cast<int16_t>(x.re);
            this->im = static_cast<int16_t>(x.im);
        }
        return *this;
    }

    c_int16_t c_int16_t::mul_sat(int16_t x) const
    {
        int32_t real_part = static_cast<int32_t>(this->re) * static_cast<int32_t>(x);
        int32_t imag_part = static_cast<int32_t>(this->im) * static_cast<int32_t>(x);

        if (real_part > INT16_MAX)
            real_part = INT16_MAX;
        else if (real_part < INT16_MIN)
            real_part = INT16_MIN;

        if (imag_part > INT16_MAX)
            imag_part = INT16_MAX;
        else if (imag_part < INT16_MIN)
            imag_part = INT16_MIN;

        return c_int16_t(static_cast<int16_t>(real_part), static_cast<int16_t>(imag_part));
    }

    c_int16_t c_int16_t::mul_sat(const c_int16_t &other) const
    {
        int32_t real_part = static_cast<int32_t>(this->re) * static_cast<int32_t>(other.re) - static_cast<int32_t>(this->im) * static_cast<int32_t>(other.im);
        int32_t imag_part = static_cast<int32_t>(this->re) * static_cast<int32_t>(other.im) + static_cast<int32_t>(this->im) * static_cast<int32_t>(other.re);

        if (real_part > INT16_MAX)
            real_part = INT16_MAX;
        else if (real_part < INT16_MIN)
            real_part = INT16_MIN;

        if (imag_part > INT16_MAX)
            imag_part = INT16_MAX;
        else if (imag_part < INT16_MIN)
            imag_part = INT16_MIN;

        return c_int16_t(static_cast<int16_t>(real_part), static_cast<int16_t>(imag_part));
    }

    c_int16_t c_int16_t::div_sat(int16_t x) const
    {
        if (x == 0)
        {
            throw std::overflow_error("Division by zero");
        }

        int32_t real_part = static_cast<int32_t>(this->re) / static_cast<int32_t>(x);
        int32_t imag_part = static_cast<int32_t>(this->im) / static_cast<int32_t>(x);

        if (real_part > INT16_MAX)
            real_part = INT16_MAX;
        else if (real_part < INT16_MIN)
            real_part = INT16_MIN;

        if (imag_part > INT16_MAX)
            imag_part = INT16_MAX;
        else if (imag_part < INT16_MIN)
            imag_part = INT16_MIN;

        return c_int16_t(static_cast<int16_t>(real_part), static_cast<int16_t>(imag_part));
    }

    c_int16_t c_int16_t::div_sat(const c_int16_t &other) const
    {
        int32_t den = static_cast<int32_t>(other.re) * static_cast<int32_t>(other.re) + static_cast<int32_t>(other.im) * static_cast<int32_t>(other.im);
        int32_t real_part = (static_cast<int32_t>(this->re) * static_cast<int32_t>(other.re) + static_cast<int32_t>(this->im) * static_cast<int32_t>(other.im)) / den;
        int32_t imag_part = (static_cast<int32_t>(this->im) * static_cast<int32_t>(other.re) - static_cast<int32_t>(this->re) * static_cast<int32_t>(other.im)) / den;

        if (real_part > INT16_MAX)
            real_part = INT16_MAX;
        else if (real_part < INT16_MIN)
            real_part = INT16_MIN;

        if (imag_part > INT16_MAX)
            imag_part = INT16_MAX;
        else if (imag_part < INT16_MIN)
            imag_part = INT16_MIN;

        return c_int16_t(static_cast<int16_t>(real_part), static_cast<int16_t>(imag_part));
    }

    // ========================================================================
    // c_int32_t
    // ========================================================================

    c_int32_t c_int32_t::operator=(const c_intT_t<int32_t> &x)
    {
        if (this != &x)
        {
            this->re = static_cast<int32_t>(x.re);
            this->im = static_cast<int32_t>(x.im);
        }
        return *this;
    }

    c_int32_t c_int32_t::mul_sat(int32_t x) const
    {
        int64_t real_part = static_cast<int64_t>(this->re) * static_cast<int64_t>(x);
        int64_t imag_part = static_cast<int64_t>(this->im) * static_cast<int64_t>(x);

        if (real_part > INT32_MAX)
            real_part = INT32_MAX;
        else if (real_part < INT32_MIN)
            real_part = INT32_MIN;

        if (imag_part > INT32_MAX)
            imag_part = INT32_MAX;
        else if (imag_part < INT32_MIN)
            imag_part = INT32_MIN;

        return c_int32_t(static_cast<int32_t>(real_part), static_cast<int32_t>(imag_part));
    }

    c_int32_t c_int32_t::mul_sat(const c_int32_t &other) const
    {
        int64_t real_part = static_cast<int64_t>(this->re) * static_cast<int64_t>(other.re) - static_cast<int64_t>(this->im) * static_cast<int64_t>(other.im);
        int64_t imag_part = static_cast<int64_t>(this->re) * static_cast<int64_t>(other.im) + static_cast<int64_t>(this->im) * static_cast<int64_t>(other.re);

        if (real_part > INT32_MAX)
            real_part = INT32_MAX;
        else if (real_part < INT32_MIN)
            real_part = INT32_MIN;

        if (imag_part > INT32_MAX)
            imag_part = INT32_MAX;
        else if (imag_part < INT32_MIN)
            imag_part = INT32_MIN;

        return c_int32_t(static_cast<int32_t>(real_part), static_cast<int32_t>(imag_part));
    }

    c_int32_t c_int32_t::div_sat(int32_t x) const
    {
        if (x == 0)
        {
            throw std::overflow_error("Division by zero");
        }

        int64_t real_part = static_cast<int64_t>(this->re) / static_cast<int64_t>(x);
        int64_t imag_part = static_cast<int64_t>(this->im) / static_cast<int64_t>(x);

        if (real_part > INT32_MAX)
            real_part = INT32_MAX;
        else if (real_part < INT32_MIN)
            real_part = INT32_MIN;

        if (imag_part > INT32_MAX)
            imag_part = INT32_MAX;
        else if (imag_part < INT32_MIN)
            imag_part = INT32_MIN;

        return c_int32_t(static_cast<int32_t>(real_part), static_cast<int32_t>(imag_part));
    }

    c_int32_t c_int32_t::div_sat(const c_int32_t &other) const
    {
        int64_t den = static_cast<int64_t>(other.re) * static_cast<int64_t>(other.re) + static_cast<int64_t>(other.im) * static_cast<int64_t>(other.im);
        int64_t real_part = (static_cast<int64_t>(this->re) * static_cast<int64_t>(other.re) + static_cast<int64_t>(this->im) * static_cast<int64_t>(other.im)) / den;
        int64_t imag_part = (static_cast<int64_t>(this->im) * static_cast<int64_t>(other.re) - static_cast<int64_t>(this->re) * static_cast<int64_t>(other.im)) / den;

        if (real_part > INT32_MAX)
            real_part = INT32_MAX;
        else if (real_part < INT32_MIN)
            real_part = INT32_MIN;

        if (imag_part > INT32_MAX)
            imag_part = INT32_MAX;
        else if (imag_part < INT32_MIN)
            imag_part = INT32_MIN;

        return c_int32_t(static_cast<int32_t>(real_part), static_cast<int32_t>(imag_part));
    }

    // ========================================================================
    // c_int64_t
    // ========================================================================

    c_int64_t c_int64_t::operator=(const c_intT_t<int64_t> &x)
    {
        if (this != &x)
        {
            this->re = static_cast<int64_t>(x.re);
            this->im = static_cast<int64_t>(x.im);
        }
        return *this;
    }

    // // ========================================================================
    // // c_float_t
    // // ========================================================================

    // c_float_t c_float_t::operator=(const c_intT_t<float> &x)
    // {
    //     if (this != &x)
    //     {
    //         this->re = static_cast<float>(x.re);
    //         this->im = static_cast<float>(x.im);
    //     }
    //     return *this;
    // }

    // c_float_t c_float_t::operator*(const float &other) const
    // {
    //     float real_part = this->re * other;
    //     float imag_part = this->im * other;

    //     return c_float_t(real_part, imag_part);
    // }

    // c_float_t c_float_t::operator*(const c_float_t &other) const
    // {
    //     float real_part = this->re * other.re - this->im * other.im;
    //     float imag_part = this->re * other.im + this->im * other.re;

    //     return c_float_t(real_part, imag_part);
    // }

    // c_float_t c_float_t::conjugate() const
    // {
    //     return c_float_t(this->re, -this->im);
    // }

    // // ========================================================================
    // // c_double_t
    // // ========================================================================

    // c_double_t c_double_t::operator=(const complex<double> &x)
    // {
    //     if (this != &x)
    //     {
    //         this->re = static_cast<double>(x.re);
    //         this->im = static_cast<double>(x.im);
    //     }
    //     return *this;
    // }

    // c_double_t c_double_t::operator*(const double &other) const
    // {
    //     double real_part = this->re * other;
    //     double imag_part = this->im * other;

    //     return c_double_t(real_part, imag_part);
    // }

    // c_double_t c_double_t::operator*(const c_double_t &other) const
    // {
    //     double real_part = this->re * other.re - this->im * other.im;
    //     double imag_part = this->re * other.im + this->im * other.re;

    //     return c_double_t(real_part, imag_part);
    // }

    // c_double_t c_double_t::conjugate() const
    // {
    //     return c_double_t(this->re, -this->im);
    // }

    // double c_double_t::abs() const
    // {
    //     return sqrt(this->re * this->re + this->im * this->im);
    // }

    // // ========================================================================
    // // c_long_double_t
    // // ========================================================================

    // c_long_double_t c_long_double_t::operator=(const complex<long double> &x)
    // {
    //     if (this != &x)
    //     {
    //         this->re = static_cast<long double>(x.re);
    //         this->im = static_cast<long double>(x.im);
    //     }
    //     return *this;
    // }

    // c_long_double_t c_long_double_t::operator*(const long double &other) const
    // {
    //     long double real_part = this->re * other;
    //     long double imag_part = this->im * other;

    //     return c_long_double_t(real_part, imag_part);
    // }

    // c_long_double_t c_long_double_t::operator*(const c_long_double_t &other) const
    // {
    //     long double real_part = this->re * other.re - this->im * other.im;
    //     long double imag_part = this->re * other.im + this->im * other.re;

    //     return c_long_double_t(real_part, imag_part);
    // }

    // c_long_double_t c_long_double_t::conjugate() const
    // {
    //     return c_long_double_t(this->re, -this->im);
    // }

    // long double c_long_double_t::abs() const
    // {
    //     return sqrtl(this->re * this->re + this->im * this->im);
    // }

    
}