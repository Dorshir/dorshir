#include "appointment_diary.h"

void SortMeetings(pMeeting *meetings)
{
    /*  SORT BY BEGIN TIME */
}

int BinarySearch(pMeeting array[], int begin, int length)
{
    int low = 0;
    int high = length - 1;
    int mid;
    while (low <= high)
    {
        mid = low + (high - low) / 2;

        if (begin == array[mid]->begin)
            return mid;

        if (begin > array[mid]->begin)
            low = mid + 1;

        else
            high = mid - 1;
    }

    return -1;
}

void PrintMeeting(pMeeting meeting)
{
    if (meeting == NULL)
    {
        return;
    }
    printf("Meeting begin time: %f\nMeeting end time: %f\nMeeting room: %d\n", meeting->begin, meeting->end, meeting->room);
}

Calander *CreateAD(int meetingsSize, int blockSize)
{
    return NULL;
}

pMeeting CreateMeeting(float begin, float end, int room)
{
    pMeeting newMeeting;
    if (begin >= end || end > MEETING_END_BOUNDARY || begin < MEETING_BEGIN_BOUNDARY)
    {
        return NULL;
    }

    newMeeting = (pMeeting)malloc(sizeof(Meeting));
    if (newMeeting == NULL)
    {
        return NULL;
    }
    newMeeting->begin = begin;
    newMeeting->end = end;
    newMeeting->room = room;

    return newMeeting;
}

pMeeting FindMeeting(pCalander calander, int begin)
{
    pMeeting foundMeeting;
    int index;

    if (begin < MEETING_BEGIN_BOUNDARY || begin > MEETING_END_BOUNDARY || calander == NULL || calander->meetings == NULL)
    {
        foundMeeting = NULL;
    }

    index = BinarySearch(calander->meetings, begin, calander->meetingsSize);
    if (index == -1)
    {
        foundMeeting = NULL;
    }
    else
    {
        foundMeeting = (calander->meetings)[index];
    }

    return foundMeeting;
}

int InsertMeeting(pCalander calander, pMeeting meeting)
{
    int *temp;

    if (meeting->begin < MEETING_BEGIN_BOUNDARY || meeting->end > MEETING_END_BOUNDARY || meeting->begin >= meeting->end || calander == NULL || calander->meetings == NULL || meeting == NULL)
    {
        return INSERT_FAILED;
    }

    if (calander->numOfMeetings == calander->meetingsSize)
    {
        if (calander->blockSize == 0)
        {
            return OVERFLOW;
        }

        /* realoc... */
    }
    else
    {
        calander->meetings[calander->numOfMeetings] = meeting;
        calander->numOfMeetings++;
        return OK;
    }

    SortMeetings(calander->meetings);

    return OK;
}

int RemoveMeeting(pCalander calander, int begin)
{
    int index = -1;

    if (calander == NULL || calander->meetings == NULL)
    {
        return NULL_PTR_ERROR;
    }
    if (calander->numOfMeetings == 0)
    {
        return UNDERFLOW;
    }

    index = FindMeeting(calander, begin);
    if (index == -1)
    {
        return NOT_FOUND;
    }

    pMeeting foundMeeting = (calander->meetings)[index];
    free(foundMeeting);

    foundMeeting = NULL;
    calander->numOfMeetings--;

    return OK;
}

int PrintAD(pCalander AD)
{
    int index;
    if (AD == NULL || AD->meetings == NULL)
    {
        return NULL_PTR_ERROR;
    }

    for (index = 0; index < AD->numOfMeetings; index++)
    {
        printf("Meeting num %d:\n", index);
        PrintMeeting((AD->meetings)[index]);
    }

    return OK;
}

void DestroyAD(pCalander *calander)
{
    if (calander == NULL || (*calander) == NULL)
    {
        return;
    }
    free((*calander)->meetings);
    free(*calander);
    *calander == NULL;
}
