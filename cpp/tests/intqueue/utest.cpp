#include "mu_test.h"
#include "intqueue.hpp"


BEGIN_TEST(constructor_with_valid_input)
    ds::IntQueue queue(10);
    ASSERT_EQUAL(queue.size(), 0);
END_TEST

BEGIN_TEST(default_constructor)
    ds::IntQueue queue;
    ASSERT_EQUAL(queue.size(), 0);
END_TEST

BEGIN_TEST(enqueue_operation)
    ds::IntQueue queue(5);

    queue.enqueue(10);
    ASSERT_EQUAL(queue.size(), 1);

    queue.enqueue(20);
    ASSERT_EQUAL(queue.size(), 2);

    ASSERT_EQUAL(queue.dequeue(), 10);
END_TEST

BEGIN_TEST(dequeue_operation)
    ds::IntQueue queue(5);

    queue.enqueue(10);
    queue.enqueue(20);

    ASSERT_EQUAL(queue.dequeue(), 10);
    ASSERT_EQUAL(queue.size(), 1);

    ASSERT_EQUAL(queue.dequeue(), 20);
    ASSERT_EQUAL(queue.size(), 0);
END_TEST

BEGIN_TEST(dequeue_empty_queue)
    ds::IntQueue queue(5);
    
END_TEST

BEGIN_TEST(enqueue_overflow)
    ds::IntQueue queue(2);

END_TEST

BEGIN_TEST(dequeue_underflow)
    ds::IntQueue queue(2);

END_TEST

BEGIN_TEST(copy_constructor)
    ds::IntQueue original(5);
    original.enqueue(10);
    original.enqueue(20);

    ds::IntQueue copy(original);

    ASSERT_EQUAL(copy.size(), 2);
    ASSERT_EQUAL(copy.dequeue(), 10);
    ASSERT_EQUAL(copy.dequeue(), 20);
    ASSERT_EQUAL(copy.size(), 0);
END_TEST

BEGIN_TEST(copy_assignment_operator)
    ds::IntQueue original(5);
    original.enqueue(10);
    original.enqueue(20);

    ds::IntQueue copy = original;

    ASSERT_EQUAL(copy.size(), 2);
    ASSERT_EQUAL(copy.dequeue(), 10);
    ASSERT_EQUAL(copy.dequeue(), 20);
    ASSERT_EQUAL(copy.size(), 0);
END_TEST

BEGIN_TEST(empty_function)
    ds::IntQueue queue(5);
    ASSERT_THAT(queue.empty());

    queue.enqueue(10);
    ASSERT_THAT(!queue.empty());
END_TEST

BEGIN_TEST(full_function)
    ds::IntQueue queue(2);

    queue.enqueue(10);
    ASSERT_THAT(!queue.full());

    queue.enqueue(20);
    ASSERT_THAT(queue.full());
END_TEST

BEGIN_TEST(swap_functionality)
    ds::IntQueue queue1(5);
    ds::IntQueue queue2(5);

    queue1.enqueue(10);
    queue1.enqueue(20);

    queue2.enqueue(30);
    queue2.enqueue(40);

    queue1.swap(queue2);

    ASSERT_EQUAL(queue1.size(), 2);
    ASSERT_EQUAL(queue1.dequeue(), 30);
    ASSERT_EQUAL(queue1.dequeue(), 40);

    ASSERT_EQUAL(queue2.size(), 2);
    ASSERT_EQUAL(queue2.dequeue(), 10);
    ASSERT_EQUAL(queue2.dequeue(), 20);
END_TEST

TEST_SUITE(IntQueue unit tests)
    TEST(constructor_with_valid_input)
    TEST(default_constructor)
    TEST(enqueue_operation)
    TEST(dequeue_operation)
    TEST(copy_constructor)
    TEST(copy_assignment_operator)
    TEST(empty_function)
    TEST(full_function)
    TEST(swap_functionality)
END_SUITE
