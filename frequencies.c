#include "frequencies.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define NUM_OF_LETTERS 52
#define INITIAL_BUFFER_SIZE 1000
#define UPPER_CASES_START_POSITION 26
#define LINE_SIZE 100
#define WORD_SIZE 25

typedef struct Node
{
    struct Node* next;
    char word[WORD_SIZE];
    int freq;
} Node, *pNode;

void WordToLower(char *word)
{
    int index = 0;
    while (word[index] != '\0')
    {
        word[index] = tolower(word[index]);
        ++index;
    }
}

void DestroyList(pNode *head)
{
    pNode temp = *head;
    while (*head != NULL)
    {
        *head = (*head)->next;
        free(temp);
        temp = *head;
    }
}

void PrintWordFrequency(pNode node)
{
    if (node != NULL)
    {
        printf("%s\t%d\n", node->word, node->freq);
    }
}

void PrintWordsFrequencies(pNode head)
{
    while (head != NULL)
    {
        PrintWordFrequency(head);
        head = head->next;
    }
}

Status CreateNode(pNode *newNode, char *word)
{
    if (word == NULL)
    {
        return NULL_PTR_ERR;
    }

    *newNode = (pNode)malloc(sizeof(Node));
    if (newNode != NULL)
    {
        if (strlen(word) > WORD_SIZE)
        {
            return WORD_SIZE_OVERFLOW;
        }
        strcpy((*newNode)->word, word);
        (*newNode)->freq = 1;
        (*newNode)->next = NULL;
        return OK;
    }
    else
    {
        return ALLOCATE_MEMORY_FAILED;
    }
}

Status InsertNewNode(pNode *head, char *word)
{
    pNode temp;
    pNode newNode;
    Status status;
    status = CreateNode(&newNode, word);
    if (status != OK)
    {
        return status;
    }
    if (*head != NULL)
    {
        newNode->next = *head;
    }

    *head = newNode;

    return OK;
}

pNode SearchWord(pNode head, char *word)
{
    pNode temp = head;
    while (temp != NULL && temp->next != NULL)
    {
        printf("%s", temp->word);
        if (strcmp(temp->word, word) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

Status IncrementWordFreq(pNode *head, char *word)
{
    Status status = OK;
    pNode temp;
    WordToLower(word);

    temp = SearchWord(*head, word);
    if (temp != NULL)
    {
        ++(temp->freq);
    }
    else
    {
        status = InsertNewNode(head, word);
    }
    return status;
}

Status IncrementWordsFrequencies(pNode *list, char *line)
{
    const char *delimiters = " \t\n\"'`:;,.}{)(][/=+-!@#$^&*";
    char *word;
    word = strtok(line, delimiters);
    while (word != NULL)
    {
        IncrementWordFreq(list, word);
        word = strtok(NULL, delimiters);
    }
}

void IncrementFrequency(int *frequencies, char c)
{
    int index;
    if (c >= 'A' && c <= 'Z')
    {
        index = UPPER_CASES_START_POSITION + (c - 'A');
    }
    else
    {
        index = c - 'a';
    }
    ++frequencies[index];
}

Status CalculateFrequencies(char *buffer, int *frequencies, int bytesReceived)
{
    char c;
    int index = 0;

    if (bytesReceived < 0)
    {
        return INVALID_NUMBER_OF_BYTES;
    }

    while (index < bytesReceived)
    {
        c = buffer[index++];
        if (isalpha((unsigned char)c))
        {
            IncrementFrequency(frequencies, c);
        }
    }
    return OK;
}

char GetCurrentLetter(int index)
{
    return (index >= UPPER_CASES_START_POSITION) ? 'A' + (index - UPPER_CASES_START_POSITION) : 'a' + index;
}

void PrintLettersFrequencies(int *frequencies)
{
    int index;
    char currLetter;
    int currFreq;
    for (index = 0; index < NUM_OF_LETTERS; index++)
    {
        currFreq = frequencies[index];
        if (currFreq > 0)
        {
            currLetter = GetCurrentLetter(index);
            printf("%c\t%d\n", currLetter, currFreq);
        }
    }
}



Status LetterFrequencies(const char *fileName)
{
    FILE *fp;
    int frequencies[NUM_OF_LETTERS] = {0};
    long fileSize;
    char buffer[INITIAL_BUFFER_SIZE];
    int bytesReceived;
    Status status = OK;

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        return OPEN_FILE_FAILED;
    }

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while ((bytesReceived = fread(buffer, sizeof(char), INITIAL_BUFFER_SIZE, fp)) > 0)
    {
        status = CalculateFrequencies(buffer, frequencies, bytesReceived);
        if (status != OK)
        {
            fclose(fp);
            return status;
        }
    }

    PrintLettersFrequencies(frequencies);
    fclose(fp);

    return status;
}

Status WordsFrequencies(const char *fileName)
{
    FILE *fp;
    char line[LINE_SIZE];
    pNode list = NULL;

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        return OPEN_FILE_FAILED;
    }

    while (fgets(line, LINE_SIZE, fp) != NULL)
    {
        IncrementWordsFrequencies(&list, line);
    }
    PrintWordsFrequencies(list);

    DestroyList(&list);
    return OK;
}