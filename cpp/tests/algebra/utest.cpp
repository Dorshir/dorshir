#include "mu_test.h"
#include "rational.hpp"
#include <climits>      // INT_MAX

BEGIN_TEST(constructor_with_valid_input)
    algebra::Rational r1(3, 4);
    ASSERT_EQUAL(r1.value(), 0.75);

    algebra::Rational r2(-3, 4);
    ASSERT_EQUAL(r2.value(), -0.75);

    algebra::Rational r3(3, -4);
    ASSERT_EQUAL(r3.value(), -0.75);

    algebra::Rational r4(-3, -4);
    ASSERT_EQUAL(r4.value(), 0.75);
END_TEST

BEGIN_TEST(constructor_with_invalid_denominator)
    bool exception_caught = false;
    try {
        algebra::Rational r(1, 0);
    } catch (const std::invalid_argument&) {
        exception_caught = true;
    }
    ASSERT_THAT(exception_caught);
END_TEST

BEGIN_TEST(add_integer_to_rational)
    algebra::Rational r(3, 4);
    r.add(1);
    ASSERT_EQUAL(r.value(), 1.75);

    r.add(-2);
    ASSERT_EQUAL(r.value(), -0.25);
END_TEST

BEGIN_TEST(add_two_rationals)
    algebra::Rational r1(3, 4);
    algebra::Rational r2(1, 4);
    r1.add(r2);
    ASSERT_EQUAL(r1.value(), 1.0);

    r2 = algebra::Rational(-1, 4);
    r1.add(r2);
    ASSERT_EQUAL(r1.value(), 0.75);
END_TEST

BEGIN_TEST(multiply_rational_by_integer)
    algebra::Rational r(3, 4);
    r.mul(2);
    ASSERT_EQUAL(r.value(), 1.5);

    r.mul(-1);
    ASSERT_EQUAL(r.value(), -1.5);
END_TEST

BEGIN_TEST(multiply_two_rationals)
    algebra::Rational r1(3, 4);
    algebra::Rational r2(2, 3);
    r1.mul(r2);
    ASSERT_EQUAL(r1.value(), 0.5);

    r2 = algebra::Rational(-1, 4);
    r1.mul(r2);
    ASSERT_EQUAL(r1.value(), -0.125);
END_TEST

BEGIN_TEST(reduce_functionality)
    algebra::Rational r(8, 16);
    ASSERT_EQUAL(r.value(), 0.5);

    r = algebra::Rational(24, -36);
    ASSERT_EQUAL(r.value(), -0.6666666666666666);
END_TEST

BEGIN_TEST(overflow_in_addition)
    bool exception_caught = false;
    try {
        algebra::Rational r(INT_MAX, 1);
        r.add(1);
    } catch (const std::overflow_error&) {
        exception_caught = true;
    }
    ASSERT_THAT(exception_caught);
END_TEST

BEGIN_TEST(overflow_in_multiplication)
    bool exception_caught = false;
    try {
        algebra::Rational r(INT_MAX, 1);
        r.mul(2);
    } catch (const std::overflow_error&) {
        exception_caught = true;
    }
    ASSERT_THAT(exception_caught);
END_TEST

TEST_SUITE(rational unit tests)
    TEST(constructor_with_valid_input)
    TEST(constructor_with_invalid_denominator)
    TEST(add_integer_to_rational)
    TEST(add_two_rationals)
    TEST(multiply_rational_by_integer)
    TEST(multiply_two_rationals)
    TEST(reduce_functionality)
    TEST(overflow_in_addition)
    TEST(overflow_in_multiplication)
END_SUITE