#ifndef EXECUTOR_RUNNABLE_TASK_HPP
#define EXECUTOR_RUNNABLE_TASK_HPP

namespace executor {

/**
 * @class RunnableTask
 * @brief Abstract base class representing a task that can be executed by an executor.
 * 
 * This class provides an interface for tasks that can be scheduled and executed 
 * by an executor. Derived classes must implement the `execute` method.
 */
class RunnableTask 
{
public:

    explicit RunnableTask() = default;
    virtual ~RunnableTask() = default;

    /**
     * @brief Executes the task.
     * 
     * Derived classes must implement this method to define the behavior of the task
     * when executed by the executor.
     * 
     * @return An integer status code:
     *         - `0`: The task should be re-scheduled.
     *         - Non-zero: The task should not be re-scheduled.
     */
    virtual int execute() = 0;


};

} // namespace executor

#endif // EXECUTOR_RUNNABLE_TASK_HPP
