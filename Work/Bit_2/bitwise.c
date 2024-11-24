#include "bitwise.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BITS_IN_BYTE 8
#define UCH_IN_BITS sizeof(unsigned char) * BITS_IN_BYTE
#define RIGHT 1
#define LEFT 0
#define EIGHT_BIT_OPTIONS 256

typedef unsigned char (*Operation)(unsigned char _num);

static int ShapeN(int n);
static Bool IsValidCode(char c);
static Status CompressChars(char *str, int length, int index, char curr, unsigned char *compressedChar1, unsigned char *compressedChar2);
static void BuildCountBitLTU(unsigned char *_countBitLTU);
static unsigned char ReverseBitsOp(unsigned char _num);
static void BuildLTU(unsigned char *_LTU, Operation _op);
static unsigned char FlipPairsOp(unsigned char _num);

/************************************** Main functions ***************************************/

Status CompressString(char *_str)
{
    size_t length = strlen(_str);
    size_t compressedLength = (length + 1) / 2;

    int index, compressedIndex;
    unsigned char curr;
    unsigned char compressedChar1 = 0, compressedChar2 = 0;

    for (index = 0, compressedIndex = 0; index < length; index += 2, compressedIndex++)
    {
        curr = _str[index];
        if (CompressChars(_str, length, index, curr, &compressedChar1, &compressedChar2) != OK)
        {
            return INVALID_INPUT;
        }

        _str[compressedIndex] = (compressedChar1 << 4) | compressedChar2;
    }
    _str[compressedLength] = '\0';

    return OK;
}

Status RotateBits(unsigned char *x, int _n)
{
    unsigned char leftPart;
    unsigned char rightPart;
    if (x == NULL)
    {
        return NULL_PTR_ERROR;
    }
    if (_n < 0)
    {
        return INVALID_INPUT;
    }

    _n = ShapeN(_n);
    if (_n == 0 || _n == UCH_IN_BITS)
    {
        return OK;
    }

    printf("x = ");
    PrintBinaryUC(*x);

    leftPart = *x >> _n;
    rightPart = *x << (UCH_IN_BITS - _n);

    *x = leftPart | rightPart;
    printf("x rotated by %d = ", _n);
    PrintBinaryUC(*x);

    return OK;
}

Status InvertBits(unsigned char _x, unsigned char *_y)
{
    if (_y == NULL)
    {
        return NULL_PTR_ERROR;
    }

    *_y = ~_x;

    printf("x = ");
    PrintBinaryUC(_x);
    printf("x inverted = ");
    PrintBinaryUC(*_y);
    return OK;
}

int CountBits(unsigned int _c)
{
    int count = 0;
    while (_c != 0)
    {
        count += 1 & _c;
        _c = _c >> 1;
    }
    return count;
}

int CountBitLTUchar(unsigned char _num)
{
    static unsigned char countBitLTU[EIGHT_BIT_OPTIONS];
    static int flag;

    if (!flag)
    {
        BuildCountBitLTU(countBitLTU);
        flag = 1;
    }
    return countBitLTU[_num];
}

int CountBitLTUint(unsigned int _num)
{
    static unsigned char countBitLTU[EIGHT_BIT_OPTIONS];
    static int flag;

    if (!flag)
    {
        BuildCountBitLTU(countBitLTU);
        flag = 1;
    }

    size_t numBytes = sizeof(unsigned int);
    int count = 0;
    for (size_t byteNum = 0; byteNum < numBytes; byteNum++)
    {
        count += countBitLTU[(_num >> (byteNum * BITS_IN_BYTE)) & 0xFF];
    }

    return count;
}

