#ifndef BF_TYPES_HPP
#define BF_TYPES_HPP

#include <vector>

enum class Instruction 
{
    INC, DEC, LEFT, RIGHT, WRITE, READ, BEGIN_LOOP, END_LOOP
};

struct OpCode 
{
    Instruction ins;
    int data;
};

#endif // BF_TYPES_HPP
