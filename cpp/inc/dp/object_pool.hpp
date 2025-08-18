#ifndef DP_OBJECT_POOL_HPP
#define DP_OBJECT_POOL_HPP

#include <stack>
#include <memory>
#include <cstddef>
#include <functional>
#include "token.hpp"
#include "factory_method.hpp"


namespace dp {


/**
* @brief An object pool that manages a fixed number of reusable objects.
* 
* @tparam T The type of object managed by the pool.
* @tparam RampUpPolicy The policy used to handle pool expansion.
*/
template <typename T, typename RampUpPolicy, typename BackToNormalPolicy>
class ObjectPool : public std::enable_shared_from_this<ObjectPool<T, RampUpPolicy, BackToNormalPolicy>>{

public:


    /**
    * @brief Constructs an object pool with an initial number of objects.
    * 
    * @param initial_limit The initial size of the pool.
    * @param policy The expansion policy used when the pool is empty. Policy must implement 'handle_empty' function!
    * @param factory A factory method that creates new objects.
    */
    static std::shared_ptr<ObjectPool> create_pool(size_t initial_limit, RampUpPolicy const&  rampUpPolicy,
            BackToNormalPolicy const& backToNormal, Creator<T>& factory_method);


    ~ObjectPool() = default;
    ObjectPool(ObjectPool const&) = delete; 
    ObjectPool& operator=(ObjectPool const&) = delete; 
    ObjectPool(ObjectPool&&) noexcept = delete;
    ObjectPool& operator=(ObjectPool&&) noexcept = delete;  

public:

    using CustomDeleterUptr = std::unique_ptr<T, std::function<void(T*)>>;


    /**
    * @brief Retrieves an object from the pool. If the pool is empty, policy applied.
    * 
    * @return A unique pointer to the object with a custom deleter that returns it to the pool.
    * @throws std::runtime_error If expansion is not allowed and the pool is empty.
    */
    CustomDeleterUptr take();


    /**
    * @brief Creates one object in the pool, requires Token<RampUpPolicy>.
    * @throws std::runtime_error if we exceed the limit.
    */
    void create([[maybe_unused]] Token<RampUpPolicy> const& token);



    /**
    * @brief Removes one object from the pool, requires Token<BackToNormalPolicy>.
    *        The back-to-normal policy might call this to shrink the pool.
    */
    void remove_one([[maybe_unused]] Token<BackToNormalPolicy> const& token);


    /**
    * @brief Returns the number of available objects in the pool.
    * 
    * @return The current size of the pool.
    */
    inline size_t size(){ return m_pool.size();};


private:

    explicit ObjectPool(size_t initial_limit, RampUpPolicy const&  rampUpPolicy,
            BackToNormalPolicy const& backToNormal, Creator<T>& factory_method);


    /**
    * @brief Expands the pool by creating additional objects.
    * 
    * @param count The number of new objects to add.
    */
    void expand_pool(size_t count);


    /**
    * @brief Returns an object to the pool.
    * 
    * This function is called by the custom deleter when an object is no longer in use.
    * 
    * @param object The object being returned to the pool.
    */
    void release_object(T* object);

private:

    std::stack<std::unique_ptr<T>> m_pool;
    std::weak_ptr<ObjectPool<T, RampUpPolicy, BackToNormalPolicy>> m_weak_ref;
    RampUpPolicy m_ramp_up_policy;
    BackToNormalPolicy m_back_to_normal_policy;
    Creator<T>& m_factory;
    size_t m_limit;

};



} // namespace dp

#include "object_pool.txx"

#endif // DP_OBJECT_POOL_HPP