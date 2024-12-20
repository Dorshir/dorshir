#ifndef __wctail_h__
#define __wctail_h__

typedef enum Status
{
    FILE_OPEN_FAILED = -3,
    INVALID_NUMBER_OF_LINES,
    EMPTY_FILE,
    ALLOCATE_MEMORY_FAILED,
    OK
} Status;

/**
 * @brief   Prints the number of lines, words, letters in a given file.
 *
 * @param   fileName    String that holds the file's name to consider.
 *
 * @return  Returns `OK` (1) on success.
 *          Returns `FILE_OPEN_FAILED` (2) if file open failed.
 */
Status Wc(const char *fileName);

/**
 * @brief   Prints the last -n- lines from the end of the given file.
 *
 * @param   fileName                String that holds the file's name to consider.
 * @param   desiredNumberOfLines    Integer for the desired number of lines.
 *
 * @return  Returns `OK` (1) on success.
 *          Returns `FILE_OPEN_FAILED` (2) if file open failed.
 *          Returns `INVALID_NUMBER_OF_LINES` (3) if the desired number of lines is lower than zero.
 *          Returns `EMPTY_FILE` (4) if the file is empty.
 *          Returns `ALLOCATE_MEMORY_FAILED` (5) if allocating memory for the positions array failed.
 */
Status Tail(const char *fileName, int desiredNumberOfLines);

#endif