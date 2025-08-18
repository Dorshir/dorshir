#include "intstack.hpp"
#include <cstdlib>      // calloc, free
#include <stdexcept>    // overflow_error, underflow_error, bad_alloc
#include <climits>      // INT_MAX

#define HAS_ENOUGH_CAP(this_ptr, other) ((this_ptr->m_capacity - this_ptr->m_size) >= (other.m_size))

namespace ds {

IntStack::IntStack(size_t capacity)
: m_capacity{capacity}
, m_size{0}
, m_elements{(int*)::calloc(capacity, sizeof(*m_elements))}
{
    if (m_elements == nullptr) {
        throw std::bad_alloc();
    }
}

IntStack::~IntStack()
{
    ::free(m_elements);
}

IntStack::IntStack(IntStack const& other)
: m_capacity{other.m_capacity}
, m_size{other.m_size}
, m_elements{(int*)::calloc(m_capacity, sizeof(*m_elements))}
{
    for (size_t i = 0; i < m_size; ++i) {
        m_elements[i] = other.m_elements[i];
    }
}

void IntStack::swap(IntStack& other) 
{
	std::swap(m_elements, other.m_elements);
	std::swap(m_capacity, other.m_capacity);
	std::swap(m_size, other.m_size);
}

IntStack& IntStack::operator=(IntStack const& other)
{
    if (this != &other) {
		IntStack copy{other};
		swap(copy);
	}
	
	return *this;
}

void IntStack::push(int num)
{
    if(is_full(*this)){
        throw std::overflow_error("Overflow in IntStack::push");
    }

    m_elements[m_size++] = num;
}

int IntStack::pop()
{
    if(is_empty(*this)){
        throw std::underflow_error("Underflow in IntStack::pop");
    }
    return m_elements[--m_size];
}

int IntStack::top()
{
    if(is_empty(*this)){
        return INT_MAX;
    }
    return m_elements[m_size - 1];
}

size_t IntStack::capacity() const
{
    return m_capacity;
}

size_t IntStack::size() const
{
    return m_size;
}

void IntStack::drain(IntStack & other)
{
    if (this != &other) {
		// if (!ensure_capacity(other.m_capacity) ) {
		// 	return;
		// }
		
        for (size_t i = 0; i < other.m_size; ++i) {
            this->push(other.m_elements[i]);
        }
        other.m_size = 0;
    }
}


/* Global Functions */

bool is_empty(IntStack& stack)
{
    return stack.size() == 0;
}

bool is_full(IntStack& stack)
{
    return stack.size() == stack.capacity();
}

void swap(IntStack& first_stack, IntStack& second_stack)
{
    IntStack temp = first_stack;
    first_stack = second_stack;
    second_stack = temp;
}

}
