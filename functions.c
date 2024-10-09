#include "functions.h"

int findMedian(const int arr[], int length)
{
    int curr_min = INT_MIN;
    int curr_max = INT_MAX;
    int k1, k2;
    int count_less;
    int count_equal;
    int index;
    int j;
    int median1 = 0, median2 = 0;
    int median1_assigned = 0, median2_assigned = 0;

    if (arr == NULL)
    {
        return NULL_PTR_ERROR;
    }

    if (length <= 0)
    {
        return INVALID_INPUT_ERROR;
    }

    /*
        Even length. We need to calculate the avarage of the 2 middles.
    */
    if (length % 2 == 0)
    {
        k1 = length / 2 - 1;
        k2 = length / 2;
    }
    /*
        Odd length. The median is positioned in the middle.
    */
    else
    {
        k1 = length / 2;
        k2 = k1;
    }

    for (index = 0; index < length; index++)
    {
        int current = arr[index];

        if (current < curr_min || current > curr_max)
        {
            continue;
        }

        count_less = 0;
        count_equal = 0;

        for (j = 0; j < length; j++)
        {
            if (arr[j] < current)
            {
                count_less++;
            }
            else if (arr[j] == current)
            {
                count_equal++;
            }
        }

        if (!median1_assigned && count_less <= k1 && (count_less + count_equal) > k1)
        {
            median1 = current;
            median1_assigned = 1;
        }

        if (!median2_assigned && count_less <= k2 && (count_less + count_equal) > k2)
        {
            median2 = current;
            median2_assigned = 1;
        }

        if (median1_assigned && median2_assigned)
        {
            break;
        }

        if (count_less > k2)
        {
            curr_max = current;
        }
        else if ((count_less + count_equal - 1) < k1)
        {
            curr_min = current;
        }
    }

    if (!median1_assigned || !median2_assigned)
    {
        return INVALID_INPUT_ERROR;
    }

    if (length % 2 == 0)
    {
        return (median1 + median2) / 2;
    }
    else
    {
        return median1;
    }
}

int merge(const int arr1[], int length1, const int arr2[], int length2, int result_arr[])
{
    int result_index = 0;
    int index1 = 0;
    int index2 = 0;

    if (arr1 == NULL || arr2 == NULL || result_arr == NULL)
    {
        return NULL_PTR_ERROR;
    }

    if (length1 <= 0 || length2 <= 0)
    {
        return INVALID_INPUT_ERROR;
    }

    while (index1 < length1 && index2 < length2)
    {
        if (arr1[index1] < arr2[index2])
        {
            result_arr[result_index] = arr1[index1++];
        }
        else
        {
            result_arr[result_index] = arr2[index2++];
        }
        result_index++;
    }

    /* Fill the remainings of arr1 */
    while (index1 < length1)
    {
        result_arr[result_index++] = arr1[index1++];
    }

    /* Fill the remainings of arr2 */
    while (index2 < length2)
    {
        result_arr[result_index++] = arr2[index2++];
    }

    return OK;
}

void CharSwap(char *char1, char *char2)
{
    char temp;
    temp = *char1;
    *char1 = *char2;
    *char2 = temp;
}

int ReverseStr(char *_str)
{
    size_t length = strlen(_str);
    int left = 0;
    int right = length - 1;

    if (_str == NULL)
    {
        return NULL_PTR_ERROR;
    }

    while (left < right)
    {
        if (_str[left] != _str[right])
        {
            CharSwap(&_str[left++], &_str[right--]);
        }
    }
    return OK;
}

int IsPalindrome(char *_str)
{
    size_t length = strlen(_str);
    int left = 0;
    int right = length - 1;

    if (_str == NULL)
    {
        return NULL_PTR_ERROR;
    }

    while (left < right)
    {
        if (_str[left++] != _str[right--])
        {
            return OK;
        }
    }

    return 1;
}

int MyAToI(char *_str, int *_num)
{
    size_t length;
    int index = 0;
    int j = 0;
    int m = 1;
    int isNegative = 0;
    int result = 0;

    if (_str == NULL || _num == NULL)
    {
        return NULL_PTR_ERROR;
    }

    while (_str[index] == ' ')
    {
        index++;
    }

    if (_str[index] == '+')
    {
        index++;
    }
    else if (_str[index] == '-')
    {
        isNegative = 1;
        index++;
    }

    if (!isdigit(_str[index]))
    {
        return INVALID_INPUT_ERROR;
    }

    while (isdigit(_str[index]))
    {
        result = result * 10 + (_str[index++] - '0');
    }

    if (isNegative)
    {
        result = -result;
    }
    *_num = result;

    return OK;
}

int MyIToA(int _num, char *_buffer)
{
    int digit;
    int index = 0;
    int isNegative = 0;

    if (_buffer == NULL)
    {
        return NULL_PTR_ERROR;
    }

    if (_num < 0)
    {
        isNegative = 1;
        _num = abs(_num);
    }

    if (_num == 0)
    {
        _buffer[0] = '0';
        _buffer[1] = '\0';
        return OK;
    }

    while (_num > 0)
    {
        digit = _num % 10;
        _buffer[index++] = (char)digit + '0';
        _num /= 10;
    }

    if (isNegative)
    {
        _buffer[index++] = '-';
    }

    _buffer[index] = '\0';

    ReverseStr(_buffer);

    return OK;
}
