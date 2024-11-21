
#include <stdio.h>  /* printf */
#include <stdlib.h> /* size_t, rand, malloc, free */
#include <string.h> /* memcpy */
#include <time.h>   /* clock */
#include <math.h>   /* fmin, fmax */
#include <limits.h> /* INT_MAX, INT_MIN */

#define FIRST 1
#define SECOND 2
#define THIRD 3

#define FIRST_ARRAY_SIZE 10000
#define SECOND_ARRAY_SIZE 40000
#define THIRD_ARRAY_SIZE 100000

#define FIRST_MODULO_RANGE 12000
#define SECOND_MODULO_RANGE 50000
#define THIRD_MODULO_RANGE 120000

#define TIME_IN_SEC(start, end) (((double)(end - start)) / CLOCKS_PER_SEC)
#define SWAP(arr, left, right)  \
    {                           \
        int temp = arr[left];   \
        arr[left] = arr[right]; \
        arr[right] = temp;      \
    }

typedef enum Status
{
    SUCCESS,
    FAILURE
} Status;

typedef Status (*sortFunc)(int *_arr, size_t _length);

static void IsSorted(int *_arr, size_t _length);
static void GetProperties(size_t *_length, size_t *_modRange, int _case);
static void InitWithRandoms(int *_arr, int _case);
static void PrintArray(int *_arr, size_t length);
static int ShakeRight(int *_arr, size_t _begin, size_t _end);
static int ShakeLeft(int *_arr, size_t _begin, size_t _end);
static size_t Partition(int *_arr, size_t _begin, size_t _end);
static size_t GetIndexOfMin(int *_arr, size_t _begin, size_t _end);
static Status QuickSortRec(int *_arr, size_t _begin, size_t _end);
static Status InsertionSortWithGap(int *_arr, size_t _length, size_t _begin, size_t _gap);
static void RunSort(int *_tempArray, int *_baseArray, size_t _length, sortFunc _sortFunc, char *_sortName);
static void CountFreq(int *_baseArr, size_t _length, int *_freqArr, int _bias);
static void ComputeCumulativeFreq(int *_freqArr, int _range);
static void FillSortedArray(int *_baseArr, size_t _length, int *_freqArr, int *_sortedArr, int _bias);
static void DestroyLocalArrays(int **_freqArr, int **_sortedArr);
static void GetMinMaxValues(int *_arr, size_t _length, int *_min, int *_max);
static void MergeIntegerArrays(int *_firstArr, size_t _firstLen, int *_secondArr, size_t _secondLen, int *_mergedArr);
static Status AllocateArrays(int **_freqArr, size_t _range, int **_sortedArr, size_t _length);
static void GetCurrProp(size_t *_mid, size_t *_end, size_t *_begin, size_t _currLen, size_t _length, size_t *_leftLen, size_t *_rightLen);
static void MergeRec(int *_arr, size_t _begin, size_t _end, int *_tempArr);
static int GetMax(int *_arr, size_t _length);
static void CountingSortForRadix(int *_arr, size_t _length, int _exp);

Status BubbleSort(int *_arr, size_t _length)
{
    size_t left, right;
    int breakFlag;
    if (_arr == NULL)
    {
        return FAILURE;
    }
    else if (_length <= 1)
    {
        return SUCCESS;
    }

    for (left = 0; left < _length; left++)
    {
        breakFlag = 1;
        for (right = 0; right < _length - left - 1; right++)
        {
            if (_arr[right] > _arr[right + 1])
            {
                SWAP(_arr, right, right + 1);
                breakFlag = 0;
            }
        }
        if (breakFlag)
        {
            break;
        }
    }
    return SUCCESS;
}

Status ShakeSort(int *_arr, size_t _length)
{
    size_t left, right;

    if (_arr == NULL)
    {
        return FAILURE;
    }
    else if (_length <= 1)
    {
        return SUCCESS;
    }

    left = 0;
    right = _length;

    while (left < right)
    {
        if (ShakeRight(_arr, left, right--))
        {
            break;
        }
        if (ShakeLeft(_arr, left++, right))
        {
            break;
        }
    }
    return SUCCESS;
}

