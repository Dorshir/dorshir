#include "mu_test.h"
#include "challenger_1.hpp"

BEGIN_TEST(fibg_test)

	ASSERT_EQUAL_INT(fibg(), 1);
	ASSERT_EQUAL_INT(fibg(), 1);
	ASSERT_EQUAL_INT(fibg(), 2);
	ASSERT_EQUAL_INT(fibg(), 3);
	ASSERT_EQUAL_INT(fibg(), 5);
	ASSERT_EQUAL_INT(fibg(), 8);
END_TEST

BEGIN_TEST(factorial_test)

	ASSERT_EQUAL_INT(factorial(0), 1);
	ASSERT_EQUAL_INT(factorial(1), 1);

	ASSERT_EQUAL_INT(factorial(5), 120);
	ASSERT_EQUAL_INT(factorial(10), 3628800);

	ASSERT_EQUAL_INT(factorial(12), 479001600);
END_TEST

BEGIN_TEST(sort_balls_test_first_red)

	std::vector<Ball> balls = {
		{Red, 10, true}, 
		{Black, 15, false}, 
		{White, 12, true}, 
		{Red, 20, true}, 
		{Black, 18, false}, 
		{White, 25, false}
	};

	sort_balls(balls);

	ASSERT_EQUAL_INT(balls[0].color, White);
	ASSERT_EQUAL_INT(balls[1].color, White);
	ASSERT_EQUAL_INT(balls[2].color, Red);
	ASSERT_EQUAL_INT(balls[3].color, Red);
	ASSERT_EQUAL_INT(balls[4].color, Black);
	ASSERT_EQUAL_INT(balls[5].color, Black);
END_TEST

BEGIN_TEST(sort_balls_test_first_black_last_white)

	std::vector<Ball> balls = {
		{Black, 10, true}, 
		{Black, 15, false}, 
		{White, 12, true}, 
		{Red, 20, true}, 
		{Black, 18, false}, 
		{White, 25, false}
	};

	sort_balls(balls);

	ASSERT_EQUAL_INT(balls[0].color, White);
	ASSERT_EQUAL_INT(balls[1].color, White);
	ASSERT_EQUAL_INT(balls[2].color, Red);
	ASSERT_EQUAL_INT(balls[3].color, Black);
	ASSERT_EQUAL_INT(balls[4].color, Black);
	ASSERT_EQUAL_INT(balls[5].color, Black);
END_TEST

BEGIN_TEST(empty_vector_sort_test)

	std::vector<Ball> empty_balls;
	sort_balls(empty_balls);

	ASSERT_THAT(empty_balls.empty());
END_TEST

BEGIN_TEST(single_element_sort_test)

	std::vector<Ball> one_ball = {{Red, 10, true}};
	sort_balls(one_ball);

	ASSERT_EQUAL_INT(one_ball[0].color, Red);
	ASSERT_EQUAL_INT(one_ball[0].radius, 10);
	ASSERT_THAT(one_ball[0].is_solid);
END_TEST

TEST_SUITE(Testing challenger_1 functions)
	TEST(fibg_test)
	TEST(factorial_test)
	TEST(sort_balls_test_first_red)
    TEST(sort_balls_test_first_black_last_white)
	TEST(empty_vector_sort_test)
	TEST(single_element_sort_test)
END_SUITE
