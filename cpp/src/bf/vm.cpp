#include "vm.hpp"
#include "compiler.hpp"
#include <string>

namespace bf {

VM::VM(size_t memory_size, size_t block_size)
    : m_memory{memory_size, block_size}
    , m_io{}
    , m_cpu{m_memory, m_io}
    {}


void VM::load_and_compile(const std::string& source) 
{
    std::vector<OpCode> compiled = Compiler::compile(source);
    load_program(compiled);
}


void VM::load_program(std::vector<OpCode> const& program) 
{
    m_cpu.load_program(program);
}


void VM::run() 
{
    m_cpu.execute();
}


IO& VM::get_io()
{
    return m_io;
}

} // namespace bf
