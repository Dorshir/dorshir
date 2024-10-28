#include <stdio.h>
#include <string.h>
#define NAME 0
#define ID 1
#define AGE 2
#define EDUCATION 3

typedef struct Person
{
    char name[25];
    int id;
    int age;
    int education;
} Person;

void printPersonsArray(Person array[], size_t length);

void sortPersonsArray(Person array[], size_t length, int by);