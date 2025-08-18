#include "cpu.hpp"
#include <stddef.h> // for size_t
#include <stack>
#include <vector>

namespace bf {

CPU::CPU(Memory& memory, IO& io)
    : m_memory{memory}
    , m_io{io}
    , m_program{}
    , m_instruction_pointer{0}
    {}


void CPU::load_program(std::vector<OpCode> const& program) 
{
    m_program = program;
    m_instruction_pointer = 0;
}


void CPU::execute() 
{
    while (m_instruction_pointer < m_program.size()) {
        Instruction instruction_code = m_program[m_instruction_pointer].ins;
        (this->*m_op_codes[static_cast<size_t>(instruction_code)])(m_program[m_instruction_pointer].data);
        ++m_instruction_pointer;
    }
}


void CPU::inc(int data)
{
    m_memory.write(m_memory.read() + data);
}


void CPU::dec(int data)
{
    m_memory.write(m_memory.read() - data);
}


void CPU::left(int data)
{
    for (int i = 0; i < data; i++)
    {
        m_memory.move_left();
    }
}


void CPU::right(int data)
{
    for (int i = 0; i < data; i++)
    {
        m_memory.move_right();
    }
}


void CPU::write(int data)
{
    ++data;
    char c = m_io.read_buffered();
    // char c = m_io.read_stream();
    m_memory.write(c);
}


void CPU::read(int data)
{
    ++data;
    m_io.write_buffered(m_memory.read());
    // m_io.write_stream(m_memory.read());
}


void CPU::begin_loop(int data)
{
    if (m_memory.read() == 0) {
        m_instruction_pointer = data;
    }
}


void CPU::end_loop(int data)
{
    if (m_memory.read() != 0) {
        m_instruction_pointer = data;
    }
}


} // namespace bf
