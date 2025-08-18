#include "intqueue.hpp"
#include "intstack.hpp"

#include <cstddef>  // for size_t
#include <utility>  // for std::swap
#include <cassert>  // for assert
#include <cstdlib>


namespace ds {


IntQueue::IntQueue(size_t capacity)
: m_capacity{capacity}
, m_size{0}
, m_push_stack{capacity}
, m_pop_stack{capacity}
{
    assert(capacity > 0);
}

void IntQueue::enqueue(int num)
{
    assert(!is_full(m_push_stack));
    m_push_stack.push(num);
    ++m_size;
}

int IntQueue::dequeue()
{
    assert(!empty());

    if(is_empty(m_pop_stack)){
        while(!is_empty(m_push_stack)){
            m_pop_stack.push(m_push_stack.pop());
        }
    }

    --m_size;

    return m_pop_stack.pop();
}

size_t IntQueue::size() const
{
    return m_size;
}


void IntQueue::clear()
{
    while(!is_empty(m_push_stack)){
        m_push_stack.pop();
    }

    while(!is_empty(m_pop_stack)){
        m_pop_stack.pop();
    }

    m_size = 0;
}

void IntQueue::swap(IntQueue& other)
{
    std::swap(m_capacity, other.m_capacity);
	std::swap(m_size, other.m_size);
    std::swap(m_push_stack, other.m_push_stack);
    std::swap(m_pop_stack, other.m_pop_stack);
}


bool IntQueue::empty() const
{
    return (m_size == 0);
}

bool IntQueue::full() const
{
    return (m_size == m_capacity);
}


}
