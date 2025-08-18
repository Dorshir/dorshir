#include "object_pool.hpp"
#include "token.hpp"
#include "factory_method.hpp"
#include "object_pool_exepction.hpp"


namespace dp {


template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::ObjectPool(size_t initial_limit, RampUpPolicy const&  ramp_up_policy,
            BackToNormalPolicy const& back_to_normal_policy, Creator<T>& factory_method)
: m_ramp_up_policy{ramp_up_policy}
, m_back_to_normal_policy{back_to_normal_policy}
, m_factory{factory_method}
, m_limit{initial_limit}
{
    expand_pool(initial_limit);
}


template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
std::shared_ptr<ObjectPool<T, RampUpPolicy, BackToNormalPolicy>>
ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::create_pool(
    size_t initial_limit,
    RampUpPolicy const& ramp_up_policy,
    BackToNormalPolicy const& back_to_normal_policy,
    Creator<T>& factory_method)
{
    auto sp = std::shared_ptr<ObjectPool<T, RampUpPolicy, BackToNormalPolicy>>(
        new ObjectPool(initial_limit, ramp_up_policy, back_to_normal_policy, factory_method)
    );

    sp->m_weak_ref = sp;

    return sp;
}



template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
typename ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::CustomDeleterUptr ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::take() 
{
    if (m_pool.empty()) {
        m_ramp_up_policy.handle_empty(*this);
        if (m_pool.empty()) {
            return {};
        }
    }
    std::unique_ptr<T> object = std::move(m_pool.top());
    m_pool.pop();

    return ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::CustomDeleterUptr(
        object.release(), 
        [this](T* object) {
            this->release_object(object); 
        });
}


template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
void ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::release_object(T* object) 
{
    if (object) {
        m_pool.push(std::unique_ptr<T>(object));
        m_back_to_normal_policy.handle_return(*this);
    }
}


template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
void ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::create([[maybe_unused]]Token<RampUpPolicy> const& token) 
{
    if (m_pool.size() < m_limit) {
        expand_pool(1);
    } 
    else {
        throw dp::ObjectPoolException{"Cannot create more objects"};
    }
}


template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
void ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::remove_one([[maybe_unused]]Token<BackToNormalPolicy> const& token)
{
    if (!m_pool.empty()) {
        m_pool.pop();
    }
}


template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
void ObjectPool<T, RampUpPolicy, BackToNormalPolicy>::expand_pool(size_t count) 
{
    for (size_t idx = 0; idx < count; ++idx) {
        m_pool.push(m_factory.create());
    }
}






} // namespace dp