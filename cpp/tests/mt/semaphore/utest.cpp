#include "mu_test.h"
#include "semaphore.hpp"
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <atomic>

BEGIN_TEST(basic_acquire_release)
{
    mt::Semaphore sem(1);

    bool success = false;

    std::thread t1([&](){
        sem.acquire();
        success = true; // If we reach this line, acquire worked
        sem.release();
    });

    t1.join();

    ASSERT_EQUAL(success, true);
}
END_TEST

BEGIN_TEST(multiple_threads_semaphore)
{
    mt::Semaphore sem(2);

    std::atomic<int> active_threads = 0;
    std::vector<std::thread> threads;

    auto worker = [&](){
        sem.acquire();
        active_threads.fetch_add(1, std::memory_order_relaxed);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        active_threads.fetch_sub(1, std::memory_order_relaxed);
        sem.release();
    };

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker);
    }

    for (auto &t : threads) {
        t.join();
    }

    ASSERT_EQUAL(active_threads.load(), 0);
}
END_TEST

BEGIN_TEST(blocking_behavior)
{
    mt::Semaphore sem(0);

    std::atomic<bool> acquired = false;

    std::thread t1([&](){
        sem.acquire();
        acquired = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_EQUAL(acquired.load(), false);

    sem.release();
    t1.join();

    ASSERT_EQUAL(acquired.load(), true);
}
END_TEST


TEST_SUITE(semaphore tests)

    TEST(basic_acquire_release)
    TEST(multiple_threads_semaphore)
    TEST(blocking_behavior)

END_SUITE
