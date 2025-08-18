#ifndef MT_SEMAPHORE_HPP
#define MT_SEMAPHORE_HPP

#include <condition_variable>
#include <mutex>

namespace mt {

class Semaphore {

public:

    explicit Semaphore(int initial_count);
    ~Semaphore() = default;

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;
    Semaphore(Semaphore&&) noexcept = delete;
    Semaphore& operator=(Semaphore&&) noexcept = delete;

public:

    /**
    * @brief Acquires the semaphore (decrements the count).
    * 
    * If the semaphore count is greater than zero, it decrements the count and continues.
    * If the count is zero, the calling thread blocks until another thread releases the semaphore.
    */
    void acquire();

    /**
    * @brief Releases the semaphore (increments the count).
    * 
    * Increments the semaphore count and wakes up one waiting thread, if any.
    */
    void release();

private:

    int m_count;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
};

}  // namespace mt

#endif  // MT_SEMAPHORE_HPP