Status QuickSort(int *_arr, size_t _length)
{
    if (_arr == NULL)
    {
        return FAILURE;
    }
    return QuickSortRec(_arr, 0, _length);
}

Status InsertionSort(int *_arr, size_t _length)
{
    if (_arr == NULL)
    {
        return FAILURE;
    }
    else if (_length <= 1)
    {
        return SUCCESS;
    }
    return InsertionSortWithGap(_arr, _length, 0, 1);
}

Status ShellSort(int *_arr, size_t _length)
{
    for (size_t gap = _length / 2; gap > 0; gap /= 2)
    {
        for (size_t index = 0; index < gap; index++)
        {
            InsertionSortWithGap(_arr, _length, index, gap);
        }
    }
    return SUCCESS;
}

Status SelectionSort(int *_arr, size_t _length)
{
    size_t index, nextMinIndex;
    if (_arr == NULL)
    {
        return FAILURE;
    }
    else if (_length <= 1)
    {
        return SUCCESS;
    }

    for (index = 0; index < _length; index++)
    {
        nextMinIndex = GetIndexOfMin(_arr, index, _length);
        if (nextMinIndex == index)
        {
            continue;
        }
        SWAP(_arr, index, nextMinIndex);
    }

    return SUCCESS;
}

Status MergeSortIt(int *_arr, size_t _length)
{
    int *tempArr;
    size_t mid, currLen, begin, end, leftLen, rightLen;
    if (_arr == NULL)
    {
        return FAILURE;
    }
    else if (_length <= 1)
    {
        return SUCCESS;
    }
    tempArr = malloc(sizeof(int) * _length);
    if (tempArr == NULL)
    {
        return FAILURE;
    }

    for (currLen = 1; currLen <= _length - 1; currLen *= 2)
    {
        for (begin = 0; begin < _length - 1; begin += 2 * currLen)
        {
            GetCurrProp(&mid, &end, &begin, currLen, _length, &leftLen, &rightLen);
            MergeIntegerArrays(&_arr[begin], leftLen, &_arr[mid + 1], rightLen, &tempArr[begin]);
            memcpy(&_arr[begin], &tempArr[begin], (leftLen + rightLen) * sizeof(int));
        }
    }

    return SUCCESS;
}

Status MergeSortRec(int *_arr, size_t _length)
{
    int *tempArr;
    if (_arr == NULL)
    {
        return FAILURE;
    }
    else if (_length <= 1)
    {
        return SUCCESS;
    }
    tempArr = malloc(sizeof(int) * _length);
    if (tempArr == NULL)
    {
        return FAILURE;
    }

    MergeRec(_arr, 0, _length, tempArr);
    memcpy(_arr, tempArr, _length * sizeof(int));

    free(tempArr);
    return SUCCESS;
}

Status CountingSort(int *_arr, size_t _length)
{
    size_t range;
    int _min, _max;
    int *freqArr, *sortedArr;
    if (_arr == NULL)
    {
        return FAILURE;
    }
    if (_length <= 1)
    {
        return SUCCESS;
    }
    GetMinMaxValues(_arr, _length, &_min, &_max);
    range = _max - _min + 1;

    if (AllocateArrays(&freqArr, range, &sortedArr, _length) != SUCCESS)
    {
        return FAILURE;
    }

    CountFreq(_arr, _length, freqArr, _min);
    ComputeCumulativeFreq(freqArr, range);
    FillSortedArray(_arr, _length, freqArr, sortedArr, _min);
    memcpy(_arr, sortedArr, _length * sizeof(int));

    DestroyLocalArrays(&freqArr, &sortedArr);
    return SUCCESS;
}

