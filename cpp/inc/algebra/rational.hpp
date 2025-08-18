#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#include <iostream> // for std::ostream

namespace algebra {

class Rational {
public:
    /**
     * @brief Constructs a Rational number with the given numerator and denominator.
     * 
     * @param numerator The numerator of the rational number (default is 0).
     * @param denominator The denominator of the rational number (default is 1).
     * @throw std::invalid_argument If the denominator is 0.
     */
    Rational(int numerator = 0, int denominator = 1);
	Rational(Rational const&) = default;
	Rational& operator=(Rational const&) = default;
    ~Rational() = default;

    /**
     * @brief Adds an integer to the rational number.
     * 
     * @param num The integer to add.
     * @return Reference to the updated Rational object.
     */
    Rational& add(int num);

    /**
     * @brief Adds another Rational to the current Rational.
     * 
     * @param other The Rational to add.
     * @return Reference to the updated Rational object.
     */
    Rational& add(Rational other);

    /**
     * @brief Multiplies the rational number by an integer.
     * 
     * @param num The integer to multiply by.
     * @return Reference to the updated Rational object.
     */
    Rational& mul(int num);

    /**
     * @brief Multiplies the rational number by another Rational.
     * 
     * @param other The Rational to multiply by.
     * @return Reference to the updated Rational object.
     */
    Rational& mul(Rational other);

    /**
     * @brief Simplifies the rational number by reducing it to its simplest form.
     * 
     * @return Reference to the updated Rational object.
     */
    Rational& reduce();

    /**
     * @brief Returns the value of the rational number as a double.
     * 
     * @return The floating-point representation of the rational number.
     */
    double value() const;

    /**
     * @brief Prints the rational number in the format "numerator/denominator".
     */
    void print() const;

    /**
     * @brief Returns the absolute value of the rational number.
     * 
     * @return A new Rational object representing the absolute value.
     */
    Rational abs() const;


    /**
     * @brief Prints the rational to the given output stream in a human-readable format.
     * @param os The output stream.
     */
    void print(std::ostream& os) const;

private:
    int m_numerator;
    int m_denominator;
};


/**
 * @brief Adds an integer to a Rational number.
 * 
 * @param ratio The Rational number to update.
 * @param num The integer to add.
 * @return Reference to the updated Rational object.
 */
Rational& add(Rational& ratio, int num);


/**
 * @brief Adds one Rational number to another.
 * 
 * @param first The first Rational number to update.
 * @param second The second Rational number to add.
 * @return Reference to the updated first Rational object.
 */
Rational& add(Rational& first, Rational second);


/**
 * @brief Multiplies a Rational number by an integer.
 * 
 * @param ratio The Rational number to update.
 * @param num The integer to multiply by.
 * @return Reference to the updated Rational object.
 */
Rational& mul(Rational& ratio, int num);


/**
 * @brief Multiplies two Rational numbers.
 * 
 * @param first The first Rational number to update.
 * @param second The second Rational number to multiply by.
 * @return Reference to the updated first Rational object.
 */
Rational mul(const Rational& first, Rational second);


/**
 * @brief Prints a Rational number in the format "numerator/denominator".
 * 
 * @param ratio The Rational number to print.
 */
void print(Rational ratio);


/**
 * @brief Overloaded output operator for Rational.
 * @param os The output stream.
 * @param ratio The Rational to write to the stream.
 * @return A reference to the output stream.
 *
 * Prints the Rational in a readable form (e.g. "3/2").
 */
std::ostream& operator<<(std::ostream& os, Rational const& ratio);


} // namespace algebra

#endif // RATIONAL_HPP
