#include "abc.h"

int CheckInputStrStr(char *str1, char *str2)
{
    int result = TRUE;
    if (str1 == NULL || str2 == NULL || strlen(str2) > strlen(str1))
    {
        result = FALSE;
    }
    return result;
}

int IsEntireStr2InSubStr1(char *str1, char *str2, int *index1, size_t length1, size_t length2)
{
    int result = FALSE;
    int index2 = 0;

    while (*index1 < length1 && index2 < length2 && str2[index2] == str1[*index1])
    {
        index2++;
        *index1 = *index1 + 1;
    }

    if (index2 == length2)
    {
        result = TRUE;
    }

    return result;
}

void *MyStrStr(char *str1, char *str2)
{
    int index1;
    int index2;

    size_t length1;
    size_t length2;

    void *result = NULL;

    if (!CheckInputStrStr(str1, str2))
    {
        return result;
    }

    length1 = strlen(str1);
    length2 = strlen(str2);

    for (index1 = 0; index1 < length1; index1++)
    {
        index2 = 0;
        if (str1[index1] == str2[index2])
        {
            if (IsEntireStr2InSubStr1(str1, str2, &index1, length1, length2))
            {
                return &str1[index1 - length2];
            }
        }
    }
    return result;
}

void ConvertToBinary(int number, int *array, int *index)
{
    while (number > 0)
    {
        array[*index] = number % 2;
        number /= 2;
        ++(*index);
    }
    --(*index);
}

void PrintBinary(int *array, int *index)
{
    while (*index >= 0)
    {
        printf("%d", array[*index]);
        --*(index);
    }
    printf("\n");
}

void StringDecToBinary(char *str)
{
    int index;
    size_t length;
    int number;
    int binaryArray[32];

    if (str == NULL)
    {
        return;
    }

    length = strlen(str);
    index = 0;

    number = atoi(str);
    ConvertToBinary(number, binaryArray, &index);
    PrintBinary(binaryArray, &index);
}


