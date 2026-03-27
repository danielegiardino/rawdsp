/**
 * @file General.hpp
 * @author Daniele Giardino
 * @brief General utility functions and definitions for the rawdsp library.
 */

#ifndef LIB_RAWDSP_GENERAL_HPP
#define LIB_RAWDSP_GENERAL_HPP

#include <iostream>
#include <string>
#include <vector>

// For the plotXY function
#include <algorithm>
#include <iomanip>

namespace rawdsp
{
    // ANSI color codes for terminal output
    // The following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
    constexpr const char *RESET = "\033[0m";
    constexpr const char *BLACK = "\033[30m";              /* Black */
    constexpr const char *RED = "\033[31m";                /* Red */
    constexpr const char *GREEN = "\033[32m";              /* Green */
    constexpr const char *YELLOW = "\033[33m";             /* Yellow */
    constexpr const char *BLUE = "\033[34m";               /* Blue */
    constexpr const char *MAGENTA = "\033[35m";            /* Magenta */
    constexpr const char *CYAN = "\033[36m";               /* Cyan */
    constexpr const char *WHITE = "\033[37m";              /* White */
    constexpr const char *BOLDBLACK = "\033[1m\033[30m";   /* Bold Black */
    constexpr const char *BOLDRED = "\033[1m\033[31m";     /* Bold Red */
    constexpr const char *BOLDGREEN = "\033[1m\033[32m";   /* Bold Green */
    constexpr const char *BOLDYELLOW = "\033[1m\033[33m";  /* Bold Yellow */
    constexpr const char *BOLDBLUE = "\033[1m\033[34m";    /* Bold Blue */
    constexpr const char *BOLDMAGENTA = "\033[1m\033[35m"; /* Bold Magenta */
    constexpr const char *BOLDCYAN = "\033[1m\033[36m";    /* Bold Cyan */
    constexpr const char *BOLDWHITE = "\033[1m\033[37m";   /* Bold White */

    /****************************************************/
    // Functions to print
    /****************************************************/

    /**
     * Print a red text
     * @param a Real number
     * @param b Imag number
     * @return Complex Number
     */
    void printerr(std::string x)
    {
        printf(RED);
        printf("%s", x.c_str());
        printf(RESET);
    }

    /**
     * @brief Plot a simple ASCII graph of the given x and y data points in the terminal.
     * @param x The vector of x-axis values.
     * @param y The vector of y-axis values (must be the same size as x).
     * @param width The width of the plot in characters (default is 60).
     * @param height The height of the plot in characters (default is 20).
     */
    template <typename T>
        requires std::floating_point<T> || std::integral<T>
    void plotXY(const std::vector<T> &x, const std::vector<T> &y, int width = 60, int height = 20)
    {
        if (x.empty() || x.size() != y.size())
            return;

        auto [minX, maxX] = std::minmax_element(x.begin(), x.end());
        auto [minY, maxY] = std::minmax_element(y.begin(), y.end());

        float rangeX = (*maxX - *minX == 0) ? 1.0f : (*maxX - *minX);
        float rangeY = (*maxY - *minY == 0) ? 1.0f : (*maxY - *minY);

        std::vector<std::string> grid(height, std::string(width, ' '));

        for (size_t i = 0; i < x.size(); ++i)
        {
            int px = static_cast<int>((x[i] - *minX) / rangeX * (width - 1));
            int py = static_cast<int>((y[i] - *minY) / rangeY * (height - 1));
            grid[(height - 1) - py][px] = 'o'; // Data point marker
        }

        // --- Rendering with Labels ---
        std::cout << std::fixed << std::setprecision(2);

        for (int r = 0; r < height; ++r)
        {
            // Y-axis numbers (Top, Middle, Bottom)
            if (r == 0)
                std::cout << std::setw(8) << *maxY << " ┤";
            else if (r == height / 2)
                std::cout << std::setw(8) << (*maxY + *minY) / 2 << " ┤";
            else if (r == height - 1)
                std::cout << std::setw(8) << *minY << " ┤";
            else
                std::cout << "         │";

            std::cout << grid[r] << "│" << std::endl;
        }

        // X-axis line
        std::string horizontalLine;
        for (int i = 0; i < width; ++i)
        {
            horizontalLine += "─"; // Double quotes make this a string literal (safe)
        }
        std::cout << "         └" << horizontalLine << "┘" << std::endl;

        // X-axis numbers
        std::cout << "          " << *minX;
        int spaces = width - std::to_string((int)*minX).length() - std::to_string((int)*maxX).length() - 2;
        std::cout << std::string(std::max(0, spaces), ' ') << *maxX << std::endl;
    }

} // namespace rawdsp

#include "../src/General.cpp"

#endif // RAWDSP_GENERAL_HPP