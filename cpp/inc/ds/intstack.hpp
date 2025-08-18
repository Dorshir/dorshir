#ifndef DS_INTSTACK_H
#define DS_INTSTACK_H

#include <cstddef>  // size_t

namespace ds {


class IntStack {
public:

    /**
     * @brief Constructs an IntStack with the specified capacity.
     * @param capacity The maximum number of elements the stack can hold. Defaults to 16.
     * @throws std::bad_alloc If memory allocation fails.
     */
    IntStack(size_t capacity = 16);

    /**
     * @brief Destroys the stack and releases allocated memory.
     */
    ~IntStack();

    /**
     * @brief Copy constructor.
     * @param other The stack to copy.
     * @throws std::bad_alloc If memory allocation fails.
     */
    IntStack(IntStack const& other);

    /**
     * @brief Copy assignment operator.
     * @param other The stack to copy.
     * @return A reference to the current stack.
     * @throws std::bad_alloc If memory allocation fails.
     */
    IntStack& operator=(IntStack const& other);

    /**
     * @brief Pushes an integer onto the stack.
     * @param num The integer to push.
     * @throws std::overflow_error If the stack is full.
     */
    void push(int num);

    /**
     * @brief Pops the top integer from the stack.
     * @return The integer at the top of the stack.
     * @throws std::underflow_error If the stack is empty.
     */
    int pop();

    /**
     * @brief Retrieves the top integer from the stack without removing it.
     * @return The integer at the top of the stack, or `INT_MAX` if the stack is empty.
     */
    int top();

    /**
     * @brief Returns the maximum capacity of the stack.
     * @return The capacity of the stack.
     */
    size_t capacity() const;

    /**
     * @brief Returns the current size of the stack.
     * @return The number of elements in the stack.
     */
    size_t size() const;

    /**
     * @brief Drains elements from another stack and pushes them onto the current stack.
     * @param other The stack to drain.
     * @throws std::overflow_error If there is not enough capacity in the current stack.
     */
    void drain(IntStack& other);

    /**
     * @brief Swaps this stack with other stack.
     * @param other The stack to swap with.
     */
    void swap(IntStack& other);

private:
    size_t m_capacity;
    size_t m_size;
    int* m_elements;
};

/**
 * @brief Checks if a stack is empty.
 * @param stack The stack to check.
 * @return True if the stack is empty, false otherwise.
 */
bool is_empty(IntStack& stack);

/**
 * @brief Checks if a stack is full.
 * @param stack The stack to check.
 * @return True if the stack is full, false otherwise.
 */
bool is_full(IntStack& stack);


} // namespace ds

#endif // DS_INTSTACK_H
