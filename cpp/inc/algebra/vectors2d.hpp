#include <cstdlib>  // for rand()
#include <cassert>  // for assert

namespace algebra {

struct Vector2f 
{
    float x;
    float y;

    explicit Vector2f(float x = 0.0, float y = 0.0)
    : x{x}
    , y{y}
    {}

    float operator[](int index) const{
        assert(index <= 1 && index >= 0);
        return index == 0 ? x : y;
    }
};

struct Vector2i 
{
    int x;
    int y;

    explicit Vector2i(int x = 0, int y = 0)
    : x{x}
    , y{y}
    {}

    explicit Vector2i(float x, float y)
    : x{static_cast<int>(x)}
    , y{static_cast<int>(y)}
    {}

    int operator[](int index) const{
        assert(index <= 1 && index >= 0);
        return index == 0 ? x : y;
    }
};


static float generate_random_number(int low, int high)
{
    return low + static_cast<float>(rand()) / RAND_MAX * (high - low);
}

template<typename T>
T generate(int low = -100, int high = 100)
{
    return T{generate_random_number(low, high), generate_random_number(low, high)};
}

template<typename T>
void fill(T * begin, T const* end)
{
    assert(begin && end);
    assert(begin <= end);

    while (begin != end) {
        *begin = generate<T>();
        ++begin;
    }
}

template<typename T, typename F>
void transform(T const * begin, T const* end, T * output_begin, F func)
{
    assert(begin && end && output_begin);
    assert(begin <= end);

    while (begin != end) {
        *output_begin = func(*begin);
        ++begin;
        ++output_begin;
    }
}

template<typename T, typename U, typename R, typename F>
void transform(T const* first_begin, T const* first_end, U const* second_begin, R * output_begin, F bi_func)
{
    assert(first_begin && first_end && second_begin && output_begin);
    assert(first_begin <= first_end);

    while (first_begin != first_end) {
        *output_begin = bi_func(*first_begin, *second_begin);
        ++first_begin;
        ++second_begin;
        ++output_begin;
    }
}

template<typename T, typename F, typename G, typename R>
R transform_reduce(T const * begin, T const* end, F transfoming_func, G reducing_func, R init)
{
    assert(begin && end);
    assert(begin <= end);

    R result = init;
    while (begin != end) {
        R transformed = transfoming_func(*begin);
        result = reducing_func(result, transformed);
        ++begin;
    }
    return result;
}


}