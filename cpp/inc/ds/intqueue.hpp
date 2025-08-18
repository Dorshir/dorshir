#ifndef DS_INTQUEUE_H
#define DS_INTQUEUE_H

#include "intstack.hpp"
#include <cstddef>  // size_t


namespace ds {


class IntQueue {


public:

    explicit IntQueue(size_t capacity = 128);
    ~IntQueue() = default;
    IntQueue(IntQueue const& other) = default;
    IntQueue& operator=(IntQueue const& other) = default;

    /**
     * @brief Enqueue an integer onto the queue.
     * @param num The integer to enqueue.
     * @throws std::overflow_error If the queue is full.
     */
    void enqueue(int num);

    /**
     * @brief Dequeue the bottom integer from the queue.
     * @return The integer at the bottom of the queue.
     * @throws std::underflow_error If the queue is empty.
     */
    int dequeue();

    /**
     * @brief Returns the current size of the queue.
     * @return The number of elements in the queue.
     */
    size_t size() const;

    /**
     * @brief Clears the queue.
     */
    void clear();

    /**
     * @brief Swaps this queue with other queue.
     * @param other The queue to swap with.
     */
    void swap(IntQueue& other);

    /**
     * @brief Checks if a queue is empty.
     * @return True if the queue is empty, false otherwise.
     */
    bool empty() const;

    /**
     * @brief Checks if a queue is full.
     * @return True if the queue is full, false otherwise.
     */
    bool full() const;

private:
    size_t m_capacity;
    size_t m_size;
    IntStack m_push_stack;
    IntStack m_pop_stack;
};

} // namespace ds

#endif // DS_INTQUEUE_H