Status RadixSort(int *_arr, size_t _length)
{
    int maxNum;
    int exp;
    if (_arr == NULL)
    {
        return FAILURE;
    }
    if (_length <= 1)
    {
        return SUCCESS;
    }
    maxNum = GetMax(_arr, _length);
    for (exp = 1; (maxNum / exp) > 0; exp *= 10)
    {
        CountingSortForRadix(_arr, _length, exp);
    }
    return SUCCESS;
}

int main()
{
    size_t length;
    int *tempArray, *baseArray;

    sortFunc sortFuncs[] = {BubbleSort, ShakeSort, QuickSort, InsertionSort, ShellSort, SelectionSort, CountingSort, MergeSortRec, MergeSortIt,RadixSort};
    size_t sortsLength = sizeof(sortFuncs) / sizeof(sortFuncs[0]);
    char *sortNames[] = {"BubbleSort", "ShakeSort", "QuickSort", "InsertionSort", "ShellSort", "SelectionSort", "CountingSort", "MergeSortRec", "MergeSortIt","RadixSort"};

    int baseArray1[FIRST_ARRAY_SIZE];
    int baseArray2[SECOND_ARRAY_SIZE];
    int baseArray3[THIRD_ARRAY_SIZE];
    InitWithRandoms(baseArray1, FIRST);
    InitWithRandoms(baseArray2, SECOND);
    InitWithRandoms(baseArray3, THIRD);

    for (size_t testCase = FIRST; testCase <= THIRD; testCase++)
    {
        switch (testCase)
        {
        case FIRST:
            baseArray = baseArray1;
            length = FIRST_ARRAY_SIZE;
            break;
        case SECOND:
            baseArray = baseArray2;
            length = SECOND_ARRAY_SIZE;
            break;
        case THIRD:
            baseArray = baseArray3;
            length = THIRD_ARRAY_SIZE;
            break;
        default:
            break;
        }
        printf("\n**************************************\n");
        printf("[Run %ld | Array size: %ld]\n\n", testCase, length);

        tempArray = malloc(length * sizeof(int));
        if (tempArray == NULL)
        {
            return 1;
        }

        for (size_t index = 0; index < sortsLength; index++)
        {
            RunSort(tempArray, baseArray, length, sortFuncs[index], sortNames[index]);
        }
        free(tempArray);
    }

    return 0;
}

static void RunSort(int *_tempArray, int *_baseArray, size_t _length, sortFunc _sortFunc, char *_sortName)
{
    clock_t start, end;
    double time;
    memcpy(_tempArray, _baseArray, _length * sizeof(int));
    start = clock();
    _sortFunc(_tempArray, _length);
    end = clock();
    time = TIME_IN_SEC(start, end);
    printf("%s took %f seconds.\n", _sortName, time);
}

static void GetProperties(size_t *_length, size_t *_modRange, int _case)
{
    switch (_case)
    {
    case FIRST:
        *_length = FIRST_ARRAY_SIZE;
        *_modRange = FIRST_MODULO_RANGE;
        break;
    case SECOND:
        *_length = SECOND_ARRAY_SIZE;
        *_modRange = SECOND_MODULO_RANGE;
        break;
    case THIRD:
        *_length = THIRD_ARRAY_SIZE;
        *_modRange = THIRD_MODULO_RANGE;
        break;
    default:
        break;
    }
}

static void InitWithRandoms(int *_arr, int _case)
{
    size_t length, modRange;
    GetProperties(&length, &modRange, _case);

    for (size_t index = 0; index < length; index++)
    {
        _arr[index] = rand() % modRange;
    }
}

static void PrintArray(int *_arr, size_t length)
{
    size_t i;
    printf("[");
    for (i = 0; i < length - 1; i++)
    {
        printf("%d,", _arr[i]);
    }
    printf("%d]\n", _arr[i]);
}

static int ShakeRight(int *_arr, size_t _begin, size_t _end)
{
    size_t index;
    int breakFlag = 1;

    for (index = _begin; index < _end - 1; index++)
    {
        if (_arr[index] > _arr[index + 1])
        {
            SWAP(_arr, index, index + 1);
            breakFlag = 0;
        }
    }
    return breakFlag;
}

