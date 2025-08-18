#ifndef MT_BLOCKING_QUEUE_HPP
#define MT_BLOCKING_QUEUE_HPP

#include "token.hpp"
#include "enqueue_front_tag.hpp"

#include <array>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <type_traits>

namespace mt {

template <typename TaskContainer>
class ThreadPoolExecutor;

/**
* @brief A thread-safe bounded blocking queue.
*
* The BlockingBoundedQueue class provides a fixed-size queue that supports
* concurrent enqueuing and dequeuing of elements. When the queue is full, 
* enqueue operations block until space becomes available. Similarly, if the 
* queue is empty, dequeue operations block until an element is available.
*
* @tparam T The type of elements stored in the queue.
* @tparam Privileged The privileged class that is restricted to use the enqeueu front
*/
template <typename T, typename Privileged>
class BlockingBoundedQueue {

public:

    /**
    * @brief Constructs a BlockingBoundedQueue with the specified capacity.
    *
    * @param capacity The maximum number of elements the queue can hold.
    *                 Must be greater than zero. Default is 100.
    *
    * @throws std::invalid_argument if capacity is 0.
    */
    BlockingBoundedQueue(size_t capacity = 100);
    ~BlockingBoundedQueue() = default;

    BlockingBoundedQueue(BlockingBoundedQueue const&) = delete;
    BlockingBoundedQueue& operator=(BlockingBoundedQueue const&) = delete;
    BlockingBoundedQueue(BlockingBoundedQueue&&) noexcept = delete;
    BlockingBoundedQueue& operator=(BlockingBoundedQueue&&) noexcept = delete;

public:

    /**
    * @brief Enqueues a new element into the queue.
    *
    * If the queue is full, this call will block until space is available.
    *
    * @param value The value to enqueue (passed by value).
    */
    void enqueue(T const& value);
    void enqueue(T&& value);

    /**
    * @brief Enqueues an element at the front of the queue.
    *
    * Only the class specified by the Privileged template parameter can call this.
    *
    * @param value The value to enqueue at the front (passed by value).
    * @param token The token that restricts access to this function.
    */
    void enqueue_front(T const& value, [[maybe_unused]]Token<Privileged> const& token);
    void enqueue_front(T&& value, [[maybe_unused]]Token<Privileged> const& token);

    /**
    * @brief Dequeues an element from the queue.
    *
    * If the queue is empty, this call will block until an element is available.
    *
    * @return The dequeued element.
    */
    T dequeue();

    /**
    * @brief Clears all elements from the queue.
    */
    void clear();

    /**
    * @brief Checks if the queue is empty.
    *
    * @return true if the queue contains no elements, false otherwise.
    */
    bool is_empty() const;

    /**
    * @brief Checks if the queue is full.
    *
    * @return true if the queue is at capacity, false otherwise.
    */
    bool is_full() const;

    /**
    * @brief Gets the current number of elements in the queue.
    *
    * @return The number of elements stored in the queue.
    */
    size_t size() const;

    /**
    * @brief Gets the capacity of the queue.
    *
    * @return The maximum number of elements the queue can hold.
    */
    size_t capacity() const;

private:

    /**
    * @brief Internal helper function for enqueuing a value.
    *
    * @param value The value to enqueue.
    */
    void enqueue_impl(T&& value);

    /**
    * @brief Internal helper function for enqueuing a value at front.
    *
    * @param value The value to enqueue.
    */
    void enqueue_front_impl(T&& value);

private:

    std::atomic<size_t> m_num_of_elems;
    size_t m_capacity;
    size_t m_head;
    size_t m_tail;
    alignas(T) std::unique_ptr<std::byte[]> m_elements;

    mutable std::mutex m_mutex;
    std::condition_variable m_not_empty;
    std::condition_variable m_not_full;
};

} // namespace mt

#include "blocking_queue.txx"

#endif // MT_BLOCKING_QUEUE_HPP