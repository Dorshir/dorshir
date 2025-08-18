#include "ui.h"
#include <stdio.h> /* printf, fgets, stdin */
#define TRUE 1

void PrintMessage(char *_message)
{
    if (_message == NULL)
    {
        return;
    }
    printf("%s", _message);
}

void GetInput(char *_input, size_t _length)
{
    if (_input == NULL || _length == 0)
    {
        return;
    }
    while (TRUE)
    {
        if (fgets(_input, _length, stdin) != NULL)
        {
            break;
        }
        PrintMessage("Error reading input. Please try again.\n");
    }
}