static int ShakeLeft(int *_arr, size_t _begin, size_t _end)
{
    size_t index;
    int breakFlag = 1;
    for (index = _end - 1; index > _begin; index--)
    {
        if (_arr[index] < _arr[index - 1])
        {
            SWAP(_arr, index, index - 1);
            breakFlag = 0;
        }
    }
    return breakFlag;
}

static size_t Partition(int *_arr, size_t _begin, size_t _end)
{
    size_t left = _begin + 1;
    size_t right;
    int pivotValue = _arr[_begin];

    for (right = left; right < _end; right++)
    {
        if (_arr[right] <= pivotValue)
        {
            SWAP(_arr, left, right);
            ++left;
        }
    }
    SWAP(_arr, _begin, left - 1);

    return left - 1;
}

static void IsSorted(int *_arr, size_t _length)
{
    size_t index;
    int count = 0;
    printf("SORTED? ");
    for (index = 0; index < _length - 1; index++)
    {
        if (_arr[index] > _arr[index + 1])
        {
            count++;
        }
    }
    if (count > 0)
    {
        printf("NO, %d diff..\n", count);
    }
    else
    {
        printf("YES\n");
    }
}

static size_t GetIndexOfMin(int *_arr, size_t _begin, size_t _end)
{
    size_t result = _begin;
    size_t index;
    int currMin = _arr[_begin];

    for (index = _begin + 1; index < _end; index++)
    {
        if (_arr[index] < currMin)
        {
            result = index;
            currMin = _arr[index];
        }
    }
    return result;
}

static Status QuickSortRec(int *_arr, size_t _begin, size_t _end)
{
    if (_end - _begin <= 1)
    {
        return SUCCESS;
    }

    size_t pivot = Partition(_arr, _begin, _end);

    QuickSortRec(_arr, _begin, pivot);
    QuickSortRec(_arr, pivot + 1, _end);

    return SUCCESS;
}

static Status InsertionSortWithGap(int *_arr, size_t _length, size_t _begin, size_t _gap)
{
    size_t index1, index2;
    int curr;

    for (index1 = _begin; index1 < _length; index1 += _gap)
    {
        curr = _arr[index1];
        index2 = index1;
        while (index2 >= _gap && _arr[index2 - _gap] > curr)
        {
            _arr[index2] = _arr[index2 - _gap];
            index2 -= _gap;
        }
        _arr[index2] = curr;
    }
    return SUCCESS;
}

static void CountFreq(int *_baseArr, size_t _length, int *_freqArr, int _bias)
{
    size_t index;
    int currVal;
    for (index = 0; index < _length; index++)
    {
        currVal = _baseArr[index];
        ++_freqArr[currVal - _bias];
    }
}

static void ComputeCumulativeFreq(int *_freqArr, int _range)
{
    size_t index;
    for (index = 1; index < _range; index++)
    {
        _freqArr[index] += _freqArr[index - 1];
    }
}

static void FillSortedArray(int *_baseArr, size_t _length, int *_freqArr, int *_sortedArr, int _bias)
{
    size_t index = _length;
    size_t sortedPos;
    int currVal;
    while (index > 0)
    {
        --index;
        currVal = _baseArr[index];
        sortedPos = _freqArr[currVal - _bias];

        _sortedArr[sortedPos] = currVal;
        --(_freqArr[currVal - _bias]);
    }
}

static void DestroyLocalArrays(int **_freqArr, int **_sortedArr)
{
    free(*_freqArr);
    *_freqArr = NULL;
    free(*_sortedArr);
    *_sortedArr = NULL;
}

static void GetMinMaxValues(int *_arr, size_t _length, int *_min, int *_max)
{
    size_t index;
    int candMin = INT_MAX, candMax = INT_MIN;
    for (index = 0; index < _length; index++)
    {
        candMin = fmin(candMin, _arr[index]);
        candMax = fmax(candMax, _arr[index]);
    }
    *_min = candMin;
    *_max = candMax;
}

