#include "mu_test.h"
#include "vectors2d.hpp"
#include <cmath>

static float compute_magnitude(const algebra::Vector2f& v);
static float sum_floats(float acc, float value);
static algebra::Vector2f increment_vector(const algebra::Vector2f& v);
static algebra::Vector2f add_vectors(const algebra::Vector2f& a, const algebra::Vector2f& b);


BEGIN_TEST(generate_vector2f_range_check)

    algebra::Vector2f vf = algebra::generate<algebra::Vector2f>();
    ASSERT_THAT(vf[0] >= -100.0f && vf[0] <= 100.0f);
    ASSERT_THAT(vf[1] >= -100.0f && vf[1] <= 100.0f);

END_TEST


BEGIN_TEST(generate_vector2i_range_check)

    int low = -50;
    int high = 50;
    algebra::Vector2i vi = algebra::generate<algebra::Vector2i>(low, high);
    ASSERT_THAT(vi[0] >= low && vi[0] <= high);
    ASSERT_THAT(vi[1] >= low && vi[1] <= high);

END_TEST


BEGIN_TEST(fill_vector2f)

    const int size = 5;
    algebra::Vector2f arr[size];
    algebra::fill(arr, arr + size);

    for (int i = 0; i < size; ++i) {
        ASSERT_THAT((arr[i])[0] >= -100.0f && (arr[i])[0] <= 100.0f);
        ASSERT_THAT((arr[i])[1] >= -100.0f && (arr[i])[1] <= 100.0f);
    }

END_TEST


BEGIN_TEST(transform_unary_vector2f)

    algebra::Vector2f arrIn[3] = { algebra::Vector2f(1.0f, 2.0f), 
                                   algebra::Vector2f(3.5f, -4.0f), 
                                   algebra::Vector2f(-1.0f, 0.5f) };
    algebra::Vector2f arrOut[3];

    algebra::transform(arrIn, arrIn + 3, arrOut, increment_vector);

    ASSERT_EQUAL((arrOut[0])[0], 2.0f);
    ASSERT_EQUAL((arrOut[0])[1], 3.0f);

    ASSERT_EQUAL((arrOut[1])[0], 4.5f);
    ASSERT_EQUAL((arrOut[1])[1], -3.0f);

    ASSERT_EQUAL((arrOut[2])[0], 0.0f);
    ASSERT_EQUAL((arrOut[2])[1], 1.5f);

END_TEST


BEGIN_TEST(transform_binary_vector2f)

    algebra::Vector2f arrA[2] = { algebra::Vector2f(1.0f, 2.0f), 
                                  algebra::Vector2f(3.5f, 4.5f) };
    algebra::Vector2f arrB[2] = { algebra::Vector2f(2.0f, -1.0f), 
                                  algebra::Vector2f(-1.5f, 0.5f) };
    algebra::Vector2f arrOut[2];

    algebra::transform(arrA, arrA + 2, arrB, arrOut, add_vectors);

    ASSERT_EQUAL((arrOut[0])[0], 3.0f);
    ASSERT_EQUAL((arrOut[0])[1], 1.0f);

    ASSERT_EQUAL((arrOut[1])[0], 2.0f);
    ASSERT_EQUAL((arrOut[1])[1], 5.0f);

END_TEST


BEGIN_TEST(transform_reduce_vector2f)

    algebra::Vector2f arr[3] = { algebra::Vector2f(3.0f, 4.0f), 
                                 algebra::Vector2f(1.0f, 2.0f), 
                                 algebra::Vector2f(6.0f, 8.0f) };

    float total = algebra::transform_reduce(arr, arr + 3, compute_magnitude, sum_floats, 0.0f);

    ASSERT_THAT(total > 17.23f && total < 17.24f);

END_TEST


TEST_SUITE(Vector2d unit tests)

    TEST(generate_vector2f_range_check)
    TEST(generate_vector2i_range_check)
    TEST(fill_vector2f)
    TEST(transform_unary_vector2f)
    TEST(transform_binary_vector2f)
    TEST(transform_reduce_vector2f)

END_SUITE



static float compute_magnitude(const algebra::Vector2f& v) {
    return static_cast<float>(std::sqrt(v[0] * v[0] + v[1] * v[1]));
}

static float sum_floats(float acc, float value) {
    return acc + value;
}

static algebra::Vector2f increment_vector(const algebra::Vector2f& v) {
    return algebra::Vector2f(v[0] + 1.0f, v[1] + 1.0f);
}

static algebra::Vector2f add_vectors(const algebra::Vector2f& a, const algebra::Vector2f& b) {
    return algebra::Vector2f(a[0] + b[0], a[1] + b[1]);
}
