#include "mu_test.h"
#include "thread_pool_executor.hpp"
#include "blocking_queue.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

using Task = std::function<void()>;

BEGIN_TEST(basic_thread_pool_execution)
{
    std::atomic<int> counter = 0;
    mt::ThreadPoolExecutor pool{50, 3};

    ASSERT_EQUAL(pool.tasks_size(), 0);
    ASSERT_EQUAL(pool.worker_size(), 3);

    for (int i = 0; i < 10; ++i) {
        pool.submit([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            counter.fetch_add(1);
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    ASSERT_EQUAL(counter.load(), 10);
}
END_TEST

BEGIN_TEST(task_capacity_limit)
{
    mt::ThreadPoolExecutor pool{5,0};
    std::atomic<int> counter = 0;

    for (int i = 0; i < 5; ++i) {
        pool.submit([&counter]() { counter.fetch_add(1); });
    }

    ASSERT_EQUAL(pool.tasks_size(), 5);
    pool.add_workers(2); // Should not cause any tasks to be submitted as the queue is full

    // Attempt to add another task (should be blocked if the queue is full)
    // bool submitted = true;
    try {
        pool.submit([]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
        ASSERT_THAT(true);
    } catch (...) {
        ASSERT_FAIL("Task submission failed!");
    }

}
END_TEST

BEGIN_TEST(graceful_shutdown)
{
    std::atomic<int> counter = 0;
    mt::ThreadPoolExecutor pool{50, 3};

    for (int i = 0; i < 10; ++i) {
        pool.submit([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            counter.fetch_add(1);
        });
    }

    pool.shutdown_gracefull();
    ASSERT_EQUAL(counter.load(), 10); // Ensure all tasks complete before shutdown
}
END_TEST

BEGIN_TEST(immediate_shutdown)
{
    std::atomic<int> counter = 0;
    mt::ThreadPoolExecutor pool{50, 3};

    for (int i = 0; i < 10; ++i) {
        pool.submit([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            counter.fetch_add(1);
        });
    }

    pool.shutdown_immediate();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Some tasks may not complete due to immediate shutdown
    ASSERT_THAT(counter.load() < 10);
}
END_TEST

BEGIN_TEST(dynamic_worker_scaling)
{
    mt::ThreadPoolExecutor pool{50,2};

    ASSERT_EQUAL(pool.worker_size(), 2);

    pool.add_workers(2);
    ASSERT_EQUAL(pool.worker_size(), 4);

    pool.remove_workers(4);
    ASSERT_EQUAL(pool.worker_size(), 0);
    
}
END_TEST

BEGIN_TEST(concurrent_task_submission)
{
    mt::ThreadPoolExecutor pool{100,4};
    std::atomic<int> counter = 0;
    constexpr int num_tasks = 50;
    
    std::vector<std::thread> submitters;
    for (int i = 0; i < 5; ++i) {
        submitters.emplace_back([&]() {
            for (int j = 0; j < num_tasks / 5; ++j) {
                pool.submit([&counter]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    counter.fetch_add(1);
                });
            }
        });
    }

    for (auto& thread : submitters) {
        thread.join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    ASSERT_EQUAL(counter.load(), num_tasks);
}
END_TEST

TEST_SUITE(threadpool tests)

    TEST(basic_thread_pool_execution)
    TEST(task_capacity_limit)
    TEST(graceful_shutdown)
    TEST(immediate_shutdown)
    TEST(dynamic_worker_scaling)
    TEST(concurrent_task_submission)

END_SUITE
