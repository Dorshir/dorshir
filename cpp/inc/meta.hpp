#ifndef META_HPP
#define META_HPP

#include <cstdint>
#include <cstddef>

namespace meta {

template<uint64_t N>
struct CountBits
{
    static constexpr size_t value = N & 1 + CountBits<(N >> 1)>::value;
};

template<>
struct CountBits<0>
{
    static constexpr size_t value = 0;
};


template<int64_t Base, int64_t Exponent>
struct PositivePower 
{
    static constexpr double value = Base * PositivePower<Base, Exponent-1>::value;
};

template<int64_t Base>
struct PositivePower<Base, 0>
{
    static constexpr double value = 1.0;
}; 

template<int64_t Base, int64_t Exponent>
struct Power 
{
    static constexpr int64_t abs_exponent = (Exponent < 0) ? -Exponent : Exponent;
    static constexpr double value = (Exponent < 0) 
        ? (1.0 / PositivePower<Base, abs_exponent>::value)
        : PositivePower<Base, abs_exponent>::value;
};




template<int64_t Base, int64_t Exponent>
double power_v = Power<Base, Exponent>::value;

template<uint64_t N>
size_t bit_count_v = CountBits<N>::value;

} // namespace udl

#endif // META_HPP