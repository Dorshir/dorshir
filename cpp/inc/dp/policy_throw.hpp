#ifndef DP_POLICY_THROW_HPP
#define DP_POLICY_THROW_HPP

#include "object_pool.hpp"
#include "token.hpp"
#include "object_pool_exepction.hpp"

namespace dp {

template <typename T>
class PolicyThrow {

public:

    template <typename RampUpPol, typename BackToNormalPol>
    void handle_empty(ObjectPool<T, RampUpPol, BackToNormalPol>& pool) const;

};

template <typename T>
template <typename RampUpPol, typename BackToNormalPol>
void PolicyThrow<T>::handle_empty(ObjectPool<T, RampUpPol, BackToNormalPol>&) const 
{
    throw dp::ObjectPoolException("Pool is empty!");
}


} // namespace dp


#endif // DP_POLICY_THROW_HPP