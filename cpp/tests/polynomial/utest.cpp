#include "mu_test.h"
#include "polynomial.hpp"
#include "rational.hpp"

using namespace algebra;

BEGIN_TEST(default_constructor)
    Polynomial p; 
    ASSERT_EQUAL(p(Rational(1, 1)), 0.0);
END_TEST

BEGIN_TEST(constructor_with_degree)
    Polynomial p(5);
    ASSERT_EQUAL(p(Rational(2, 1)), 0.0);
END_TEST

BEGIN_TEST(constructor_from_range)
    Rational c[] = { Rational(1, 2), Rational(2, 3), Rational(4, 1) };
    Polynomial p{c, c + 3};

    double res = p(Rational(1, 1));
    ASSERT_THAT((res > 5.16) && (res < 5.17));
END_TEST

BEGIN_TEST(copy_constructor)
    Polynomial original(2);
    original[0] = Rational(3, 2);
    original[1] = Rational(1, 1);
    original[2] = Rational(-2, 1);

    Polynomial copy(original);
    double origVal = original(Rational(2, 1));
    double copyVal = copy(Rational(2, 1));
    ASSERT_EQUAL(origVal, copyVal);
END_TEST

BEGIN_TEST(assignment_operator)
    Polynomial p(1);
    p[0] = Rational(1, 1);
    p[1] = Rational(2, 1);

    Polynomial q(3);
    q = p;

    double valP = p(Rational(3, 1));
    double valQ = q(Rational(3, 1));
    ASSERT_EQUAL(valP, valQ);
END_TEST

BEGIN_TEST(operator_index_access)
    Polynomial p(2);
    p[0] = Rational(2, 3);
    p[1] = Rational(3, 1);
    p[2] = Rational(-1, 2);

    ASSERT_THAT(p[0].value() == (2.0 / 3.0));
    ASSERT_THAT(p[1].value() == 3.0);
    ASSERT_THAT(p[2].value() == -0.5);
END_TEST

BEGIN_TEST(polynomial_evaluation)
    Polynomial p(2);
    p[0] = Rational(2, 1);
    p[1] = Rational(-1, 3);
    p[2] = Rational(3, 4);

    double val = p(Rational(2, 1));
    ASSERT_THAT(val > 4.33 && val < 4.34);
    
END_TEST

BEGIN_TEST(operator_mult_by_int)
    Polynomial p(1);
    p[0] = Rational(1, 1);
    p[1] = Rational(1, 1);

    p *= 3;
    double val = p(Rational(2, 1));
    ASSERT_EQUAL(val, 9.0);
END_TEST

BEGIN_TEST(operator_mult_by_rational)
    Polynomial p(1);
    p[0] = Rational(1, 1);
    p[1] = Rational(2, 1);

    p *= Rational(1, 2);
    double val = p(Rational(2, 1));
    ASSERT_THAT(val > 2.49 && val < 2.51);
END_TEST

BEGIN_TEST(operator_plus_polynomials)
    Polynomial p(1);
    p[0] = Rational(2, 1);
    p[1] = Rational(1, 1);

    Polynomial q(0);
    q[0] = Rational(3, 1);

    Polynomial sum = p + q;

    double val = sum(Rational(2, 1));
    ASSERT_EQUAL(val, 7.0);
END_TEST

BEGIN_TEST(operator_mult_polynomials)
    Polynomial p(1);
    p[0] = Rational(1, 1);
    p[1] = Rational(1, 1);

    Polynomial q(1);
    q[0] = Rational(-1, 1);
    q[1] = Rational(2, 1);

    Polynomial r = p * q;
    double val = r(Rational(2, 1));
    ASSERT_EQUAL(val, 9.0);
END_TEST

BEGIN_TEST(derive_polynomial)
    Polynomial p(3);
    p[0] = Rational(-4, 1);
    p[1] = Rational(0, 1);
    p[2] = Rational(3, 1);
    p[3] = Rational(2, 1);

    Polynomial dp = derive(p);
    double val = dp(Rational(2, 1));
    ASSERT_EQUAL(val, 36.0);
END_TEST

TEST_SUITE(Polynomial unit tests)
    TEST(default_constructor)
    TEST(constructor_with_degree)
    TEST(constructor_from_range)
    TEST(copy_constructor)
    TEST(assignment_operator)
    TEST(operator_index_access)
    TEST(polynomial_evaluation)
    TEST(operator_mult_by_int)
    TEST(operator_mult_by_rational)
    TEST(operator_plus_polynomials)
    TEST(operator_mult_polynomials)
    TEST(derive_polynomial)
END_SUITE
