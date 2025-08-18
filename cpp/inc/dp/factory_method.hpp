#ifndef DP_FACTORY_METHOD_HPP
#define DP_FACTORY_METHOD_HPP

namespace dp {


/**
* @brief Abstract base class representing an object.
* 
* This class serves as a base interface for all objects that need to be instantiated
* and managed dynamically. Derived classes must implement the `use()` method.
*/
class Object {
public:
    Object() = default;
    virtual ~Object() = default;
    virtual void use() = 0;
};

/**
* @brief Generic factory interface for creating objects of type T.
* 
* @tparam T The type of object that the creator produces.
*/
template <typename T>
class Creator {
public:
    virtual ~Creator() = default;
    virtual std::unique_ptr<T> create() = 0;
};


} // namespace dp

#endif // DP_FACTORY_METHOD_HPP