#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include "rational.hpp"
#include <cstddef>  // size_t
#include <iostream> // std::ostream

namespace algebra {

class Polynomial
{
public:

    explicit Polynomial(int degree = 0);
    ~Polynomial();
    Polynomial(Polynomial const& other);
    Polynomial& operator=(Polynomial const& other);


    /**
     * @brief Constructs a polynomial from a range of Rational coefficients.
     * @param begin Pointer to the first element of the coefficient range.
     * @param end Pointer to one past the last element of the coefficient range.
     */
    Polynomial(const Rational* begin, const Rational* end);


    /**
     * @brief Returns a reference to the coefficient at the given index.
     * @param index The 0-based index of the coefficient.
     * @return A modifiable reference to the Rational coefficient.
     */
    Rational& operator[](int index);


    /**
     * @brief Returns a const reference to the coefficient at the given index.
     * @param index The 0-based index of the coefficient.
     * @return A const reference to the Rational coefficient.
     */
    const Rational& operator[](int index) const;


    /**
     * @brief Evaluates the polynomial at a given Rational value x.
     * @param x The value (of type Rational) to substitute into the polynomial.
     * @return The result of the evaluation as a double.
     */
    double operator()(Rational x) const;


    /**
     * @brief Prints the polynomial to the given output stream in a human-readable format.
     * @param os The output stream.
     */
    void print(std::ostream& os) const;


    /**
     * @brief Exchanges the contents of this polynomial with another.
     * @param other The polynomial to swap with.
     */
    void swap(Polynomial& other);


    /**
     * @brief Provides the degree of the polynomial.
     * @return The degree (highest exponent) of the polynomial.
     */
    int degree() const;


    /**
     * @brief Multiplies this polynomial by an integer in place.
     * @param num The integer factor.
     * @return A reference to this Polynomial after scaling.
     */
    Polynomial& operator*=(int num);


    /**
     * @brief Multiplies this polynomial by a Rational in place.
     * @param num The Rational factor.
     * @return A reference to this Polynomial after scaling.
     */
    Polynomial& operator*=(const Rational& num);


private:
    int m_degree;
    Rational* m_coefficients;
};


/**
 * @brief Overloaded output operator for Polynomial.
 * @param os The output stream.
 * @param poly The Polynomial to write to the stream.
 * @return A reference to the output stream.
 *
 * Prints the polynomial in a readable form (e.g. "3/2x^2 - 1/4").
 */
std::ostream& operator<<(std::ostream& os, Polynomial const& poly);


/**
 * @brief Computes the derivative of a Polynomial.
 * @param other The polynomial to differentiate.
 * @return A new Polynomial that is the derivative of 'other'.
 *
 * If 'other' has degree d, then the result has degree d - 1 (unless other is constant, in which case the result is 0).
 */
Polynomial derive(const Polynomial& other);


/**
 * @brief Computes the sum of two Polynomials.
 * @param lhs The left polynomial.
 * @param rhs The right polynomial.
 * @return A new Polynomial representing (lhs + rhs).
 *
 * The result will have degree max(lhs degree, rhs degree).
 */
Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs);


/**
 * @brief Computes the product of two Polynomials.
 * @param lhs The left polynomial.
 * @param rhs The right polynomial.
 * @return A new Polynomial representing (lhs * rhs).
 *
 * The result will have degree lhs.degree() + rhs.degree().
 */
Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs);

} // namespace algebra

#endif // POLYNOMIAL_HPP
