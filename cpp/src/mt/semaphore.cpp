#include "semaphore.hpp"
#include <iostream>

namespace mt {

Semaphore::Semaphore(int initial_count)
: m_count{initial_count}
{}

void Semaphore::acquire()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_count <= 0) {
        m_condition.wait(lock);
    }
    --m_count;
}

void Semaphore::release()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    ++m_count;

    m_condition.notify_one();
}

}  // namespace mt
