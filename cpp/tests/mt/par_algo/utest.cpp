#include "mu_test.h"
#include "par_algorithms.hpp"
#include <vector>
#include <numeric>
#include <iterator>

BEGIN_TEST(test_par_map_reduce)
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    auto map_func = [](int x) { return x * x; };

    auto reduce_func = [](int acc, int x) { return acc + x; };

    int expected = std::accumulate(
        data.begin(),
        data.end(),
        0,
        [&](int acc, int x) { return acc + map_func(x); }
    );

    int result = mt::par_map_reduce<int, decltype(data.begin()) ,decltype(reduce_func), decltype(map_func)>(
        data.begin(),
        data.end(),
        0,
        reduce_func,
        map_func
    );

    ASSERT_EQUAL(result, expected);
}
END_TEST

BEGIN_TEST(test_par_max)
{
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 10, 7, 8};

    auto max_func = [](int a, int b) { return std::max(a, b); };

    int expected = *std::max_element(data.begin(), data.end());

    int result = mt::par_max<int, std::vector<int>, decltype(max_func)>(
        data,
        std::numeric_limits<int>::min(),
        max_func
    );

    ASSERT_EQUAL(result, expected);
}
END_TEST

TEST_SUITE(par_map_reduce_tests)
    TEST(test_par_map_reduce)
    TEST(test_par_max)
END_SUITE
