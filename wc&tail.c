#include "wc&tail.h"

void UpdateCounters(char c, int *numOfLines, int *numOfWords, Bool *inWord)
{
    /* Outside of a word, should pass all of the blanks */
    if (isspace(c))
    {
        if (c == '\n')
        {
            ++(*numOfLines);
        }
        *inWord = FALSE;
    }
    /* Inside of a word */
    else
    {
        /* Count a word once then walk through it without counting */
        if (*inWord == FALSE)
        {
            ++(*numOfWords);
            *inWord = TRUE;
        }
    }
}

void WalkTheLine(char *line, int *numOfLines, int *numOfWords)
{
    char c;
    Bool inWord = FALSE;
    int index = 0;
    c = line[index];

    while (c != '\0')
    {
        UpdateCounters(c, numOfLines, numOfWords, &inWord);
        c = line[++index];
    }
}

Status Wc(const char *fileName)
{
    FILE *fp;
    char c;
    int index;
    char line[MAX_LINE_LENGTH];
    int numOfChars = 0;
    int numOfWords = 0;
    int numOfLines = 0;

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return FILE_OPEN_FAILED;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        WalkTheLine(line, &numOfLines, &numOfWords);
    }
    numOfChars = ftell(fp);

    printf(" %d  %d %d %s\n", numOfLines, numOfWords, numOfChars, fileName);

    fclose(fp);
    return OK;
}

long GetFileSize(FILE *fp)
{
    long result;
    if (fseek(fp, 0, SEEK_END) != 0)
    {
        result = -1;
    }
    else
    {
        result = ftell(fp);
    }
    return result;
}

void ShapeNumberOfLines(int *desiredNumberOfLines)
{
    if (*desiredNumberOfLines > MAX_LINES_ALLOWED)
    {
        *desiredNumberOfLines = MAX_LINES_ALLOWED;
    }
}

int SafeMod(int a, int b)
{
    int result = a % b;
    if (result < 0)
    {
        result += b;
    }
    return result;
}

void PrintLines(FILE *fp, long *positions, int desiredNumberOfLines, int index, int lineCount)
{
    char temp[MAX_LINE_LENGTH];
    int linesToPrint = MIN(lineCount, desiredNumberOfLines);
    int startIndex;

    if (lineCount < desiredNumberOfLines)
    {
        startIndex = 0;
    }
    else
    {
        startIndex = index % desiredNumberOfLines;
    }

    int pos_index = SafeMod(startIndex, desiredNumberOfLines);
    if (fseek(fp, positions[pos_index], SEEK_SET) != 0)
    {
        return;
    }
    while (fgets(temp, MAX_LINE_LENGTH, fp) != NULL)
    {
        printf("%s", temp);
    }
}

int SafeModIndex(int index, int desiredNumberOfLines, int lineCount)
{
    int tempIndex = index - MIN(lineCount, desiredNumberOfLines) - 1;
    return SafeMod(tempIndex, desiredNumberOfLines);
}

void FillPosition(long *positions, long pos, int *index, int desiredNumberOfLines, int *lineCount)
{
    positions[*index] = pos;
    *index = (*index + 1) % desiredNumberOfLines;
    ++(*lineCount);
}

void SkipFirstPartial(FILE *fp, long offset, long fileSize, char *temp)
{
    if (offset != fileSize)
    {
        /* EOF reached */
        if (fgets(temp, MAX_LINE_LENGTH, fp) == NULL)
        {
            return;
        }
    }
}

void AdjustOffset(long *estimatedLineLength, long *estimatedOffset, long *offset, long fileSize, int desiredNumberOfLines, int lineCount)
{
    *estimatedLineLength = (lineCount > 0) ? (*offset / lineCount) : INITIAL_AVG_LINE_SIZE;
    *estimatedOffset = desiredNumberOfLines * (*estimatedLineLength);
    *offset = (*estimatedOffset < fileSize) ? *estimatedOffset : fileSize;
}

Status Tail(const char *fileName, int desiredNumberOfLines)
{
    FILE *fp;
    long fileSize;
    long *positions;
    char temp[MAX_LINE_LENGTH];
    long pos;
    long estimatedLineLength;
    long estimatedOffset;
    long offset;
    int lineCount;
    int index;

    if (desiredNumberOfLines <= 0)
    {
        return INVALID_NUMBER_OF_LINES;
    }

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return FILE_OPEN_FAILED;
    }

    fileSize = GetFileSize(fp);
    if (fileSize <= 0)
    {
        fclose(fp);
        return EMPTY_FILE;
    }

    ShapeNumberOfLines(&desiredNumberOfLines);

    positions = malloc(desiredNumberOfLines * sizeof(long));
    if (positions == NULL)
    {
        fclose(fp);
        return ALLOCATE_MEMORY_FAILED;
    }

    index = 0;
    lineCount = 0;

    while (1)
    {
        AdjustOffset(&estimatedLineLength, &estimatedOffset, &offset, fileSize, desiredNumberOfLines, lineCount);

        /* Find position by -offset from the end of the file */
        if (fseek(fp, -offset, SEEK_END) != 0)
        {
            break;
        }
        SkipFirstPartial(fp, offset, fileSize, temp);

        while (1)
        {
            pos = ftell(fp);
            /* Break if current position reached the boundary gap position [........| V V V ] (meaning the beggining of the already found lines) */
            if (lineCount >= desiredNumberOfLines && pos == positions[SafeModIndex(index, desiredNumberOfLines, lineCount)])
            {
                break;
            }

            if (fgets(temp, MAX_LINE_LENGTH, fp) == NULL)
            {
                /* EOF reached */
                break;
            }
            if (strlen(temp) == MAX_LINE_LENGTH)
            {
                continue;
            }
            FillPosition(positions, pos, &index, desiredNumberOfLines, &lineCount);
        }

        /* Check if we have read enough lines or reached the start of the file */
        if (lineCount >= desiredNumberOfLines || offset == fileSize)
        {
            break;
        }
    }

    PrintLines(fp, positions, desiredNumberOfLines, index, lineCount);

    fclose(fp);
    free(positions);
    return OK;
}
