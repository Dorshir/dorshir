#include "mu_test.h"
#include "count_if.hpp"
#include <vector>
#include <list>
#include <cstddef>

BEGIN_TEST(count_if_vec_ints)

    std::vector<int> vec = {1, 3, 5, 6, 7, 8, 9};

    size_t count = algo::count_if(vec.begin(), vec.end(), [](int elem) { return elem % 2 == 0; });
    ASSERT_EQUAL(count, 2);

END_TEST

BEGIN_TEST(count_if_list_doubles)

    std::list<double> lst = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
    size_t count = algo::count_if(lst.begin(), lst.end(), [](double elem) { return elem > 4.0; });

    ASSERT_EQUAL(count, 4);

END_TEST

BEGIN_TEST(count_if_using_combinator)

    std::vector<int> vec = {1, 3, 5, 6, 7, 8, 9};

    auto even = [](int elem){return elem % 2 == 0;};
    auto big = [](int elem){return elem > 5;};

    size_t count = algo::count_if(vec.begin(), vec.end(), algo::and_combinator(even, big));

    ASSERT_EQUAL(count, 2);

END_TEST

TEST_SUITE(count_if tests)

    TEST(count_if_vec_ints)
    TEST(count_if_list_doubles)
    TEST(count_if_using_combinator)

END_SUITE