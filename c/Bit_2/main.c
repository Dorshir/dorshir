#include "bitwise.h"
#include <stdio.h>

int main()
{
    unsigned int a = 1321657;

    printf("Before pairswap:\n");
    PrintBinaryUINT(a);

    unsigned int swapped = FlipPairs(a);
    printf("After pairswap:\n");
    PrintBinaryUINT(swapped);

    printf("Is number %d palindrom? %s\n", 0xFFFFFFFF, (IsPalindrom(0xFFFFFFFF) == TRUE) ? "YES" : "NO");
    PrintBinaryUINT(0xFFFFFFFF);

    printf("Reverse of: ");
    PrintBinaryUINT(a);
    PrintBinaryUINT(ReverseBits(a));

    return 0;
}