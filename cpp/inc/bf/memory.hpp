#ifndef BF_MEMORY_HPP
#define BF_MEMORY_HPP

#include <vector>
#include <cstddef> // for size_t

namespace bf {

class Memory {

public:

    explicit Memory(size_t size = 65536, size_t block_size = 4096);
    ~Memory() = default;
    Memory(Memory const & other) = default;
    Memory& operator=(Memory const& other) = default;

public:

    char read() const;
    void write(char value);
    void move_left();
    void move_right();

private:

    std::vector<char> m_data;
    size_t m_block_size;
    size_t m_pointer;

private:

    void prepend_block();
    void append_block();

};

} // namespace bf

#endif // BF_MEMORY_HPP
