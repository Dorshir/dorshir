#ifndef DP_POLICY_EXPAND_HPP
#define DP_POLICY_EXPAND_HPP

#include "object_pool.hpp"
#include "token.hpp"

namespace dp {

template <typename T>
class PolicyExpand {

public:

    explicit PolicyExpand(size_t expand_count);
    ~PolicyExpand() = default;
    PolicyExpand(PolicyExpand const&) = default; 
    PolicyExpand& operator=(PolicyExpand const&) = default; 
    PolicyExpand(PolicyExpand&&) noexcept = default;
    PolicyExpand& operator=(PolicyExpand&&) noexcept = default;  

public:

    template <typename RampUpPol, typename BackToNormalPol>
    void handle_empty(ObjectPool<T, RampUpPol, BackToNormalPol>& pool) const;

private:

    size_t m_expand_count;

};

template <typename T>
PolicyExpand<T>::PolicyExpand(size_t expand_count)
: m_expand_count(expand_count) 
{}


template <typename T>
template <typename RampUpPol, typename BackToNormalPol>
void PolicyExpand<T>::handle_empty(ObjectPool<T, RampUpPol, BackToNormalPol>& pool) const
{
    Token<PolicyExpand> token;
    for (size_t i = 0; i < m_expand_count; ++i) {
        pool.create(token);
    }
}


} // namespace dp


#endif // DP_POLICY_EXPAND_HPP