#ifndef BF_OPTIMIZER_HPP
#define BF_OPTIMIZER_HPP

#include <string>
#include <vector>
#include "types.hpp"


namespace bf {


class Optimizer 
{

public:

    Optimizer() = delete;

public:
    
    static void consec_inc_dec(std::vector<OpCode>& optimized, std::vector<OpCode> const& code);

};

} // namespace bf

#endif // BF_OPTIMIZER_HPP
