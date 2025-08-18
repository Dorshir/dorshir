#ifndef BF_CPU_HPP
#define BF_CPU_HPP

#include <stddef.h> // for size_t
#include <vector>
#include "io.hpp"
#include "types.hpp"
#include "memory.hpp"



namespace bf {

class CPU {
    
public:

    explicit CPU(Memory& memory, IO& io);
    ~CPU() = default;
    CPU(const CPU&) = delete;
    CPU& operator=(const CPU&) = delete;
    
public:

    void load_program(std::vector<OpCode> const& program);
    void execute();

private:

    Memory& m_memory;
    IO& m_io;
    std::vector<OpCode> m_program;
    size_t m_instruction_pointer;


    typedef void (CPU::*InstructionFunction)(int data);
    InstructionFunction m_op_codes[8] = {&CPU::inc, &CPU::dec, &CPU::right, &CPU::left, 
        &CPU::write, &CPU::read, &CPU::begin_loop, &CPU::end_loop};

private:
    
    void inc(int data);
    void dec(int data);
    void left(int data);
    void right(int data);
    void write(int data);
    void read(int data);
    void begin_loop(int data);
    void end_loop(int data);
};

} // namespace bf

#endif // BF_CPU_HPP
