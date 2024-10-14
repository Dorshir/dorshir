#include "abc.h"
#define SET_SIZE 256

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
    int i = *index1;

    while (i < length1 && index2 < length2 && str2[index2] == str1[i])
    {
        index2++;
        i++;
    }

    /* If entire str2 from at position *index1 = i, return TRUE and update index1, else index1 wont be incremented */
    if (index2 == length2)
    {
        result = TRUE;
        *index1 = i;
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

    for (index1 = 0; index1 < length1 - length2 + 1; index1++)
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

int StringDecToBinary(char *str)
{
    int index;
    size_t length;
    int number;
    int binaryArray[32];

    if (str == NULL)
    {
        return NULL_PTR_ERROR;
    }

    length = strlen(str);
    index = 0;

    if (length == 1 && str[0] == '0')
    {
        printf("0\n");
    }
    /* Empty or invalid input (e.g abc123) */
    else if (length == 0 || ((number = atoi(str)) == 0))
    {
        return INVALID_INPUT_ERROR;
    }
    /* Valid input, convert to binary representation and print */
    else
    {
        ConvertToBinary(number, binaryArray, &index);
        PrintBinary(binaryArray, &index);
    }
    return OK;
}

void MarkPresented(char charSet[], const char *str)
{
    int index;
    for (index = 0; str[index] != '\0'; index++)
    {
        charSet[(unsigned char)str[index]] = TRUE;
    }
}

int Squeeze(char *s1, const char *s2)
{
    char charsPresented[SET_SIZE] = {0};
    int left;
    int right;

    if (s1 == NULL || s2 == NULL)
    {
        return NULL_PTR_ERROR;
    }

    MarkPresented(charsPresented, s2);

    for (left = 0, right = 0; s1[right] != '\0'; right++)
    {
        if (!charsPresented[(unsigned char)s1[right]])
        {
            s1[left++] = s1[right];
        }
    }
    s1[left] = '\0';

    return OK;
}