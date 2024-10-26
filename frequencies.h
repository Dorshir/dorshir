#ifndef __FREQUENCY_H__
#define __FREQUENCY_H__

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