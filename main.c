#include "PointerToFunc/pointer_to_func.c"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static void PrintArray(const int *arr, size_t length)
{
    printf("Array: [");
    for (size_t i = 0; i < length; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("]\n");
}

static int CompareAscending(int a, int b)
{
    return a > b;
}

static int CompareDescending(int a, int b)
{
    return a < b;
}

static int CompareBySinus(int a, int b)
{
    return sin((double)(a)) > sin((double)b);
}

static int CompareByLeastSignificantDigit(int a, int b)
{
    int lsd_a = abs(a) % 10; 
    int lsd_b = abs(b) % 10; 

    return lsd_a > lsd_b;
}

int main()
{
    const int i = 5;
    int* ptr = &i;

    return 0;
}
