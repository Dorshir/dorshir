#include "mu_test.h"
#include "blocking_queue.hpp"
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

struct Cat {
    int souls;
    short tail;
    unsigned char nick;

    Cat(int s, short t, unsigned char n)
    : souls{s}
    , tail{t}
    , nick{n}
    {}
};

BEGIN_TEST(simple_integer_bbq)

    mt::BlockingBoundedQueue<int,Cat> queue(5);

    ASSERT_THAT(queue.size() == 0);
    ASSERT_THAT(queue.capacity() == 5);
    
    queue.enqueue(10);
    ASSERT_THAT(queue.size() == 1);

    queue.enqueue(20);
    ASSERT_THAT(queue.size() == 2);

    queue.enqueue(30);
    ASSERT_THAT(queue.size() == 3);

    ASSERT_EQUAL(queue.dequeue(), 10);
    ASSERT_EQUAL(queue.dequeue(), 20);
    ASSERT_EQUAL(queue.dequeue(), 30);

    ASSERT_THAT(queue.size() == 0);

END_TEST


BEGIN_TEST(more_complex_obj_bbq)

    mt::BlockingBoundedQueue<Cat, Cat> queue(5);

    ASSERT_THAT(queue.size() == 0);
    ASSERT_THAT(queue.capacity() == 5);
    
    queue.enqueue(Cat{9,3,'N'});
    ASSERT_THAT(queue.size() == 1);

    queue.enqueue(Cat(8,2,'T'));
    ASSERT_THAT(queue.size() == 2);

    queue.enqueue(Cat(7,1,'J'));
    ASSERT_THAT(queue.size() == 3);

    ASSERT_EQUAL(queue.dequeue().tail, 3);
    ASSERT_EQUAL(queue.dequeue().tail, 2);
    ASSERT_EQUAL(queue.dequeue().tail, 1);

    ASSERT_THAT(queue.size() == 0);

END_TEST

BEGIN_TEST(simple_multi_threaded_bbq_FIFO_order)
{
    constexpr int N = 1000;

    mt::BlockingBoundedQueue<int,Cat> queue(N/4);

    std::vector<int> consumed_values;
    consumed_values.resize(N);

    // Producer
    std::thread producer([&queue] {
        for (int i = 0; i < N; ++i) {
            queue.enqueue(i);
        }
    });

    // Consumer
    std::thread consumer([&queue, &consumed_values] {
        for (int i = 0; i < N; ++i) {
            consumed_values[i] = queue.dequeue();
        }
    });

    producer.join();
    consumer.join();

    // Check the consumer got all items in correct FIFO order
    for (int i = 0; i < N; ++i) {
        ASSERT_EQUAL(consumed_values[i], i );
    }

    ASSERT_THAT(queue.is_empty());
}
END_TEST


TEST_SUITE(bq tests)

    TEST(simple_integer_bbq)
    TEST(more_complex_obj_bbq)
    TEST(simple_multi_threaded_bbq_FIFO_order)

END_SUITE
