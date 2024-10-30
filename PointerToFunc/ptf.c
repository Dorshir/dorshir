#include "pointer_to_func.h"
#include "/workspaces/dorshir/Structs/structs.h"
#include <math.h>
#include <stdio.h>

#define NUM_OF_LETTERS(digit) (((digit) == 7 || (digit) == 9) ? 4 : 3)
#define MAX_NUM_OF_DIGITS 10

/* Help Functions */

static int CountDigits(int num)
{
    int count = 0;
    if (num == 0)
    {
        return 1;
    }
    if (num < 0)
    {
        num = -num;
    }
    while (num != 0)
    {
        ++count;
        num /= 10;
    }
    return count;
}

static char ParseLetter(int digit, int depth)
{
    int mod;
    static const char *codes[] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

    if (digit < 2 || digit > 9)
    {
        return '\0';
    }
    mod = NUM_OF_LETTERS(digit);

    return codes[digit][(depth - 1) % mod];
}

static int GetDigit(int num, int position, int numOfDigits)
{
    int divisor = (int)pow(10, numOfDigits - position);
    return (num / divisor) % 10;
}

static char *ParseWord(int num, char *result)
{
    int numOfDigits = CountDigits(num);
    int index = 0;
    int depth = 1;
    int prevDigit = -1;


    for (int pos = 1; pos <= numOfDigits; pos++)
    {
        int currentDigit = GetDigit(num, pos, numOfDigits);

        if (currentDigit == prevDigit && currentDigit > 0)
        {
            ++depth;
        }
        else
        {
            depth = 1;
            prevDigit = currentDigit;
        }

        char letter = ParseLetter(currentDigit, depth);
        if (letter != '\0')
        {
            result[index++] = letter;
        }
        
    }

    return result;
}

/* Generic */

static int CompareAscendingDoubleGeneric(void *a, void *b)
{
    return *((double *)a) > *((double *)b);
}

static int CompareAscendingIntGeneric(void *a, void *b)
{
    return *((int *)a) > *((int *)b);
}

static int ComparePersonsByNameGeneric(void *a, void *b)
{
    return (strcmp(((Person *)a)->name, ((Person *)b)->name) > 0);
}

static int CompareNumberToLetterGeneric(void *a, void *b)
{
    char resultA[MAX_NUM_OF_DIGITS + 1] = {0};
    char resultB[MAX_NUM_OF_DIGITS + 1] = {0};

    return (strcmp(ParseWord(*((int *)a), resultA), ParseWord(*((int *)b), resultB)) > 0);
}

/* Prints */

static void PrintIntArray(int *arr, size_t length)
{
    printf("Array: [");
    for (size_t i = 0; i < length; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("]\n");
}

static void PrintArrayDouble(double *arr, size_t length)
{
    printf("Array: [");
    for (size_t i = 0; i < length; i++)
    {
        printf("%f, ", arr[i]);
    }
    printf("]\n");
}

void PrintPersonsArray(Person array[], size_t length)
{
    int index;
    printf("==== Printing persons ====\n");
    for (index = 0; index < length; index++)
    {
        printf("Name: %s\nId: %d\nAge: %d\nEducation: %d\n\n", array[index].name, array[index].id, array[index].age, array[index].education);
    }
}

/* Regular */

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
    int intArr[] = {1, 6, 5, 8, 34, 234, 547, 58, 2, 12, 325, 8, 9, 5, 23, 25, 63, 86, 8, 63, 23, 2, 47, 85, 7, 532};
    size_t length = sizeof(intArr) / sizeof(intArr[0]);
    PrintIntArray(intArr, length);

    GenericSort(intArr, length, sizeof(intArr[0]), CompareAscendingIntGeneric);
    PrintIntArray(intArr, length);

    double doubleArr[] = {1.1, 0.2, 33.9, 999.1, 999.0, 444.5};
    length = sizeof(doubleArr) / sizeof(doubleArr[0]);
    PrintArrayDouble(doubleArr, length);

    GenericSort(doubleArr, length, sizeof(doubleArr[0]), CompareAscendingDoubleGeneric);
    PrintArrayDouble(doubleArr, length);

    Person personArr[] = {{"Bob", 32165431, 25, 12}, {"Lola", 32165431, 50, 10}, {"Alice", 32165431, 66, 12}};
    length = sizeof(personArr) / sizeof(personArr[0]);
    PrintPersonsArray(personArr, length);

    GenericSort(personArr, length, sizeof(personArr[0]), ComparePersonsByNameGeneric);
    PrintPersonsArray(personArr, length);

    int newIntArr[] = {777786667, 23444};
    length = sizeof(newIntArr) / sizeof(newIntArr[0]);
    PrintIntArray(newIntArr, length);

    GenericSort(newIntArr, length, sizeof(newIntArr[0]), CompareNumberToLetterGeneric);
    PrintIntArray(newIntArr, length);

    return 0;
}
