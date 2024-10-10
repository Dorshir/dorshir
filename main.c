#include "abc.c"
#include <stdio.h>

int main()
{

    char *str1 = "dc";
    char *str2 = "cd";

    void *result = MyStrStr(str1, str2);
    if (result)
    {
        printf("Result from Str1: %s\n", (char*)MyStrStr(str1, str2));
    }
    else{
        
    }

    return 0;
}