#include "rational.hpp"
#include <numeric>      // gcd, lcm
#include <stdexcept>    // invalid_argument, overflow_error
#include <iostream>     // cout, endl, ostream
#include <climits>      // INT_MAX

namespace algebra {

Rational::Rational(int numerator, int denominator)
: m_numerator{numerator}
, m_denominator{denominator}
{
    if (denominator == 0) {
        throw std::invalid_argument("Denominator cannot be zero");
    }
    reduce();
}

double Rational::value() const
{ 
    return static_cast<double>(m_numerator)/m_denominator;
}

Rational& Rational::reduce()
{
    int gcd = std::gcd(m_numerator, m_denominator);
    m_numerator /= gcd;
    m_denominator /= gcd;

    if (m_denominator < 0) {
        m_numerator = -m_numerator;
        m_denominator = -m_denominator;
    }

    return *this;
}

void Rational::print() const
{
    std::cout << m_numerator << '/' << m_denominator << std::endl;
}

Rational& Rational::add(int num)
{
    long long numerator = (long long)m_numerator + (long long)num * m_denominator;

    if (numerator > INT_MAX || numerator < INT_MIN) {
        throw std::overflow_error("Addition overflow in Rational::add");
    }

    m_numerator = (int)numerator;
    return reduce();
}

Rational& Rational::add(Rational other)
{
    if ((long long)m_denominator * other.m_denominator > INT_MAX) {
        throw std::overflow_error("Denominator overflow in Rational::add(Rational)");
    }

    int lcm = std::lcm(m_denominator, other.m_denominator);

    long long numerator = (long long)m_numerator * (lcm / m_denominator) +
                          (long long)other.m_numerator * (lcm / other.m_denominator);

    if (numerator > INT_MAX || numerator < INT_MIN) {
        throw std::overflow_error("Numerator overflow in Rational::add(Rational)");
    }

    m_numerator = (int)numerator;
    m_denominator = lcm;
    return reduce();
}

Rational& Rational::mul(int num)
{
    long long numerator = (long long)m_numerator * num;

    if (numerator > INT_MAX || numerator < INT_MIN) {
        throw std::overflow_error("Multiplication overflow in Rational::mul");
    }

    m_numerator = (int)numerator;
    return reduce();
}

Rational& Rational::mul(Rational other)
{
    long long numerator = (long long)m_numerator * other.m_numerator;
    long long denominator = (long long)m_denominator * other.m_denominator;

    if (numerator > INT_MAX || numerator < INT_MIN || denominator > INT_MAX || denominator < INT_MIN) {
        throw std::overflow_error("Multiplication overflow in Rational::mul(Rational)");
    }

    m_numerator = (int)numerator;
    m_denominator = (int)denominator;
    return reduce();
}

Rational Rational::abs() const {
    return Rational{
        m_numerator < 0 ? -m_numerator : m_numerator,
        m_denominator
    };
}

void Rational::print(std::ostream& os) const
{
    os << m_numerator << "/" << m_denominator;
}

/* Global Functions */

Rational& add(Rational& ratio, int num) 
{
    return ratio.add(num);
}

Rational& add(Rational& first, Rational second) 
{
    return first.add(second);
}

Rational& mul(Rational& ratio, int num) 
{
    return ratio.mul(num);
}

Rational mul(const Rational& first, Rational second) 
{
    Rational tmp = first;
    tmp.mul(second);
    return tmp;
}

void print(Rational ratio)
{
    ratio.print();
}

std::ostream& operator<<(std::ostream& os, Rational const& ratio)
{
    ratio.print(os);
    return os;
}


} // namespace algebra
