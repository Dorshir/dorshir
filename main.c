#include "functions.c"
#include <stdio.h>
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main()
{
    int i;

    /* Test findMedian() */
    int arr1[] = {1, 3, 5, 7, 9};
    int arr2[] = {1, 2, 3, 4, 5, 6};
    int arr3[] = {7, 8, 3, 1, 5, 6, 9};

    /* Test merge() */
    int arr4[] = {1, 3, 5};
    int arr5[] = {2, 4, 6, 6};
    int arr6[] = {-5, 0, 3};
    int mergedArr1[ARRAY_SIZE(arr4) + ARRAY_SIZE(arr5)];
    int mergedArr2[ARRAY_SIZE(arr4) + ARRAY_SIZE(arr6)];

    /* Test ReverseStr() */
    char str1[] = "hello world";
    char str2[] = "a";
    char str3[] = "";

    /* Test IsPalindrome() */
    char str4[] = "abcdcba";
    char str5[] = "hello";
    char str6[] = "a";
    char str7[] = "";

    /* Test MyAToI() */
    char numStr1[] = "12345";
    char numStr2[] = "-6789";
    char numStr3[] = "  987";
    char numStr4[] = "abc123";
    char numStr5[] = " -8 44";
    char numStr6[] = " +44";
    char numStr7[] = "  67abc";
    int num;

    /* Test MyIToA() */
    int num1 = 12345;
    int num2 = -6789;
    int num3 = 0;
    char buffer[20];

    /* Test findMedian() */
    printf("Running tests for findMedian()\n");
    printf("Median of arr1: %d\n", findMedian(arr1, ARRAY_SIZE(arr1)));
    printf("Median of arr2: %d\n", findMedian(arr2, ARRAY_SIZE(arr2)));
    printf("Median of arr3: %d\n", findMedian(arr3, ARRAY_SIZE(arr3)));

    /* Test merge() */
    printf("Running tests for merge()\n");
    if (merge(arr4, ARRAY_SIZE(arr4), arr5, ARRAY_SIZE(arr5), mergedArr1) == OK)
    {
        printf("Merged array 1: ");
        for (i = 0; i < ARRAY_SIZE(mergedArr1); i++)
        {
            printf("%d ", mergedArr1[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Error while merging arrays 1\n");
    }

    if (merge(arr4, ARRAY_SIZE(arr4), arr6, ARRAY_SIZE(arr6), mergedArr2) == OK)
    {
        printf("Merged array 2: ");
        for (i = 0; i < ARRAY_SIZE(mergedArr2); i++)
        {
            printf("%d ", mergedArr2[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Error while merging arrays 2\n");
    }

    /* Test ReverseStr() */
    printf("Running tests for ReverseStr()\n");
    if (ReverseStr(str1) == OK)
    {
        printf("Reversed string 1: %s\n", str1);
    }
    else
    {
        printf("Error while reversing string 1\n");
    }

    if (ReverseStr(str2) == OK)
    {
        printf("Reversed string 2: %s\n", str2);
    }
    else
    {
        printf("Error while reversing string 2\n");
    }

    if (ReverseStr(str3) == OK)
    {
        printf("Reversed string 3: %s\n", str3);
    }
    else
    {
        printf("Error while reversing string 3\n");
    }

    /* Test IsPalindrome() */
    printf("Running tests for IsPalindrome()\n");
    printf("Is '%s' a palindrome? %d\n", str4, IsPalindrome(str4));
    printf("Is '%s' a palindrome? %d\n", str5, IsPalindrome(str5));
    printf("Is '%s' a palindrome? %d\n", str6, IsPalindrome(str6));
    printf("Is '%s' a palindrome? %d\n", str7, IsPalindrome(str7));

    /* Test MyAToI() */
    printf("Running tests for MyAToI()\n");
    if (MyAToI(numStr1, &num) == OK)
    {
        printf("String '%s' converted to integer: %d\n", numStr1, num);
    }
    else
    {
        printf("Error while converting '%s' to integer\n", numStr1);
    }
    if (MyAToI(numStr2, &num) == OK)
    {
        printf("String '%s' converted to integer: %d\n", numStr2, num);
    }
    else
    {
        printf("Error while converting '%s' to integer\n", numStr2);
    }
    if (MyAToI(numStr3, &num) == OK)
    {
        printf("String '%s' converted to integer: %d\n", numStr3, num);
    }
    else
    {
        printf("Error while converting '%s' to integer\n", numStr3);
    }
    if (MyAToI(numStr4, &num) == OK)
    {
        printf("String '%s' converted to integer: %d\n", numStr4, num);
    }
    else
    {
        printf("Error while converting '%s' to integer\n", numStr4);
    }
    if (MyAToI(numStr5, &num) == OK)
    {
        printf("String '%s' converted to integer: %d\n", numStr5, num);
    }
    else
    {
        printf("Error while converting '%s' to integer\n", numStr5);
    }
    if (MyAToI(numStr6, &num) == OK)
    {
        printf("String '%s' converted to integer: %d\n", numStr6, num);
    }
    else
    {
        printf("Error while converting '%s' to integer\n", numStr6);
    }
    if (MyAToI(numStr7, &num) == OK)
    {
        printf("String '%s' converted to integer: %d\n", numStr7, num);
    }
    else
    {
        printf("Error while converting '%s' to integer\n", numStr7);
    }

    /* Test MyIToA() */
    printf("Running tests for MyIToA()\n");
    if (MyIToA(num1, buffer) == OK)
    {
        printf("Integer %d converted to string: %s\n", num1, buffer);
    }
    else
    {
        printf("Error while converting %d to string\n", num1);
    }
    if (MyIToA(num2, buffer) == OK)
    {
        printf("Integer %d converted to string: %s\n", num2, buffer);
    }
    else
    {
        printf("Error while converting %d to string\n", num2);
    }
    if (MyIToA(num3, buffer) == OK)
    {
        printf("Integer %d converted to string: %s\n", num3, buffer);
    }
    else
    {
        printf("Error while converting %d to string\n", num3);
    }

    return 0;
}