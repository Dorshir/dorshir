#include "mu_test.h"

int count_pairs(uint64_t num)
{
    bool flag = false;
    int result = 0;
    uint64_t mask = 1; 

    while (num) {
        if (num & mask) {
            if (flag) {
                result += 1;
            }
            else {
                flag = true;
            }
        }
        else {
            flag = false;
        }
        num >>= 1;
    }
    return result;
}

size_t idx_for_max_pairs(uint64_t num)
{
    bool prev_flag = false;
    uint64_t curr_mask = 1;
    uint64_t next_mask = 2;

    size_t res = 0;
    for (size_t idx = 0; idx < 63 && num; ++idx) {
        if (!(num & curr_mask)) {
            if (num & next_mask) {
                if (prev_flag) {
                    if (num & next_mask) {
                    return idx;
                    }
                }
                else {
                    prev_flag = false;
                }
                res = idx;
            }
        }
        else {
            prev_flag = true;
        }
        num >>= 1;
    }
    return res;
}



BEGIN_TEST(test_count_pairs_single_pair)
    ASSERT_EQUAL(count_pairs(0b11), 1);
END_TEST

BEGIN_TEST(test_count_pairs_multiple_pairs)
    ASSERT_EQUAL(count_pairs(0b11011), 2);
END_TEST

BEGIN_TEST(test_count_pairs_no_pairs)
    ASSERT_EQUAL(count_pairs(0b101), 0);
END_TEST

BEGIN_TEST(test_count_pairs_all_zeros)
    ASSERT_EQUAL(count_pairs(0b0), 0);
END_TEST

BEGIN_TEST(test_idx_for_max_pairs_single_case)
    ASSERT_EQUAL(idx_for_max_pairs(0b110), 0); // Gap between bit 2 and bit 0
END_TEST

BEGIN_TEST(test_idx_for_max_pairs_multiple_gaps)
    ASSERT_EQUAL(idx_for_max_pairs(0b10101), 1); // Gap between bit 3 and bit 1
END_TEST

BEGIN_TEST(test_idx_for_max_pairs_no_gap)
    ASSERT_EQUAL(idx_for_max_pairs(0b1111), 0); // All bits set, no gaps
END_TEST

BEGIN_TEST(test_idx_for_max_pairs_no_bits_set)
    ASSERT_EQUAL(idx_for_max_pairs(0b0), 0); // No bits are set
END_TEST

TEST_SUITE(count_pairs and idx_for_max_pairs tests)
    TEST(test_count_pairs_single_pair)
    TEST(test_count_pairs_multiple_pairs)
    TEST(test_count_pairs_no_pairs)
    TEST(test_count_pairs_all_zeros)
    TEST(test_idx_for_max_pairs_single_case)
    TEST(test_idx_for_max_pairs_multiple_gaps)
    TEST(test_idx_for_max_pairs_no_gap)
    TEST(test_idx_for_max_pairs_no_bits_set)
END_SUITE
