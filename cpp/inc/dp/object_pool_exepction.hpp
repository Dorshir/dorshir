#ifndef DP_OBJECT_POOL_EXCEPTION_HPP
#define DP_OBJECT_POOL_EXCEPTION_HPP

#include <stdexcept>

namespace dp {

class ObjectPoolException : public std::runtime_error {

public:

    explicit ObjectPoolException(std::string const& message)
    : std::runtime_error(message) 
    {}

};


} // namespace dp

#endif // DP_OBJECT_POOL_EXCEPTION_HPP