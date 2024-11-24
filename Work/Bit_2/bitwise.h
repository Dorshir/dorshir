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
 * @param _str Pointer to the string to be compressed.
 * @return  Returns `OK` (1) on success.
 *          Returns `INVALID_INPUT` (0) if input string includes unsupported codes.
 *          Returns `NULL_PTR_ERROR` (1) if the input string is a null pointer.
 */
Status CompressString(char *_str);

/**
 * @brief Inverts the bits of an unsigned char.
 *
 * @param _x The unsigned char value to invert.
 * @param _y Pointer to store the inverted result.
 * @return  Returns `OK` (1) on success.
 *          Returns `INVALID_INPUT` (0) if input string includes unsupported codes.
 *          Returns `NULL_PTR_ERROR` (1) if the input string is a null pointer.
 */
Status InvertBits(unsigned char _x, unsigned char* _y);

/**
 * @brief Rotates the bits of an unsigned char by n positions.
 *
 * @param _x Pointer to the unsigned char to be rotated.
 * @param _n Number of positions to rotate.
 * @return  Returns `OK` (1) on success.
 *          Returns `INVALID_INPUT` (0) if input string includes unsupported codes.
 *          Returns `NULL_PTR_ERROR` (1) if the input string is a null pointer.
 */
Status RotateBits(unsigned char *x, int n);

/**
 * @brief Prints the binary representation of an unsigned int.
 *
 * @param _x The unsigned int value to be printed.
 */
void PrintBinaryUINT(unsigned int _x);

/**
 * @brief Prints the binary representation of an unsigned char.
 * 
 * @param _x The unsigned char value to be printed.
 */
void PrintBinaryUC(unsigned char _x);

/**
 * @brief Counts the number of set bits in an unsigned char.
 *
 * @param _num The unsigned char value to analyze.
 * @return The number of set bits (1s) in the input value.
 */
int CountBitLTUchar(unsigned char _num);

/**
 * @brief Counts the number of set bits in an unsigned int.
 *
 * @param _num The unsigned int value to analyze.
 * @return The number of set bits (1s) in the input value.
 */
int CountBitLTUint(unsigned int _num);

/**
 * @brief Counts the number of set bits in an unsigned int (naive method).
 *
 * @param _c The unsigned int value to analyze.
 * @return The number of set bits (1s) in the input value.
 */
int CountBits(unsigned int _c);

/**
 * @brief Reverses the bits of an unsigned int.
 *
 * @param _num The unsigned int value to reverse.
 * @return The unsigned int with its bits reversed.
 */
unsigned int ReverseBits(unsigned int _num);

/**
 * @brief Checks if an unsigned int is a palindrome in binary representation.
 *
 * @param _num The unsigned int value to check.
 * @return `TRUE` if the number is a binary palindrome, otherwise `FALSE`.
 */
Bool IsPalindrom(unsigned int _num);

/**
 * @brief Swaps adjacent bit pairs in an unsigned int.
 *
 * @param _num The unsigned int value to process.
 * @return The unsigned int with adjacent bit pairs swapped.
 */
unsigned int FlipPairs(unsigned int _num);

#endif