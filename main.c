#include "PointerToFunc/pointer_to_func.c"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static void PrintArray(int *arr, size_t length)
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

int main(int argc, char *argv[])
{
    int arr[] = {1, 6, 5, 8, 34, 234, 547, 58, 2, 12, 325, 8, 9, 5, 23, 25, 63, 86, 8, 63, 23, 2, 47, 85, 7, 532};
    size_t length = sizeof(arr) / sizeof(arr[0]);
    PrintArray(arr, length);

    Sort(arr, length, CompareBySinus);
    PrintArray(arr, length);

    return 0;
}
