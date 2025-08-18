#ifndef IQ_CONTAINERS_HPP
#define IQ_CONTAINERS_HPP

#include <stack>
#include <functional>
#include <memory>
#include <cstddef>
#include <stdexcept>


namespace iq {

template<typename T>
class MinStack {

public:

using Comparator = std::function<bool(T const&, T const&)>;
using SComparator = std::function<bool(std::shared_ptr<T> const&, std::shared_ptr<T> const&)>;


explicit MinStack(Comparator cmp = nullptr);
~MinStack() = default;
MinStack(MinStack const&) = default;    
MinStack& operator=(MinStack const&) = default;      
MinStack(MinStack&&) noexcept = default;
MinStack& operator=(MinStack&&) noexcept = default;  

public:

void push(std::shared_ptr<T> const& element);
std::shared_ptr<T> pop();
std::shared_ptr<T> const& min() const;

private:

SComparator m_comparator;
std::stack<std::shared_ptr<T>> m_elements;
std::stack<std::shared_ptr<T>> m_min_elements;

};

template<typename T>
MinStack<T>::MinStack(Comparator cmp)
{
    if (cmp != nullptr) {
        m_comparator = [cmp](std::shared_ptr<T> const& a, std::shared_ptr<T> const& b) {
            return cmp(*a, *b);
        };
    }
    else {
        m_comparator = [](std::shared_ptr<T> const& a, std::shared_ptr<T> const& b) {
            return *a < *b;
        };
    }
}

template <typename T>
void MinStack<T>::push(std::shared_ptr<T> const& element)
{
    if (!element) {
        throw std::runtime_error("Invalid input: nullptr");
    }

    if (m_min_elements.empty() || m_comparator(element, m_min_elements.top())) {
        m_min_elements.push(element);
    }
    m_elements.push(element);
}


template <typename T>
std::shared_ptr<T> MinStack<T>::pop() 
{
    if (m_elements.empty()) {
        throw std::runtime_error("POP: Stack is empty!");
    }
    
    std::shared_ptr<T> ret = m_elements.top();
    if (!m_min_elements.empty() && m_min_elements.top() == ret) {
        m_min_elements.pop();
    }

    m_elements.pop();

    return ret;
}


template <typename T>
std::shared_ptr<T> const& MinStack<T>::min() const
{
    if (m_min_elements.empty()) {
        throw std::runtime_error("MIN: Stack is empty!");
    }
    return m_min_elements.top();
}


////////////////////////////////////////////////////////////////////////////////


template<typename T>
class ResettableSlots {

public:

explicit ResettableSlots(size_t number_of_slots = 1, std::shared_ptr<T> const& initial_value = nullptr);
~ResettableSlots() = default;
ResettableSlots(ResettableSlots const&) = default;
ResettableSlots& operator=(ResettableSlots const&) = default;    
ResettableSlots(ResettableSlots&&) noexcept = default;         
ResettableSlots& operator=(ResettableSlots&&) noexcept = default; 

public:

void set(size_t idx, std::shared_ptr<T> const& value);
std::shared_ptr<T> get(size_t idx) const;
void reset(std::shared_ptr<T> const& reset_value);

private:

struct VersionLink {

std::shared_ptr<T> element;
size_t version;

explicit VersionLink(std::shared_ptr<T> const& elem, size_t vers);
~VersionLink() = default;

};

private:

std::shared_ptr<T> m_reset_value;
std::vector<VersionLink> m_elements;
size_t m_curr_version;

};


template<typename T>
ResettableSlots<T>::VersionLink::VersionLink(std::shared_ptr<T> const& elem, size_t vers)
: element{elem}
, version{vers}
{}


template<typename T>
ResettableSlots<T>::ResettableSlots(size_t number_of_slots, std::shared_ptr<T> const& initial_value)
: m_reset_value{initial_value}
, m_elements(number_of_slots, VersionLink{initial_value, 0})
, m_curr_version{0}
{}


template <typename T>
void ResettableSlots<T>::set(size_t idx, std::shared_ptr<T> const& value)
{
    if (!value) {
        throw std::runtime_error("Invalid input: nullptr");
    }
    
    if (idx >= m_elements.size()) {
        throw std::out_of_range("Index is out of bounds");
    }

    m_elements[idx].element = value;
    m_elements[idx].version = m_curr_version;
}


template <typename T>
std::shared_ptr<T> ResettableSlots<T>::get(size_t idx) const
{
    if (idx >= m_elements.size()) {
        throw std::out_of_range("Index is out of bounds");
    }

    if (m_curr_version > m_elements[idx].version) {
        return m_reset_value;
    }

    return m_elements[idx].element;
}


template <typename T>
void ResettableSlots<T>::reset(std::shared_ptr<T> const& reset_value)
{
    if (!reset_value) {
        throw std::runtime_error("Invalid input: nullptr");
    }

    m_reset_value = reset_value;
    ++m_curr_version;
}


}


#endif // IQ_CONTAINERS_HPP