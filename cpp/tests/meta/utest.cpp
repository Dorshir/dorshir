#include "mu_test.h"
#include "meta.hpp"

#include <cstddef>

BEGIN_TEST(count_bits_few)
{
    using namespace meta;
    ASSERT_EQUAL(bit_count_v<3>, 2);
}
END_TEST

BEGIN_TEST(count_bits_alot)
{
    using namespace meta;
    ASSERT_EQUAL(bit_count_v<262143>, 18);
}
END_TEST

BEGIN_TEST(power_positive)
{
    using namespace meta;
    auto power_result = power_v<2, 10>;
    ASSERT_EQUAL(power_result, 1024.0);
}
END_TEST

BEGIN_TEST(power_negative)
{
    using namespace meta;
    auto power_result = power_v<2, -3>;
    ASSERT_EQUAL(power_result, 0.125);
}
END_TEST


TEST_SUITE(meta tests)

    TEST(count_bits_few)
    TEST(count_bits_alot)
    TEST(power_positive)
    TEST(power_negative)

END_SUITE
