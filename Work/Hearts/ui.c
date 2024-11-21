#include "ui.h"

#include <stdio.h> /* printf */

void PrintMessage(char *_message)
{
    if (_message == NULL)
    {
        return;
    }
    printf("%s", _message);
}

void GetInput(char **_message)
{
    if (_message == NULL)
    {
        return;
    }
    
}
