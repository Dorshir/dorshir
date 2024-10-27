#include "bitwise.h"
#include <stdlib.h>
#include <stdio.h>

#define BITS_IN_BYTE 8
#define UCH_IN_BITS sizeof(unsigned char) * BITS_IN_BYTE
#define RIGHT 1
#define LEFT 0

void PrintBinary(unsigned char x);
int ShapeN(int n);
Bool IsValidCode(char c);
Status CompressChars(char *str, int length, int index, char curr, unsigned char *compressedChar1, unsigned char *compressedChar2);

/************************************** Main functions ***************************************/

Status CompressString(char *str)
{
    size_t length = strlen(str);
    size_t compressedLength = (length + 1) / 2;

    int index, compressedIndex;
    unsigned char curr;
    unsigned char compressedChar1 = 0, compressedChar2 = 0;

    for (index = 0, compressedIndex = 0; index < length; index += 2, compressedIndex++)
    {
        curr = str[index];
        if (CompressChars(str, length, index, curr, &compressedChar1, &compressedChar2) != OK)
        {
            return INVALID_INPUT;
        }

        str[compressedIndex] = (compressedChar1 << 4) | compressedChar2;
    }
    str[compressedLength] = '\0';

    return OK;
}

Status RotateBits(unsigned char *x, int n)
{
    unsigned char leftPart;
    unsigned char rightPart;
    if (x == NULL)
    {
        return NULL_PTR_ERROR;
    }
    if (n < 0)
    {
        return INVALID_INPUT;
    }

    n = ShapeN(n);
    if (n == 0 || n == UCH_IN_BITS)
    {
        return OK;
    }

    printf("x = ");
    PrintBinary(*x);

    leftPart = *x >> n;
    rightPart = *x << (UCH_IN_BITS - n);

    x = leftPart | rightPart;
    printf("x rotated by %d = ", n);
    PrintBinary(*x);

    return OK;
}

Status InvertBits(unsigned char x, unsigned char *y)
{
    if (y == NULL)
    {
        return NULL_PTR_ERROR;
    }

    *y = ~x;

    printf("x = ");
    PrintBinary(x);
    printf("x inverted = ");
    PrintBinary(*y);
    return OK;
}

/************************************** Sub functions ***************************************/

void PrintBinary(unsigned char x)
{
    int index;
    for (index = UCH_IN_BITS - 1; index >= 0; index--)
    {
        printf("%d", (x >> index) & 1);
    }
    printf("\n");
}

Bool IsValidCode(char c)
{
    return c >= 'a' && c <= 'o';
}

int ShapeN(int n)
{
    return (n > UCH_IN_BITS ? n % UCH_IN_BITS : n);
}

Status CompressChars(char *str, int length, int index, char curr, unsigned char *compressedChar1, unsigned char *compressedChar2)
{
    if (IsValidCode(curr))
    {
        *compressedChar1 = curr - 'a' + 1;
    }
    else
    {
        return INVALID_INPUT;
    }
    if (index + 1 < length)
    {
        curr = str[index + 1];

        if (IsValidCode(curr))
        {
            *compressedChar2 = curr - 'a' + 1;
        }
        else
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}
