#include "bits.h"
#include <stdio.h>
#define UC_SIZE 7

unsigned char SetBits_1(unsigned char x, unsigned int p, unsigned int n, unsigned char y)
{
    unsigned char mask;
    unsigned char yBits;

    /* Invalid input, x remains the same */
    if (n >= UC_SIZE || n <= 0 || p < n - 1 || p <= n || p >= UC_SIZE || p <= 0)
    {
        return x;
    }

    /* Create a mask to reset x's bits */
    mask = ~(((1 << n) - 1) << (p - n + 1));

    /* Resets x's bits at the right position */
    x &= mask;

    /* Get y's bits at the same position */
    yBits = (y & ((1 << n) - 1)) << (p - n + 1);

    /* OR operation with those y's bits (x's bits are setted to 0, hence will take whats in yBits)*/
    x |= yBits;

    return x;
}

unsigned char SetBits_2(unsigned int w, unsigned int i, unsigned int j, unsigned int value)
{
    unsigned int mask;

    /* Invalid input, x remains the same */
    if (value > 1 || value < 0 || i > j || i < 0 || j > 31)
    {
        return w;
    }

    /* Create a mask to set x's bits at the right positions */
    mask = (((1 << (j - i + 1)) - 1) << i);

    /* Depend on the value, set x's bits at i -> j positions */
    w = (value == 0) ? (w & (~mask)) : (w | mask);

    return w;
}

int IdenticalBitPairs(unsigned int num)
{
    int result = 0;
    int index;
    for (index = UC_SIZE; index >= 0; index--)
    {
        /* If found a pair, increment result AND decrement the index 1 more to try the next pair */
        if (((((num >> index) & 1) ^ ((num >> (index - 1)) & 1)) == 0))
        {
            --index;
            ++result;
        }
    }
    return result;
}
