#include <type_traits>
#ifndef SFINE_HPP
#define SFINE_HPP

namespace meta {

namespace cpp11 {

template <typename C>
typename std::enable_if<!std::is_arithmetic<C>::value, typename C::value_type>::type
square(C const& container)
{
    typename C::value_type result{};
    for (auto& elem : container) {
        result += elem * elem;
    }
    return result;
}

template <typename N>
typename std::enable_if<std::is_arithmetic<N>::value, N>::type
square(N const& n)
{
    return n * n;
}

} // namespace cpp11

} // namespace meta

#endif // SFINE_HPP