unsigned int ReverseBits(unsigned int _num)
{
    static unsigned char revBitsLTU[EIGHT_BIT_OPTIONS];
    static int flag;

    if (!flag)
    {
        BuildLTU(revBitsLTU, ReverseBitsOp);
        flag = 1;
    }

    size_t numBytes = sizeof(unsigned int);
    unsigned int result = 0;
    for (size_t byteNum = 0; byteNum < numBytes; byteNum++)
    {
        result = (result << BITS_IN_BYTE) | revBitsLTU[(_num >> (byteNum * BITS_IN_BYTE)) & 0xFF];
    }

    return result;
}

Bool IsPalindrom(unsigned int _num)
{
    unsigned int reversed = ReverseBits(_num);
    return (_num == reversed) ? TRUE : FALSE;
}

void PrintBinaryUC(unsigned char _x)
{
    for (int index = UCH_IN_BITS - 1; index >= 0; index--)
    {
        printf("%d", (_x >> index) & 1);
    }
    printf("\n");
}

void PrintBinaryUINT(unsigned int _x)
{
    int numBits = sizeof(unsigned int) * 8;
    for (int byteNum = numBits - 1; byteNum >= 0; byteNum--)
    {
        printf("%d", (_x >> byteNum) & 1);
    }
    printf("\n");
}

unsigned int FlipPairs(unsigned int _num)
{
    static unsigned char pairBitsLTU[EIGHT_BIT_OPTIONS];
    static int flag;

    if (!flag)
    {
        BuildLTU(pairBitsLTU, FlipPairsOp);
        flag = 1;
    }

    size_t numBytes = sizeof(unsigned int);
    unsigned int result = 0;
    for (size_t byteNum = 0; byteNum < numBytes; byteNum++)
    {
        unsigned char swappedByte = pairBitsLTU[(_num >> (byteNum * BITS_IN_BYTE)) & 0xFF];
        result |= (swappedByte << (byteNum * BITS_IN_BYTE));
    }

    return result;
}

/************************************** Static functions ***************************************/

static Status CompressChars(char *_str, int _length, int _index, char _curr, unsigned char *_compressedChar1, unsigned char *_compressedChar2)
{
    if (IsValidCode(_curr))
    {
        *_compressedChar1 = _curr - 'a' + 1;
    }
    else
    {
        return INVALID_INPUT;
    }
    if (_index + 1 < _length)
    {
        _curr = _str[_index + 1];

        if (IsValidCode(_curr))
        {
            *_compressedChar2 = _curr - 'a' + 1;
        }
        else
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

static Bool IsValidCode(char _c)
{
    return _c >= 'a' && _c <= 'o';
}

static int ShapeN(int _n)
{
    return (_n > UCH_IN_BITS ? _n % UCH_IN_BITS : _n);
}

static void BuildLTU(unsigned char *_LTU, Operation _op)
{
    for (size_t index = 0; index < EIGHT_BIT_OPTIONS; index++)
    {
        _LTU[index] = _op(index);
    }
}

static void BuildCountBitLTU(unsigned char *_countBitLTU)
{
    for (size_t index = 0; index < EIGHT_BIT_OPTIONS; index++)
    {
        _countBitLTU[index] = CountBits(index);
    }
}

static unsigned char ReverseBitsOp(unsigned char _num)
{
    unsigned char res = 0;

    for (int i = 0; i < BITS_IN_BYTE; i++)
    {
        res <<= 1;
        res |= (_num >> i) & 1;
    }
    return res;
}

static unsigned char FlipPairsOp(unsigned char _num)
{
    unsigned char mask;
    for (int i = 0; i < BITS_IN_BYTE - 1; i += 2)
    {
        /* Extract i and i + 1 bits */
        unsigned char first = (_num >> i) & 1;
        unsigned char second = (_num >> (i + 1)) & 1;

        /* Clear i and i + 1 bits of _num */
        mask = ~(1 << i);
        _num &= mask;
        mask = ~(1 << (i + 1));
        _num &= mask;

        /* Set with swapped bits */
        _num |= (second << i);
        _num |= (first << (i + 1));
    }
    return _num;
}
