#include "optimizer.hpp"
#include "compiler.hpp"

void bf::Optimizer::consec_inc_dec(std::vector<OpCode> &optimized, std::vector<OpCode> const &code)
{
    int set_by = 0;
    for (OpCode opcode : code) {
        if(opcode.ins == Instruction::INC){
            ++set_by;
        }
        else if(opcode.ins == Instruction::DEC){
            --set_by;
        }
        else{
            if(set_by != 0){
                optimized.push_back({Instruction::INC, set_by});
                set_by = 0;
            }
            optimized.push_back(opcode);
        }
    }
}