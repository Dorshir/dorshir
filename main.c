
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define ASSERT(X) (X) ? 0 : (fprintf(stderr, "Assertion in line %d, in exp: (" #X ")\n", __LINE__))

int IsLST()
{
    int a = 5;

    char *b = &a;
    if (*b == 5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int func(int _num)
{
    if (_num == 0)
    {
        return 0;
    }
    if (_num == 1)
    {
        return 1;
    }
    if (_num == 2)
    {
        return 2;
    }

    return func(_num - 1) + func(_num - 2);
}

int main()
{

    printf("Is Little-Endian? %s\n", IsLST() == 1 ? "TRUE" : "FALSE");

    int num = 4;
    printf("Number of option for a ledder of size %d: %d\n", num, func(num));

        return 0;
}



