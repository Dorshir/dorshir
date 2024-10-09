#include "functions.c"
#include <stdio.h>

int main()
{
    int i;
    int arr[] = {1, 1, 1, 3, 5, 5, 9, 9, 9, 9};

    int arr1[] = {1, 3, 5, 7, 9};
    int arr2[] = {2, 2, 4, 6, 8, 8};

    int merged_arr[11];

    merge(arr1, 5, arr2, 6, merged_arr);
    printf("{");
    for (i = 0; i < 10; i++)
    {
        printf("%d,", merged_arr[i]);
    }
    printf("%d}\n", merged_arr[10]);

    return 0;
}