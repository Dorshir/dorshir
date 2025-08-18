#include "blocking_queue.hpp"
#include <mutex>
#include <cstddef>
#include <stdexcept>

namespace mt {

template <typename T, typename Privileged>
BlockingBoundedQueue<T, Privileged>::BlockingBoundedQueue(size_t capacity)
: m_num_of_elems{0}
, m_capacity{capacity}
, m_head{0}
, m_tail{0}
, m_elements(std::make_unique<std::byte[]>(capacity * sizeof(T) + alignof(T)))
{
    if (capacity == 0) {
        throw std::invalid_argument("Capacity must be greater than zero.");
    }
}

template <typename T, typename Privileged>
void BlockingBoundedQueue<T, Privileged>::enqueue(T const& value)
{
    enqueue_impl(value);
}

template <typename T, typename Privileged>
void BlockingBoundedQueue<T, Privileged>::enqueue(T&& value)
{
    enqueue_impl(std::forward<T>(value));
}

template <typename T, typename Privileged>
void BlockingBoundedQueue<T, Privileged>::enqueue_impl(T&& value)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_not_full.wait(lock, [this]() { return !this->is_full(); });

    new (&m_elements[m_tail * sizeof(T)]) T(std::move(value));
    m_tail = (m_tail + 1) % m_capacity;
    ++m_num_of_elems;

    m_not_empty.notify_all();
}

template <typename T, typename Privileged>
void BlockingBoundedQueue<T, Privileged>::enqueue_front(T const& value, [[maybe_unused]]Token<Privileged> const& token)
{
    enqueue_front_impl(value);
}

template <typename T, typename Privileged>
void BlockingBoundedQueue<T, Privileged>::enqueue_front(T&& value, [[maybe_unused]]Token<Privileged> const& token)
{
    enqueue_front_impl(std::forward<T>(value));
}

template <typename T, typename Privileged>
void BlockingBoundedQueue<T, Privileged>::enqueue_front_impl(T&& value)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_not_full.wait(lock, [this]() { return !this->is_full(); });

    m_head = (m_head == 0) ? m_capacity - 1 : m_head - 1;
    new (&m_elements[m_head * sizeof(T)]) T(std::move(value));
    ++m_num_of_elems;

    m_not_empty.notify_all();
}

template <typename T, typename Privileged>
T BlockingBoundedQueue<T, Privileged>::dequeue()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_not_empty.wait(lock, [this](){return !this->is_empty();});

    T* elem_ptr = reinterpret_cast<T*>(&m_elements[m_head * sizeof(T)]);

    T elem = std::move(*elem_ptr);

    elem_ptr->~T();

    m_head = (m_head + 1) % m_capacity;
    --m_num_of_elems;
    m_not_full.notify_all();

    return elem;
}

template <typename T, typename Privileged>
void BlockingBoundedQueue<T, Privileged>::clear()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (!is_empty()) {
        T clear = dequeue();
    }
}

template <typename T, typename Privileged>
bool BlockingBoundedQueue<T, Privileged>::is_empty() const
{
    return m_num_of_elems == 0;
}

template <typename T, typename Privileged>
bool BlockingBoundedQueue<T, Privileged>::is_full() const
{
    return m_num_of_elems == m_capacity;
}

template <typename T, typename Privileged>
size_t BlockingBoundedQueue<T, Privileged>::size() const
{
    return m_num_of_elems;
}

template <typename T, typename Privileged>
size_t BlockingBoundedQueue<T, Privileged>::capacity() const
{
    return m_capacity;
}

} // namespace mt