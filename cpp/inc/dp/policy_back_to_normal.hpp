#ifndef DP_POLICY_BACK_TO_NORMAL_HPP
#define DP_POLICY_BACK_TO_NORMAL_HPP


#include "token.hpp"

namespace dp {


template <typename T>
class PolicyBackToNormal {

public:

    explicit PolicyBackToNormal(size_t initial_limit);

    template <typename RampUpPolicy>
    void handle_return(ObjectPool<T, RampUpPolicy, PolicyBackToNormal>& pool);

private:

    size_t m_initial_limit;

};


template <typename T>
PolicyBackToNormal<T>::PolicyBackToNormal(size_t initial_limit)
: m_initial_limit(initial_limit)
{}


template <typename T> 
template <typename RampUpPolicy>
void PolicyBackToNormal<T>::handle_return(ObjectPool<T, RampUpPolicy, PolicyBackToNormal>& pool) 
{
    Token<PolicyBackToNormal> token;
    while (pool.size() > m_initial_limit) {
        pool.remove_one(token);
    }
}


} // namespace dp

#endif // DP_POLICY_BACK_TO_NORMAL_HPP