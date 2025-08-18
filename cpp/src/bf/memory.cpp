#include "memory.hpp"
#include <stddef.h> // for size_t

namespace bf {

Memory::Memory(size_t initial_size, size_t block_size)
    : m_data(initial_size, 0)
    , m_block_size{block_size}
    , m_pointer{0}
    {}


char Memory::read() const 
{
    return m_data[m_pointer];
}


void Memory::write(char value) 
{
    m_data[m_pointer] = value;
}


void Memory::move_left() 
{
    if (m_pointer == 0) {
        prepend_block();
    }
    --m_pointer;
}


void Memory::move_right() 
{
    ++m_pointer;
    if (m_pointer >= m_data.size()) {
        append_block();
    }
}


void Memory::prepend_block() 
{
    m_data.insert(m_data.begin(), m_block_size, 0);
    m_pointer += m_block_size;
}


void Memory::append_block() 
{
    m_data.resize(m_data.size() + m_block_size, 0);
    
}

} // namespace bf
