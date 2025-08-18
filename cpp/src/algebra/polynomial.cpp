#include "polynomial.hpp"
#include <algorithm> // std::copy
#include <cassert>   // assert
#include <iostream>  // std::ostream
#include <cmath>     // max


namespace algebra{

Polynomial::Polynomial(int degree)
: m_degree{degree}
, m_coefficients{new Rational[degree + 1]}
{
    assert(m_coefficients);
}


Polynomial::~Polynomial()
{
    delete[] m_coefficients;
}


Polynomial::Polynomial(const Rational* begin, const Rational* end)
: m_degree(static_cast<int>(end - begin - 1))
, m_coefficients(new Rational[m_degree + 1])
{
    assert(begin == nullptr || end == nullptr);
    assert(begin <= end);
    assert(m_coefficients);
    std::copy(begin, end, m_coefficients);
}


Polynomial::Polynomial(Polynomial const& other)
: m_degree{other.m_degree}
, m_coefficients{new Rational[other.m_degree + 1]}
{
    assert(m_coefficients);
    std::copy(other.m_coefficients, other.m_coefficients + m_degree + 1, m_coefficients);
}


Polynomial& Polynomial::operator=(Polynomial const& other)
{
    if (this != &other) {
		Polynomial copy{other};
		swap(copy);
	}
	return *this;
}


Rational& Polynomial::operator[](int index)
{
    assert(index >= 0 && index <= m_degree);
    return m_coefficients[index];
}


void Polynomial::print(std::ostream& os) const
{
    bool first = true;

    for (int i = m_degree; i >= 0; --i) {
        if (m_coefficients[i].value() != 0) {
            if (!first) {
                os << " " << (m_coefficients[i].value() > 0 ? "+ " : "- ");
            } else if (m_coefficients[i].value() < 0) {
                os << "-";
            }
            os << m_coefficients[i] << "x^" << i;
            first = false;
        }
    }

    if (first) {
        os << "0";
    }
}


double Polynomial::operator()(Rational x) const
{
    double result = 0.0;
    double power_of_x = std::pow(x.value(), m_degree);

    for (int i = m_degree; i >= 0; --i) {
        result += m_coefficients[i].value() * power_of_x;
        power_of_x /= x.value();
    }
    return result;
}


Polynomial& Polynomial::operator*=(int num)
{
    for (int i = 0; i <= m_degree; ++i) {
        m_coefficients[i] = algebra::mul(m_coefficients[i], num);
    }
    return *this;
}


Polynomial& Polynomial::operator*=(const Rational& num)
{
    for (int i = 0; i <= m_degree; ++i) {
        m_coefficients[i] = algebra::mul(m_coefficients[i], num);
    }
    return *this;
}


Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs)
{
    int max_degree = std::max(lhs.degree(), rhs.degree());
    Polynomial result(max_degree);

    for (int i = 0; i <= max_degree; ++i) {
        Rational tmp = (i <= lhs.degree() ? lhs[i] : Rational{0});
        algebra::add(tmp, (i <= rhs.degree() ? rhs[i] : Rational{0}));
        result[i] = tmp;
    }
    return result;
}


Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs)
{
    int degree = lhs.degree() + rhs.degree();
    Polynomial result(degree);

    for (int i = 0; i <= lhs.degree(); ++i) {
        for (int j = 0; j <= rhs.degree(); ++j) {
            result[i + j] = algebra::add(result[i + j], algebra::mul(lhs[i], rhs[j]));
        }
    }
    return result;
}


void Polynomial::swap(Polynomial& other)
{
    std::swap(m_coefficients, other.m_coefficients);
	std::swap(m_degree, other.m_degree);
}


const Rational& Polynomial::operator[](int index) const
{
    assert(index >= 0 && index <= m_degree);
    return m_coefficients[index];
}


int Polynomial::degree() const
{
    return m_degree;
}






/* Global Functions */

Polynomial derive(const Polynomial& poly)
{
    if (poly.degree() == 0) {
        return Polynomial();
    }

    Polynomial result(poly.degree() - 1);

    for (int i = 1; i <= poly.degree(); ++i) {
        result[i - 1] = algebra::mul(poly[i] ,Rational(i));
    }

    return result;
}


std::ostream& operator<<(std::ostream& os, const Polynomial& poly)
{
    poly.print(os);
    return os;
}

}