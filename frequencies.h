#ifndef __FREQUENCY_H__
#define __FREQUENCY_H__

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define NUM_OF_LETTERS 52
#define INITIAL_BUFFER_SIZE 1000
#define UPPER_CASES_START_POSITION 26
#define LINE_SIZE 100
#define WORD_SIZE 25

typedef enum Status
{
    OPEN_FILE_FAILED,
    NULL_PTR_ERR,
    INVALID_NUMBER_OF_BYTES,
    ALLOCATE_MEMORY_FAILED,
    WORD_SIZE_OVERFLOW,
    OK
} Status;

/**
 * @brief Represents a node in the linked list of words.
 */
typedef struct Node
{
    struct Node* next;
    char word[WORD_SIZE];
    int freq;
} Node, *pNode;


/**
 * @brief Reads a file and calculates the frequency of each letter. Then prints a letter and its frequnce.
 *
 * @param fileName String containing the name of the file to process.
 *
 * @return Returns one of the following `Status` codes:
 * - `OK` if the operation completed successfully.
 * - `OPEN_FILE_FAILED` if the file could not be opened.
 * - `INVALID_NUMBER_OF_BYTES` if an invalid number of bytes was read from the file.
 */
Status LetterFrequencies(const char *fileName);

/**
 * @brief Reads a file and calculates the frequency of each word. Then prints a word and its frequnce.
 *
 * @param fileName String containing the name of the file to process.
 *
 * @return Returns one of the following `Status` codes:
 * - `OK` if the operation completed successfully.
 * - `OPEN_FILE_FAILED` if the file could not be opened.
 * - `NULL_PTR_ERR` if a null pointer was encountered.
 * - `WORD_SIZE_OVERFLOW` if a word exceeds the maximum allowed size (`WORD_SIZE`).
 * - `ALLOCATE_MEMORY_FAILED` if memory allocation failed.
 */
Status WordsFrequencies(const char *fileName);

#endif /*__FREQUENCY_H__*/