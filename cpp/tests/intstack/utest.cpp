#include "mu_test.h"
#include "intstack.hpp"
#include <stdexcept>    // overflow_error, underflow_error
#include <climits>      // INT_MAX

#define DEFAULT_CAP 16

BEGIN_TEST(constructor_with_valid_input)
    ds::IntStack stack(10);
    ASSERT_EQUAL(stack.capacity(), 10);
    ASSERT_EQUAL(stack.size(), 0);
END_TEST

BEGIN_TEST(default_constructor)
    ds::IntStack stack;
    ASSERT_EQUAL(stack.capacity(), DEFAULT_CAP);
    ASSERT_EQUAL(stack.size(), 0);
END_TEST

BEGIN_TEST(push_to_stack)
    ds::IntStack stack(5);

    stack.push(10);
    ASSERT_EQUAL(stack.size(), 1);
    ASSERT_EQUAL(stack.top(), 10);

    stack.push(20);
    ASSERT_EQUAL(stack.size(), 2);
    ASSERT_EQUAL(stack.top(), 20);
END_TEST

BEGIN_TEST(pop_from_stack)
    ds::IntStack stack(5);

    stack.push(10);
    stack.push(20);

    ASSERT_EQUAL(stack.pop(), 20);
    ASSERT_EQUAL(stack.size(), 1);

    ASSERT_EQUAL(stack.pop(), 10);
    ASSERT_EQUAL(stack.size(), 0);
END_TEST

BEGIN_TEST(top_on_empty_stack)
    ds::IntStack stack(5);
    ASSERT_EQUAL(stack.top(), INT_MAX); // INT_MAX for empty stack
END_TEST

BEGIN_TEST(push_overflow)
    ds::IntStack stack(2);

    stack.push(10);
    stack.push(20);

    bool exception_caught = false;
    try {
        stack.push(30);
    } catch (const std::overflow_error&) {
        exception_caught = true;
    }
    ASSERT_THAT(exception_caught);
END_TEST

BEGIN_TEST(pop_underflow)
    ds::IntStack stack(2);

    bool exception_caught = false;
    try {
        stack.pop();
    } catch (const std::underflow_error&) {
        exception_caught = true;
    }
    ASSERT_THAT(exception_caught);
END_TEST

BEGIN_TEST(drain_stack)
    ds::IntStack stack1(5);
    ds::IntStack stack2(5);

    stack1.push(10);
    stack1.push(20);

    stack2.push(30);
    stack2.push(40);

    stack2.drain(stack1);

    ASSERT_EQUAL(stack2.size(), 4);
    ASSERT_EQUAL(stack2.pop(), 20);
    ASSERT_EQUAL(stack2.pop(), 10);
    ASSERT_EQUAL(stack2.pop(), 40);
    ASSERT_EQUAL(stack2.pop(), 30);

    ASSERT_EQUAL(stack1.size(), 0);
END_TEST

BEGIN_TEST(copy_constructor)
    ds::IntStack original(5);
    original.push(10);
    original.push(20);

    ds::IntStack copy(original);
    ASSERT_EQUAL(copy.size(), 2);
    ASSERT_EQUAL(copy.top(), 20);

    ASSERT_EQUAL(copy.pop(), 20);
    ASSERT_EQUAL(copy.pop(), 10);
    ASSERT_EQUAL(copy.size(), 0);
END_TEST

BEGIN_TEST(copy_assignment_operator)
    ds::IntStack original(5);
    original.push(10);
    original.push(20);

    ds::IntStack copy = original;
    ASSERT_EQUAL(copy.size(), 2);
    ASSERT_EQUAL(copy.top(), 20);

    ASSERT_EQUAL(copy.pop(), 20);
    ASSERT_EQUAL(copy.pop(), 10);
    ASSERT_EQUAL(copy.size(), 0);
END_TEST

BEGIN_TEST(is_empty_function)
    ds::IntStack stack(5);
    ASSERT_THAT(is_empty(stack));

    stack.push(10);
    ASSERT_THAT(!is_empty(stack));
END_TEST

BEGIN_TEST(is_full_function)
    ds::IntStack stack(2);

    stack.push(10);
    ASSERT_THAT(!is_full(stack));

    stack.push(20);
    ASSERT_THAT(is_full(stack));
END_TEST

BEGIN_TEST(swap_functionality)
    ds::IntStack stack1(5);
    ds::IntStack stack2(5);

    stack1.push(10);
    stack1.push(20);

    stack2.push(30);
    stack2.push(40);

    stack1.swap(stack2);

    ASSERT_EQUAL(stack1.size(), 2);
    ASSERT_EQUAL(stack1.pop(), 40);
    ASSERT_EQUAL(stack1.pop(), 30);

    ASSERT_EQUAL(stack2.size(), 2);
    ASSERT_EQUAL(stack2.pop(), 20);
    ASSERT_EQUAL(stack2.pop(), 10);
END_TEST

TEST_SUITE(intstack unit tests)
    TEST(constructor_with_valid_input)
    TEST(default_constructor)
    TEST(push_to_stack)
    TEST(pop_from_stack)
    TEST(top_on_empty_stack)
    TEST(push_overflow)
    TEST(pop_underflow)
    TEST(drain_stack)
    TEST(copy_constructor)
    TEST(copy_assignment_operator)
    TEST(is_empty_function)
    TEST(is_full_function)
    TEST(swap_functionality)
END_SUITE
