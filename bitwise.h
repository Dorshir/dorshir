#ifndef __BITWISE_H__
#define __BITWISE_H__



typedef enum Status
{
    NULL_PTR_ERROR = -1,
    INVALID_INPUT,
    OK
} Status;

typedef enum Bool
{
    FALSE,
    TRUE
} Bool;

/**
 * @brief Compresses a string by packing characters into 4-bit representations.
 *
 * Each character from 'a' to 'o' is represented using 4 bits.
 * Two characters are packed into one byte.
 * Compression is performed in place.
 *
 * @param str Pointer to the string to be compressed.
 * @return  Returns `OK` (1) on success.
 *          Returns `INVALID_INPUT` (0) if input string includes unsupported codes.
 *          Returns `NULL_PTR_ERROR` (1) if the input string is a null pointer.
 */
Status CompressString(char *str);

/**
 * @brief Inverts the bits of an unsigned char.
 *
 * @param x The unsigned char value to invert.
 * @param y Pointer to store the inverted result.
 * @return  Returns `OK` (1) on success.
 *          Returns `INVALID_INPUT` (0) if input string includes unsupported codes.
 *          Returns `NULL_PTR_ERROR` (1) if the input string is a null pointer.
 */
Status InvertBits(unsigned char x, unsigned char* y);

/**
 * @brief Rotates the bits of an unsigned char by n positions.
 *
 * Rotation is performed in place on the value pointed to by x.
 *
 * @param x Pointer to the unsigned char to be rotated.
 * @param n Number of positions to rotate.
 * @return  Returns `OK` (1) on success.
 *          Returns `INVALID_INPUT` (0) if input string includes unsupported codes.
 *          Returns `NULL_PTR_ERROR` (1) if the input string is a null pointer.
 */
Status RotateBits(unsigned char *x, int n);


void PrintBinary(unsigned char x);


#endif