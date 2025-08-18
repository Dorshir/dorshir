#ifndef EXECUTOR_PERIODIC_EXECUTOR_HPP
#define EXECUTOR_PERIODIC_EXECUTOR_HPP

#include <queue>
#include <string>
#include <vector>
#include <cstddef> // for size_t

#include "runnable_task.hpp"
#include "clock.hpp"
#include "executor.hpp"
#include "time.hpp"

namespace executor {

class PeriodicExecutor : public Executor
{

public:

    explicit PeriodicExecutor(std::string const& name, Clock& clock);

    PeriodicExecutor(PeriodicExecutor const& other) = default;
    PeriodicExecutor& operator=(PeriodicExecutor const& other) = default;
    ~PeriodicExecutor() = default;

    /**
     * @brief Adds a task to the executor with a specified period.
     * 
     * @param task Pointer to the task to be executed periodically.
     * @param period_ms The execution interval in milliseconds.
     * @return `true` if the task was successfully added, `false` otherwise.
     */
    bool add(RunnableTask* task, size_t period_ms) override;

    /**
     * @brief Starts or resumes execution of tasks.
     * 
     * This function executes tasks based on their scheduled periods. It will block until
     * all tasks have been executed or the executor is paused.
     * 
     * @return The total number of task executions performed during the run.
     */
    size_t run() override;

    /**
     * @brief Pauses the executor, halting task execution.
     * 
     * This function can be used to stop execution mid-run. Remaining tasks will stay in the queue.
     * .........
     * @return The number of tasks remaining in the queue.
     */
    size_t pause();

private:
    /**
     * @struct ScheduledTask
     * @brief Represents a task with its execution schedule.
     * 
     * This struct encapsulates a task, its period, and its next execution time.
     */
    struct ScheduledTask 
    {
        RunnableTask* task;
        Time period;
        Time next_execution_time;

        ScheduledTask(RunnableTask* task, Time const& period, Time const& next_time);
    };

    struct TaskComparator 
    {
        bool operator()(ScheduledTask const& lhs, ScheduledTask const& rhs) const {
            return lhs.next_execution_time > rhs.next_execution_time;
        }
    };

private:

    std::string m_name;
    Clock& m_clock;
    bool m_pause_flag;
    
    std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, TaskComparator> m_tasks;
};

} // namespace executor

#endif // EXECUTOR_PERIODIC_EXECUTOR_HPP