static void FillTail(int *_firstArr, size_t _firstLen, size_t _firstIndex, int *_secondArr, size_t _secondLen, size_t _secondIndex, int *_mergedArr, size_t _mergedIndex)
{
    while (_firstIndex < _firstLen)
    {
        _mergedArr[_mergedIndex] = _firstArr[_firstIndex];
        ++_firstIndex;
        ++_mergedIndex;
    }

    while (_secondIndex < _secondLen)
    {
        _mergedArr[_mergedIndex] = _secondArr[_secondIndex];
        ++_secondIndex;
        ++_mergedIndex;
    }
}

static void MergeIntegerArrays(int *_firstArr, size_t _firstLen, int *_secondArr, size_t _secondLen, int *_mergedArr)
{
    size_t firstIndex = 0, secondIndex = 0, mergedIndex = 0;
    while (firstIndex < _firstLen && secondIndex < _secondLen)
    {
        if (_firstArr[firstIndex] <= _secondArr[secondIndex])
        {
            _mergedArr[mergedIndex++] = _firstArr[firstIndex++];
        }
        else
        {
            _mergedArr[mergedIndex++] = _secondArr[secondIndex++];
        }
    }

    FillTail(_firstArr, _firstLen, firstIndex, _secondArr, _secondLen, secondIndex, _mergedArr, mergedIndex);
}

static Status AllocateArrays(int **_freqArr, size_t _range, int **_sortedArr, size_t _length)
{
    *_freqArr = calloc(_range, sizeof(int));
    if (*_freqArr == NULL)
    {
        return FAILURE;
    }
    *_sortedArr = malloc(sizeof(int) * _length);
    if (*_sortedArr == NULL)
    {
        free(*_freqArr);
        return FAILURE;
    }
    return SUCCESS;
}

static void GetCurrProp(size_t *_mid, size_t *_end, size_t *_begin, size_t _currLen, size_t _length, size_t *_leftLen, size_t *_rightLen)
{
    *_mid = fmin(*_begin + _currLen - 1, _length - 1);
    *_end = fmin(*_begin + 2 * _currLen - 1, _length - 1);
    *_leftLen = *_mid - *_begin + 1;
    *_rightLen = *_end - *_mid;
}

static void MergeRec(int *_arr, size_t _begin, size_t _end, int *_tempArr)
{
    size_t length = _end - _begin;
    size_t mid = _begin + length / 2;

    if (length <= 1)
    {
        return;
    }

    MergeRec(_arr, _begin, mid, _tempArr);
    MergeRec(_arr, mid, _end, _tempArr);

    MergeIntegerArrays(&_arr[_begin], mid - _begin, &_arr[mid], _end - mid, &_tempArr[_begin]);

    memcpy(&_arr[_begin], &_tempArr[_begin], length * sizeof(int));
}

static int GetMax(int *_arr, size_t _length)
{
    int max = _arr[0];
    for (size_t i = 1; i < _length; i++)
    {
        if (_arr[i] > max)
        {
            max = _arr[i];
        }
    }
    return max;
}

static void CountingSortForRadix(int *_arr, size_t _length, int _exp)
{
    int *output = malloc(_length * sizeof(int));
    int count[10] = {0};
    size_t index;

    if (output == NULL)
    {
        return;
    }

    for (index = 0; index < _length; index++)
    {
        int digit = (_arr[index] / _exp) % 10;
        count[digit]++;
    }

    for (index = 1; index < 10; index++)
    {
        count[index] += count[index - 1];
    }

    for (index = _length; index > 0; index--)
    {
        int digit = (_arr[index - 1] / _exp) % 10;
        output[count[digit] - 1] = _arr[index - 1];
        count[digit]--;
    }

    memcpy(_arr, output, _length * sizeof(int));

    free(output);
}