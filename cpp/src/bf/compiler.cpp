#include "compiler.hpp"
#include <stddef.h> // for size_t
#include <string>
#include <vector>
#include <stack>

#include "optimizer.hpp"

namespace bf {

static OpCode parse(char c);
static void precompute_brackets(std::vector<OpCode> &program);

/* API Functions */

std::vector<OpCode> Compiler::compile(std::string const& source, int opt_level)
{
    std::vector<OpCode> compiled;
    compiled.reserve(source.size());

    int count = 0;

    for (char c : source) {
        if (c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',') {
            compiled.push_back(parse(c));
        }
        else if (c == '[') {
            ++count;
            compiled.push_back({Instruction::BEGIN_LOOP, 0});
        }
        else if(c == ']') {
            --count;
            if (count < 0) {
                break;
            }
            compiled.push_back({Instruction::END_LOOP, 0});
        }
    }

    if(count != 0){
        return std::vector<OpCode>();
    }

    if (opt_level > 0)
    {
        std::vector<OpCode> optimized;
        bf::Optimizer::consec_inc_dec(optimized, compiled);
        compiled.swap(optimized);
    }

    precompute_brackets(compiled);

    return compiled;

}

/* Static Functions */

static void precompute_brackets(std::vector<OpCode> &program)
{
    std::stack<size_t> stack;

    for (size_t i = 0; i < program.size(); ++i) {
        if (program[i].ins == Instruction::BEGIN_LOOP) {
            stack.push(i);
        } else if (program[i].ins == Instruction::END_LOOP) {
            if (stack.empty()) return;
            size_t open_pos = stack.top();
            stack.pop();
            program[open_pos].data = i;
            program[i].data = open_pos;
        }
    }
}


static OpCode parse(char c)
{
    OpCode opcode;
    switch (c)
    {
    case '+':
        opcode.ins = Instruction::INC;
        opcode.data = 1;
        break;
    
    case '-':
        opcode.ins = Instruction::DEC;
        opcode.data = 1;
        break;
    
    case '>':
        opcode.ins = Instruction::RIGHT;
        opcode.data = 1;
        break;
    
    case '<':
        opcode.ins = Instruction::LEFT;
        opcode.data = 1;
        break;
    
    case '.':
        opcode.ins = Instruction::READ;
        opcode.data = 0;
        break;
    
    case ',':
        opcode.ins = Instruction::WRITE;
        opcode.data = 0;
        break;
    
    default:
        break;
    }

    return opcode;
}



} // namespace bf
