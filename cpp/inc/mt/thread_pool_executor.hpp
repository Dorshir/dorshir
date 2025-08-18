#ifndef MT_THREAD_POOL_HPP
#define MT_THREAD_POOL_HPP

#include "blocking_queue.hpp"
#include "enqueue_front_tag.hpp"

#include <atomic>
#include <cstddef>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>


namespace mt {

using Task = std::function<void()>;

/**
* @brief A thread pool executor that manages worker threads and executes tasks concurrently.
*
* @tparam TaskContainer The container type used for storing and retrieving tasks.
*/
template <typename TaskContainer = BlockingBoundedQueue<Task, EnqueueFrontTag>>
class ThreadPoolExecutor {

public:

    /**
    * @brief Constructs a ThreadPoolExecutor with a given number of worker threads and task queue capacity.
    *
    * @param num_of_workers The number of worker threads.
    * @param capacity_of_tasks The maximum number of tasks in the queue.
    */
    explicit ThreadPoolExecutor(size_t capacity_of_tasks = 100, size_t num_of_workers = std::thread::hardware_concurrency() - 1);
    
    /**
    * @brief Destruct the ThreadPoolExecutor using shutdown_gracefull.
    */
    ~ThreadPoolExecutor();

    ThreadPoolExecutor(const ThreadPoolExecutor&) = delete;
    ThreadPoolExecutor& operator=(const ThreadPoolExecutor&) = delete;
    ThreadPoolExecutor(ThreadPoolExecutor&&) noexcept = delete;
    ThreadPoolExecutor& operator=(ThreadPoolExecutor&&) noexcept = delete;

public:

    /**
    * @brief Submits a new task to the thread pool.
    *
    * @param new_task A callable object (e.g., lambda function) that will be executed by a worker thread.
    */
    void submit(Task new_task);

    /**
    * @brief Gets the number of tasks currently in the queue.
    *
    * @return The number of pending tasks.
    */
    size_t tasks_size() const;

    /**
    * @brief Adds additional worker threads to the thread pool.
    *
    * @param num_of_workers The number of new workers to add.
    * @return The actual number of workers added.
    */
    size_t add_workers(size_t num_of_workers = 1);

    /**
    * @brief Removes worker threads from the thread pool.
    *
    * @param num_of_workers The number of workers to remove.
    * @return The actual number of removed workers.
    */
    size_t remove_workers(size_t num_of_workers);

    /**
    * @brief Gets the current number of worker threads in the pool.
    *
    * @return The number of active worker threads.
    */
    size_t worker_size() const;

    /**
    * @brief Initiates a graceful shutdown of the thread pool.
    *
    * The thread pool stops accepting new tasks, processes all queued tasks,
    * and then shuts down all worker threads.
    */
    void shutdown_gracefull();

    /**
    * @brief Initiates an immediate shutdown of the thread pool.
    *
    * The thread pool stops accepting new tasks, clears the task queue,
    * and then shuts down all worker threads immediately.
    */
    void shutdown_immediate();

private:

    /**
    * @brief Exception used internally to signal a worker thread to stop.
    */
    class StopThreadException : public std::exception{};

    /**
    * @brief Exception used internally to stop submitting tasks, since shutdown has been started.
    */
    class ShutdownException : public std::runtime_error {
        public:
        using std::runtime_error::runtime_error;
    };

    /**
    * @brief Worker thread function that continuously processes tasks from the queue.
    *
    * The worker thread will keep executing tasks until the thread pool is shut down.
    */
    void worker_loop();

    /**
    * @brief Enqueues a specified number of poisoned tasks to stop worker threads.
    *
    * @param apple_count The number of poisoned tasks to enqueue, typically corresponding 
    *                    to the number of worker threads that need to be stopped.
    */
    void push_poisoned_apples(size_t apple_count);
    
    /**
    * @brief Marks the current worker thread as stopped.
    *
    * This method is called within the worker_loop when a worker is about to exit.
    * It increments the stopped count and notifies any waiting threads via the condition variable.
    */
    void mark_as_stopped();


private:

    /**
    * @brief Holds information about a worker thread.
    */
    struct WorkerInfo;
    
    TaskContainer m_tasks;
    std::vector<WorkerInfo> m_workers;
    size_t m_capacity_of_tasks;
    mutable std::mutex m_mutex;
    std::atomic<bool> m_stop_submit;
    std::atomic<size_t> m_stopped_count;
    std::condition_variable m_cv;


};

} // namespace mt

#include "thread_pool_executor.txx"

#endif // MT_THREAD_POOL_HPP
