#include "structs.h"


void swap(Person *arr, int i, int j)
{
    Person temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void printPersonsArray(Person array[], size_t length)
{
    int index;
    printf("==== Printing persons ====\n");
    for(index = 0; index < length; index++){
        printf("Name: %s\nId: %d\nAge: %d\nEducation: %d\n\n", array[index].name, array[index].id, array[index].age, array[index].education);
    }
}

void sortPersonsArray(Person array[], size_t length, int criterion)
{
    int i, j;
    switch (criterion)
    {
    case NAME:
        for (i = 0; i < length - 1; i++)
        {
            for (j = 0; j < length - i - 1; j++)
            {
                if (strcmp(array[j].name, array[j + 1].name) > 0)
                    swap(array, j, j + 1);
            }
        }
        break;
    case ID:
        for (i = 0; i < length - 1; i++)
        {
            for (j = 0; j < length - i - 1; j++)
            {
                if (array[j].id < array[j + 1].id)
                    swap(array, j, j + 1);
            }
        }
        break;
    case AGE:
        for (i = 0; i < length - 1; i++)
        {
            for (j = 0; j < length - i - 1; j++)
            {
                if (array[j].age < array[j + 1].age)
                    swap(array, j, j + 1);
            }
        }
        break;
    case EDUCATION:
        for (i = 0; i < length - 1; i++)
        {
            for (j = 0; j < length - i - 1; j++)
            {
                if (array[j].education < array[j + 1].education)
                    swap(array, j, j + 1);
            }
        }
        break;
    default:
    printf("Error: Invalid sorting criterion.\n");
        break;
    }
}