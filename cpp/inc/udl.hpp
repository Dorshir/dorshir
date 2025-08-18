#ifndef UDL_HPP
#define UDL_HPP

#include <cstddef>

namespace udl {

/* double sqrt */

constexpr double abs(double n)
{
    return n > 0 ? n : n * (-1.0);
}

constexpr double dsqrt_rec(double x, double prev, double num, double tolerance)
{
    return (abs(x - prev) <= tolerance) ? x : dsqrt_rec(0.5 * (x + num / x), x, num, tolerance);
}

constexpr double dsqrt(double num, double tolerance)
{
    if (num < 0) {
        return -1.0;
    }
    else if (num == 0) {
        return 0.0;
    }

    // Starting point(x_0) is (num / 2), continue untill abs(x - prev) <= tolerance
    return dsqrt_rec(num / 2.0, num, num, tolerance);
}



/* roman suffix */

constexpr int roman_to_int(char c)
{
    switch (c) {

    case 'M':
        return 1000;
    case 'C':
        return 100;
    case 'X':
        return 10;
    case 'I':
        return 1;
    case 'D':
        return 500;
    case 'L':
        return 50;
    case 'V':
        return 5;

    default:
        return 0;
    }
}

constexpr int roman_rec(const char* expr, int acc, size_t length)
{
    if (length == 0) {
        return acc;
    }

    auto curr_val = roman_to_int(expr[0]);
    auto next_val = length >= 1 ? roman_to_int(expr[1]) : 0;

    return curr_val < next_val ? roman_rec(expr + 1, acc - curr_val, length - 1)
            : roman_rec(expr + 1, acc + curr_val, length - 1);
}

constexpr int operator""_roman(const char* expr, size_t length)
{
    return roman_rec(expr, 0, length);
}

} // namespace udl

#endif // UDL_HPP
