#ifndef __UI_H__
#define __UI_H__

#include <stddef.h> /* size_t */

/**
 * @brief Print a message to the user.
 *
 * @param[in] _message - The message string to print.
 */
void PrintMessage(char *_message);

/**
 * @brief Get input from the user.
 *
 * @param[out] _input - Buffer to store the user's input.
 * @param[in] _length - Maximum length of the input buffer.
 */
void GetInput(char *_input, size_t _length);

#endif /* __UI_H__ */
