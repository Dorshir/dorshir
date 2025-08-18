#ifndef EXECUTOR_EXECUTOR_HPP
#define EXECUTOR_EXECUTOR_HPP

#include <cstddef> // for size_t
#include "runnable_task.hpp"

namespace executor {

class Executor 
{

public:

    explicit Executor() = default;
    virtual ~Executor() = default;

    /**
     * @brief Adds a task to the executor with a specified period.
     * 
     * @param task Pointer to a `RunnableTask` to be executed periodically.
     * @param period_ms Period in milliseconds between task executions.
     * @return `true` if the task was successfully added, `false` otherwise.
     */
    virtual bool add(RunnableTask* task, size_t period_ms) = 0;

    /**
     * @brief Runs or resumes execution of scheduled tasks.
     * 
     * This function executes tasks based on their scheduled periods. It resumes execution
     * if the executor was previously paused.
     * 
     * @return The total number of tasks executed during this run.
     */
    virtual size_t run() = 0;
};

} // namespace executor

#endif // EXECUTOR_EXECUTOR_HPP
