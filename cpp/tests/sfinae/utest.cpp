#include "mu_test.h"
#include "sfinae.hpp"

#include <vector>
#include <list>
#include <complex>


BEGIN_TEST(sfinae)
{
    ASSERT_EQUAL(meta::cpp11::square<>(1), 1);                                                           // int
    ASSERT_EQUAL(meta::cpp11::square<>(1.5), 2.25);                                                      // double
    ASSERT_EQUAL(meta::cpp11::square<>(std::vector<int>{1}), 1);                                         // vector
    ASSERT_EQUAL(meta::cpp11::square<>(std::list<std::complex<double>>{1}), std::complex<double>(1));    // list of complexes
}
END_TEST


TEST_SUITE(Cave ab homine uinius libri)

    TEST(sfinae)

END_SUITE
