#include "mu_test.h"
#include "list.hpp"
#include <string>

static int g_sum = 0;

void AddToGlobalSum(int& data)
{
    g_sum += data;
}

void MultiplyByTwo(int& data)
{
    data *= 2;
}

BEGIN_TEST(list_empty_on_construction)
    ds::List<int> lst;
    ASSERT_EQUAL(lst.size(), 0);
    ASSERT_THAT(lst.empty());
END_TEST

BEGIN_TEST(list_push_head_and_pop_head)
    ds::List<int> lst;
    lst.push_head(10);
    lst.push_head(20);

    ASSERT_EQUAL(lst.size(), 2);
    ASSERT_THAT(!lst.empty());

    int value = 0;
    ASSERT_THAT(lst.pop_head(value));
    ASSERT_EQUAL(value, 20);
    ASSERT_EQUAL(lst.size(), 1);

    ASSERT_THAT(lst.pop_head(value));
    ASSERT_EQUAL(value, 10);
    ASSERT_EQUAL(lst.size(), 0);

    ASSERT_THAT(!lst.pop_head(value));
END_TEST

BEGIN_TEST(list_push_tail_and_pop_tail)
    ds::List<int> lst;
    lst.push_tail(100);
    lst.push_tail(200);
    lst.push_tail(300);

    ASSERT_EQUAL(lst.size(), 3);

    int value = 0;
    ASSERT_THAT(lst.pop_tail(value));
    ASSERT_EQUAL(value, 300);
    ASSERT_EQUAL(lst.size(), 2);

    ASSERT_THAT(lst.pop_tail(value));
    ASSERT_EQUAL(value, 200);
    ASSERT_EQUAL(lst.size(), 1);

    ASSERT_THAT(lst.pop_tail(value));
    ASSERT_EQUAL(value, 100);
    ASSERT_EQUAL(lst.size(), 0);

    ASSERT_THAT(!lst.pop_tail(value));
END_TEST

BEGIN_TEST(list_mixed_push_and_pop)
    ds::List<int> lst;
    lst.push_head(1);
    lst.push_head(2);
    lst.push_head(3);
    lst.push_tail(99);
    lst.push_tail(100);

    ASSERT_EQUAL(lst.size(), 5);

    int value = 0;
    lst.pop_head(value);
    ASSERT_EQUAL(value, 3);

    lst.pop_tail(value);
    ASSERT_EQUAL(value, 100);

    lst.pop_head(value);
    ASSERT_EQUAL(value, 2);

    lst.pop_tail(value);
    ASSERT_EQUAL(value, 99);

    lst.pop_head(value);
    ASSERT_EQUAL(value, 1);
    ASSERT_THAT(lst.empty());
END_TEST

BEGIN_TEST(list_clear_test)
    ds::List<int> lst;
    lst.push_head(10);
    lst.push_tail(20);
    lst.push_tail(30);

    ASSERT_EQUAL(lst.size(), 3);
    lst.clear();
    ASSERT_EQUAL(lst.size(), 0);
    ASSERT_THAT(lst.empty());

    lst.clear();
    ASSERT_THAT(lst.empty());
END_TEST

BEGIN_TEST(list_for_each_test)
    ds::List<int> lst;
    lst.push_head(1);
    lst.push_head(2);
    lst.push_head(3);

    g_sum = 0;
    
    lst.for_each(AddToGlobalSum);
    ASSERT_EQUAL(g_sum, 6);

    lst.for_each(MultiplyByTwo);


    g_sum = 0;

    lst.for_each(AddToGlobalSum);
    ASSERT_EQUAL(g_sum, 12);
END_TEST

BEGIN_TEST(list_copy_constructor_test)
    ds::List<int> original;
    original.push_tail(10);
    original.push_tail(20);
    original.push_tail(30);

    ds::List<int> copy{original};

    ASSERT_EQUAL(original.size(), 3);
    ASSERT_EQUAL(copy.size(), 3);

    int val = 0;
    copy.pop_head(val);
    ASSERT_EQUAL(val, 10);
    ASSERT_EQUAL(copy.size(), 2);

    ASSERT_EQUAL(original.size(), 3);

    original.pop_head(val);
    ASSERT_EQUAL(val, 10);
    ASSERT_EQUAL(original.size(), 2);
END_TEST

BEGIN_TEST(list_assignment_operator_test)
    ds::List<int> list1;
    list1.push_head(111);
    list1.push_head(222);

    ds::List<int> list2;
    list2.push_tail(999);
    list2.push_tail(888);
    list2.push_tail(777);

    ASSERT_EQUAL(list1.size(), 2);
    ASSERT_EQUAL(list2.size(), 3);

    list2 = list1;

    ASSERT_EQUAL(list2.size(), 2);

    int val1 = 0, val2 = 0;
    list1.pop_head(val1);
    ASSERT_EQUAL(val1, 222);
    list2.pop_head(val2);
    ASSERT_EQUAL(val2, 222);

    list1.pop_head(val1);
    list2.pop_head(val2);
    ASSERT_EQUAL(val1, 111);
    ASSERT_EQUAL(val2, 111);

    ASSERT_THAT(list1.empty());
    ASSERT_THAT(list2.empty());

    list1 = list1;
    ASSERT_THAT(list1.empty());
END_TEST

TEST_SUITE(List Unit Tests)
    TEST(list_empty_on_construction)
    TEST(list_push_head_and_pop_head)
    TEST(list_push_tail_and_pop_tail)
    TEST(list_mixed_push_and_pop)
    TEST(list_clear_test)
    TEST(list_for_each_test)
    TEST(list_copy_constructor_test)
    TEST(list_assignment_operator_test)
END_SUITE
