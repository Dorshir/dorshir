#include <atomic>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "thread_pool_executor.hpp"

namespace mt {

template <typename TaskContainer>
ThreadPoolExecutor<TaskContainer>::ThreadPoolExecutor(size_t capacity_of_tasks, size_t num_of_workers)
: m_tasks{capacity_of_tasks}
, m_capacity_of_tasks{capacity_of_tasks}
, m_stop_submit{false}
, m_stopped_count{0}
{
    add_workers(num_of_workers);
}


template <typename TaskContainer>
ThreadPoolExecutor<TaskContainer>::~ThreadPoolExecutor()
{
    shutdown_gracefull();
}


template <typename TaskContainer>
void ThreadPoolExecutor<TaskContainer>::submit(Task new_task)
{
    if (m_stop_submit.load()) {
        throw ShutdownException("Shutdown has been started");
    }

    m_tasks.enqueue(std::move(new_task));
}


template <typename TaskContainer>
size_t ThreadPoolExecutor<TaskContainer>::tasks_size() const
{
    return m_tasks.size();
}


template <typename TaskContainer>
size_t ThreadPoolExecutor<TaskContainer>::add_workers(size_t num_of_workers)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    m_workers.reserve(num_of_workers);
    for (size_t idx = 0; idx < num_of_workers; ++idx) {
        try {
            std::thread th(&ThreadPoolExecutor::worker_loop, this);
            m_workers.emplace_back(std::move(th), false);
        } catch (std::exception const& e) {
            return idx;
        }
    }
    return num_of_workers;
}


template <typename TaskContainer>
size_t ThreadPoolExecutor<TaskContainer>::remove_workers(size_t num_of_workers)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    size_t removed_count = std::min(m_workers.size(), num_of_workers);

    if (removed_count == 0) {
        return 0;
    }

    size_t joined_count = 0;

    push_poisoned_apples(removed_count);

    m_cv.wait(lock, [this, removed_count]() {
        return m_stopped_count.load(std::memory_order_acquire) >= removed_count;
    });


    auto it = m_workers.begin();
    auto end = m_workers.end();

    while (it != end) {
        if (it->stopped.load(std::memory_order_acquire) && it->thread.joinable()) {
            it->thread.join();
            it = m_workers.erase(it);
            ++joined_count;
        } else {
            ++it;
        }
    }

    m_stopped_count.fetch_sub(joined_count, std::memory_order_relaxed);

    return joined_count;
}


template <typename TaskContainer>
size_t ThreadPoolExecutor<TaskContainer>::worker_size() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_workers.size();
}


template <typename TaskContainer>
void ThreadPoolExecutor<TaskContainer>::shutdown_gracefull()
{
    m_stop_submit.store(true, std::memory_order_relaxed);

    std::vector<std::thread> threads_to_join;

    {
        std::unique_lock<std::mutex> lock(m_mutex);

        push_poisoned_apples(std::distance(m_workers.begin(), m_workers.end()));

        for (auto& worker : m_workers) {
            if (worker.thread.joinable()) {
                threads_to_join.push_back(std::move(worker.thread));
            }
        }

        m_workers.clear();
    }

    for (auto& thread : threads_to_join) {
        thread.join();
    }
}


template <typename TaskContainer>
void ThreadPoolExecutor<TaskContainer>::shutdown_immediate()
{
    m_stop_submit.store(true, std::memory_order_release);

    auto token = EnqueueFrontTag::create_token();

    std::vector<std::thread> threads_to_join;

    {
        std::unique_lock<std::mutex> lock(m_mutex);

        std::for_each(m_workers.begin(), m_workers.end(), [this, &token](auto&) {
            m_tasks.enqueue_front(std::move([] { throw StopThreadException{}; }), token);
        });

        for (auto& worker : m_workers) {
            if (worker.thread.joinable()) {
                threads_to_join.push_back(std::move(worker.thread));
            }
        }

        m_workers.clear();
    }

    for (auto& thread : threads_to_join) {
        thread.join();
    }
}


template <typename TaskContainer>
void ThreadPoolExecutor<TaskContainer>::worker_loop()
{
    while (true) {
        Task task = m_tasks.dequeue();

        #ifdef DEBUG_POOL
        std::cout << "Worker " << std::this_thread::get_id() << " executing task\n";
        #endif
        
        try {
            task();
        } catch(StopThreadException const&) {
            break;
        } catch (...) {}
    }

    mark_as_stopped();
}


template <typename TaskContainer>
void ThreadPoolExecutor<TaskContainer>::mark_as_stopped()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    for (auto &worker : m_workers) {
        if (worker.thread.get_id() == std::this_thread::get_id()) {

            worker.stopped.store(true, std::memory_order_relaxed);
            m_stopped_count.fetch_add(1, std::memory_order_relaxed);
            m_cv.notify_all();
            break;
        }
    }
}


template <typename TaskContainer>
void ThreadPoolExecutor<TaskContainer>::push_poisoned_apples(size_t apple_count)
{
    for (size_t i = 0; i < apple_count; i++) {
        m_tasks.enqueue([] {
            throw StopThreadException{};
        });
    }
}


template <typename TaskContainer>
struct ThreadPoolExecutor<TaskContainer>::WorkerInfo {
    std::thread thread;
    std::atomic<bool> stopped;

    WorkerInfo(std::thread t, bool s)
    : thread(std::move(t))
    , stopped(s)
    {}

    WorkerInfo(WorkerInfo&& other) noexcept
    : thread(std::move(other.thread))
    , stopped(other.stopped.load(std::memory_order_relaxed))
    {}

    WorkerInfo& operator=(WorkerInfo&& other) noexcept 
    {
        if (this != &other) {
            thread = std::move(other.thread);
            stopped.store(other.stopped.load(std::memory_order_relaxed), std::memory_order_relaxed);
        }
        return *this;
    }

    WorkerInfo(const WorkerInfo&) = delete;
    WorkerInfo& operator=(const WorkerInfo&) = delete;
};




} // namespace mt