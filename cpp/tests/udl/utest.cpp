#include "mu_test.h"
#include "udl.hpp"


BEGIN_TEST(sqr_test)
{
    ASSERT_EQUAL(udl::dsqrt(9.0, 1e-6), 3.0);
    ASSERT_EQUAL(udl::dsqrt(0.0, 1e-6), 0.0);
    ASSERT_EQUAL(udl::dsqrt(-9.0, 1e-6), -1.0);
}
END_TEST

BEGIN_TEST(roman_test)
{
    using namespace udl;
    ASSERT_EQUAL("MMM"_roman, 3000);
    ASSERT_EQUAL("MIII"_roman, 1003);
    ASSERT_EQUAL("XXXIX"_roman, 39);
    ASSERT_EQUAL("CCXLVI"_roman, 246);
    ASSERT_EQUAL("DCCLXXXIX"_roman, 789);
    ASSERT_EQUAL("MMCDXXI"_roman, 2421);
    ASSERT_EQUAL("CLX"_roman, 160);
    ASSERT_EQUAL("MIX"_roman, 1009);
    ASSERT_EQUAL("CCVII"_roman, 207);
    ASSERT_EQUAL("MLXVI"_roman, 1066);
    ASSERT_EQUAL("MMMCMXCIX"_roman, 3999);
}
END_TEST


TEST_SUITE(Cave ab homine uinius libri)

    TEST(sqr_test)
    TEST(roman_test)

END_SUITE
