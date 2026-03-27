/**
 * @file test_types.cpp
 * @author Daniele Giardino
 * @brief Test for custom complex types (c_int8_t, c_int16_t, c_int32_t, c_float_t), including arithmetic operations and printing results.
 */

#include "rawdsp.hpp"
#include <complex>

int main(int argc, char const *argv[])
{

    // Create an 8-bit integer with maximum value and a scale factor of 32
    c_int8_t x8(INT8_MAX, 32);
    int8_t a = 5;

    // Perform addition and print the result
    x8 = x8 + a;
    printf("Addition Result: (%i,%i)\n", x8.re, x8.im);

    // Perform subtraction and print the result
    x8 = x8 - 5;
    printf("Subtraction Result: (%i,%i)\n", x8.re, x8.im);

    // Perform multiplication and print the result
    x8 = x8 * 2;
    printf("Multiplication Result: (%i,%i)\n", x8.re, x8.im);

    // Perform division and print the result
    x8 = x8 / 2;
    printf("Division Result: (%i,%i)\n", x8.re, x8.im);

    // Print
    printf("Real=%i | Imag=%i\n", x8.re, x8.im);

    // Sizeof
    std::cout << "Size of c_int8_t: " << sizeof(c_int8_t) << " bytes" << std::endl;

    std::cout << "--------------------------------" << std::endl;

    // Create a 16-bit integer with maximum value and a scale factor of 1024
    c_int16_t x16(INT16_MAX, 1024);
    int16_t b = 10;

    // Perform subtraction with a regular integer and print the result
    x16 = x16 + b;
    printf("Addition Result: (%i,%i)\n", x16.re, x16.im);

    // Perform subtraction and print the result
    x16 = x16 - 50;
    printf("Subtraction Result: (%i,%i)\n", x16.re, x16.im);

    // Perform multiplication and print the result
    x16 = x16 * 2;
    printf("Multiplication Result: (%i,%i)\n", x16.re, x16.im);

    // Perform division and print the result
    x16 = x16 / 2;
    printf("Division Result: (%i,%i)\n", x16.re, x16.im);

    // Print
    x16.print();
    printf("Real=%i | Imag=%i\n", x16.re, x16.im);

    // Sizeof
    std::cout << "Size of c_int16_t: " << sizeof(c_int16_t) << " bytes" << std::endl;

    std::cout << "--------------------------------" << std::endl;

    // Create a 32-bit integer with maximum value and a scale factor of 2048
    c_int32_t x32(INT32_MAX, 2048);
    
    // Perform addition and print the result
    x32 = x32 + 1000;
    printf("Addition Result: (%i,%i)\n", x32.re, x32.im);

    // Perform subtraction and print the result
    x32 = x32 - 500;
    printf("Subtraction Result: (%i,%i)\n", x32.re, x32.im);

    // Perform multiplication and print the result
    x32 = x32 * 2;
    printf("Multiplication Result: (%i,%i)\n", x32.re, x32.im);

    // Perform division and print the result
    x32 = x32 / 2;
    printf("Division Result: (%i,%i)\n", x32.re, x32.im);

    // Print
    x32.print();
    printf("Real=%i | Imag=%i\n", x32.re, x32.im);

    // Sizeof
    std::cout << "Size of c_int32_t: " << sizeof(c_int32_t) << " bytes" << std::endl;

    std::cout << "--------------------------------" << std::endl;

    // Create a floating-point number with initial value 3.14 and a scale factor of 1.0
    std::complex<float> xf_std(3.14f, 1.0f);

    // Perform arithmetic and print results
    xf_std += 1.0f;
    printf("std::complex Addition Result: (%f,%f)\n", xf_std.real(), xf_std.imag());

    xf_std -= 0.5f;
    printf("std::complex Subtraction Result: (%f,%f)\n", xf_std.real(), xf_std.imag());

    xf_std *= 2.0f;
    printf("std::complex Multiplication Result: (%f,%f)\n", xf_std.real(), xf_std.imag());

    xf_std /= 2.0f;
    printf("std::complex Division Result: (%f,%f)\n", xf_std.real(), xf_std.imag());

    printf("std::complex Real=%f | Imag=%f\n", xf_std.real(), xf_std.imag());

    std::cout << "Size of std::complex<float>: " << sizeof(std::complex<float>) << " bytes" << std::endl;

    return 0;
}
