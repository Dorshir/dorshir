#include "mu_test.h"

#include "fixed_vector.hpp"

BEGIN_TEST(can_create_fixed_vector_and_use_it)
	ds::FixedVector v{128};

	v.at(12) = 1;
	v.at(64) = 2;

	ASSERT_EQUAL(v.at(1), 0);
	ASSERT_EQUAL(v.at(12), 1);
	ASSERT_EQUAL(v.at(64), 2);
	ASSERT_EQUAL(v.at(120), 0);

END_TEST


BEGIN_TEST(work_with_const_vector)
	size_t const N = 1024;
	ds::FixedVector const cv{N};

	ASSERT_EQUAL(cv.size(), N);
	for(size_t i = 0; i < N; ++i) {
		ASSERT_EQUAL(cv.at(i), 0);
	}
END_TEST


BEGIN_TEST(can_copy_vector)
	size_t const N = 1024;
	ds::FixedVector v{N};
	for (size_t i = 0; i < N; ++i) {
		v.at(i) = i*i;
	}

	{
		ds::FixedVector copy{v};
		ASSERT_EQUAL(copy.size(), v.size());
		for (size_t i = 0; i < N; ++i) {
			ASSERT_EQUAL(v.at(i), copy.at(i));
		}
	}
END_TEST


BEGIN_TEST(can_assign_vector)
	size_t const N = 1024;
	ds::FixedVector v{N};
	for (size_t i = 0; i < N; ++i) {
		v.at(i) = i*i;
	}

	ds::FixedVector w{N/3};
	w = v;

	ASSERT_EQUAL(w.size(), v.size());
	for (size_t i = 0; i < N; ++i) {
		ASSERT_EQUAL(v.at(i), w.at(i));
	}
END_TEST

BEGIN_TEST(can_self_assign_vector)
	size_t const N = 1024;
	ds::FixedVector v{N};

	for (size_t i = 0; i < N; ++i) {
		v.at(i) = i*i;
	}
	ds::FixedVector copy{v};

	v = v;

	ASSERT_EQUAL(copy.size(), v.size());
	for (size_t i = 0; i < N; ++i) {
		ASSERT_EQUAL(v.at(i), copy.at(i));
	}
END_TEST

BEGIN_TEST(can_cascade_assign_vector)
	size_t const N = 1024;
	ds::FixedVector v{N};

	for (size_t i = 0; i < N; ++i) {
		v.at(i) = i*i;
	}

	ds::FixedVector w;
	ds::FixedVector s;

	w = (s = v);

	ASSERT_EQUAL(w.size(), v.size());
	for (size_t i = 0; i < N; ++i) {
		ASSERT_EQUAL(w.at(i), v.at(i));
	}
END_TEST


TEST_SUITE(FixedVEctor unit tests)
	TEST(can_create_fixed_vector_and_use_it)
	TEST(work_with_const_vector)
	TEST(can_copy_vector)
	TEST(can_assign_vector)
	TEST(can_self_assign_vector)
	TEST(can_cascade_assign_vector)
END_SUITE
