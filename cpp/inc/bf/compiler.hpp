#ifndef BF_COMPILER_HPP
#define BF_COMPILER_HPP

#include <string>
#include <vector>
#include "types.hpp"

namespace bf {

class Compiler 
{

public:

    Compiler() = delete;

public:
    
    static std::vector<OpCode> compile(std::string const& source, int opt_level = 0);

};

} // namespace bf

#endif // BF_COMPILER_HPP
