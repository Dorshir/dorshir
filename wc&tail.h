#ifndef __wctail_h__
#define __wctail_h__

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

#define INITIAL_AVG_LINE_SIZE 80
#define MAX_LINE_LENGTH 1024
#define MAX_LINES_ALLOWED 1000
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef enum
{
    OK,
    FILE_OPEN_FAILED,
    INVALID_NUMBER_OF_LINES,
    EMPTY_FILE,
    ALLOCATE_MEMORY_FAILED
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

#endif;