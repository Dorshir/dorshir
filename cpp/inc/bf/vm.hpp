#ifndef BF_VM_HPP
#define BF_VM_HPP

#include <string>
#include <stddef.h>  // for size_t
#include "cpu.hpp"
#include "memory.hpp"
#include "io.hpp"

namespace bf {

class VM {
public:

    explicit VM(size_t memory_size = 65536, size_t block_size = 4096);
    ~VM() = default;
    VM(VM const&) = default;
    VM& operator=(VM const&) = default;

public:

    void load_and_compile(std::string const& source);
    void load_program(std::vector<OpCode> const& program);
    void run();
    IO& get_io();

private:

    Memory m_memory;
    IO m_io;
    CPU m_cpu;

};

} // namespace bf

#endif // BF_VM_HPP
