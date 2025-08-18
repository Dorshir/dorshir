#ifndef __BITS_H__
#define __BITS_H__

/**
 * @brief Sets n bits of x starting at position p with bits from y.
 *
 *
 * @param x The original unsigned char value.
 * @param p The position in x where bits will be set.
 * @param n The number of bits to set.
 * @param y The unsigned char value from which bits are taken.
 *
 * @return The modified unsigned char value with bits set. If invalid input parameters are provided,
 *         the function returns x unchanged.
 */
unsigned char SetBits_1(unsigned char x, unsigned int p, unsigned int n, unsigned char y);


/**
 * @brief Sets bits from position i to j in w to the given value (0 or 1).
 *
 * @param w The original unsigned int value.
 * @param i The starting bit position.
 * @param j The ending bit position (must be greater than or equal to i).
 * @param value The value to set the bits to (0 or 1).
 *
 * @return The modified unsigned int value with bits set. If invalid input parameters are provided,
 *         the function returns w unchanged.
 */
unsigned char SetBits_2(unsigned int w, unsigned int i, unsigned int j, unsigned int value);


/**
 * @brief Counts the number of identical adjacent non-overlapping bit pairs in a num.
 *
 * @param num The unsigned int value.
 *
 * @return The number of identical adjacent bit pairs.
 */
int IdenticalBitPairs(unsigned int num);

#endif