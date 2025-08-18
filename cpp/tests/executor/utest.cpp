#include "mu_test.h"
#include "periodic_executor.hpp"
#include "monotonic_clock.hpp"
#include "runnable_task.hpp"


class TestIncrementTask : public executor::RunnableTask
{
public:
    explicit TestIncrementTask(int limit)
    : m_counter(0)
    , m_limit(limit) 
    {}

    int execute() override
    {
        ++m_counter;
        return (m_counter < m_limit) ? 0 : 1;
    }

    int counter() const 
    { 
        return m_counter; 
    }

private:

    int m_counter;
    int m_limit;
};

BEGIN_TEST(periodic_executor_can_add_and_run_single_task)
    executor::MonotonicClock clock;
    executor::PeriodicExecutor executor("SingleTaskExecutor", clock);

    TestIncrementTask* task = new TestIncrementTask(3);

    bool added = executor.add(task, 100);
    ASSERT_THAT(added);

    size_t executedCount = executor.run();
    ASSERT_EQUAL(executedCount, 3);
END_TEST

BEGIN_TEST(periodic_executor_can_run_multiple_tasks)
    executor::MonotonicClock clock;
    executor::PeriodicExecutor executor("MultiTaskExecutor", clock);

    TestIncrementTask* taskA = new TestIncrementTask(2);
    TestIncrementTask* taskB = new TestIncrementTask(4);

    bool addedA = executor.add(taskA, 300);
    bool addedB = executor.add(taskB, 200);

    ASSERT_THAT(addedA);
    ASSERT_THAT(addedB);

    size_t executedCount = executor.run();
    ASSERT_EQUAL(executedCount, 6);
END_TEST


TEST_SUITE("PeriodicExecutor Tests")
    TEST(periodic_executor_can_add_and_run_single_task)
    TEST(periodic_executor_can_run_multiple_tasks)
END_SUITE
