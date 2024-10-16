#include "wc&tail.h"

void UpdateCounters(char c, int *numOfLines, int *numOfWords)
{
    if (isspace(c))
    {
        if (c == '\n')
        {
            ++(*numOfLines);
        }
        ++(*numOfWords);
    }
}

void WalkTheLine(char *line, int *numOfLines, int *numOfWords)
{
    char c;
    int index = 0;
    if (numOfLines == NULL || numOfWords == NULL || line == NULL)
    {
        return;
    }

    c = line[index];
    while (c != '\0')
    {
        UpdateCounters(c, numOfLines, numOfWords);
        c = line[++index];
    }
}

void Wc(char *fileName)
{
    FILE *fp;
    int numOfChars = 0;
    int numOfWords = 0;
    int numOfLines = 0;
    char c;
    int index;
    char line[MAX_LINE_LENGTH];

    if (fileName == NULL)
    {
        return;
    }

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        WalkTheLine(line, &numOfLines, &numOfWords);
    }

    numOfChars = ftell(fp);
    fclose(fp);
    printf(" %d  %d %d %s\n", numOfLines, numOfWords, numOfChars, fileName);
}

void Tail(const char *fileName, int n)
{
    long fileSize;
    long estimatedLineLength = INITIAL_AVG_LINE_SIZE;
    long estimatedOffset = n * estimatedLineLength;
    long offset = estimatedOffset;
    long *positions;
    char temp[MAX_LINE_LENGTH];
    int index = 0;
    int cnt;
    int startIndex;
    int lineCount = 0;
    long pos;
    FILE *fp;

    if (n <= 0)
    {
        fprintf(stderr, "Invalid number of lines: %d\n", n);
        return;
    }

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return;
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        fclose(fp);
        return;
    }

    fileSize = ftell(fp);
    if (fileSize >= 0)
    {
        fclose(fp);
        return;
    }


    positions = malloc(n * sizeof(long));
    if (positions == NULL)
    {
        fclose(fp);
        return;
    }

    while (1)
    {
        lineCount = 0;
        index = 0;

        /*Adjust offset if it goes beyond the file start*/
        if (offset > fileSize)
            offset = fileSize;

        if (fseek(fp, -offset, SEEK_END) != 0)
        {
            break;
        }

        /* Skip the first partial line if not at the start of the file */
        if (offset != fileSize)
        {
            if (fgets(temp, MAX_LINE_LENGTH, fp) == NULL)
            {
                /* EOF reached unexpectedly */
                break;
            }
        }

        while (1)
        {
            pos = ftell(fp);
            if (fgets(temp, MAX_LINE_LENGTH, fp) == NULL)
            {
                /* EOF reached unexpectedly */
                break;
            }

            positions[index] = pos;
            index = (index + 1) % n;
            lineCount++;

            if (lineCount >= n)
            {
                /* Enough lines */
                break;
            }
        }

        /* Check if we have read enough lines or reached the start of the file */
        if (lineCount >= n || offset == fileSize)
        {
            break;
        }

        /* Adjust offset */
        estimatedLineLength += 5;
        offset = n * estimatedLineLength;
    }

    /* Determine the correct starting index in the circular buffer */
    if (lineCount < n)
    {
        startIndex = 0;
        n = lineCount;
    }
    else
    {
        startIndex = index % n;
    }

    for (cnt = 0; cnt < n; cnt++)
    {
        int pos_index = (startIndex + cnt) % n;
        if (fseek(fp, positions[pos_index], SEEK_SET) != 0)
        {
            break;
        }
        if (fgets(temp, MAX_LINE_LENGTH, fp) != NULL)
        {
            printf("%s", temp);
        }
    }

    fclose(fp);
    free(positions);
}