#ifndef DS_LIST_HPP
#define DS_LIST_HPP

#include <cstddef>  // size_t
#include <cstdlib>  // std::swap
#include <cassert>  // assert


namespace ds
{

template<typename T>
class List
{
public:

    struct Node;

public:
    
    explicit List();
    ~List();
    List(List const& other);
    List& operator=(List const& other);


    /**
     * @brief Inserts a new element at the beginning of the List.
     * @param data The data to insert.
     */
    void push_head(T const& data);

    /**
     * @brief Inserts a new element at the end of the List.
     * @param data The data to insert.
     */
    void push_tail(T const& data);

    /**
     * @brief Removes and retrieves the first element of the List.
     * @param r_value Reference to a variable where the removed element's data will be stored.
     * @return True if the element was successfully removed, false if the List is empty.
     */
    bool pop_head(T& r_value);

    /**
     * @brief Removes and retrieves the last element of the List.
     * @param r_value Reference to a variable where the removed element's data will be stored.
     * @return True if the element was successfully removed, false if the List is empty.
     */
    bool pop_tail(T& r_value);

    /**
     * @brief Gets the number of elements in the List.
     * @return The number of elements in the List.
     */
    size_t size() const;

    /**
     * @brief Checks if the List is empty.
     * @return True if the List is empty, false otherwise.
     */
    bool empty() const;

    /**
     * @brief Removes all elements from the List.
     */
    void clear();

    /**
     * @brief Swaps the content of this List with another List.
     * @param other The List to swap with.
     */
    void swap(List& other);

    /**
     * @brief Applies a given action to each element in the List.
     * @tparam A The type of the action, which must be callable with a single parameter of type T.
     * @param action The action to apply to each element.
     */
    template <typename A>
    void for_each(A action);

private:

    Node m_head;
    Node m_tail;
    size_t m_size;
};


}

#include "./list.txx"

#endif // DS_LIST_HPP