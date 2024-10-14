#include "appointment_diary.h"

void Swap(pMeeting *m1, pMeeting *m2)
{
    pMeeting temp = *m1;
    *m1 = *m2;
    *m2 = temp;
}

void SortMeetings(pMeeting *meetings, int numOfMeetings)
{
    int i, j;

    if (meetings == NULL)
    {
        return;
    }

    for (i = 0; i < numOfMeetings - 1; i++)
    {
        for (j = 0; j < numOfMeetings - i - 1; j++)
        {
            if (meetings[j]->begin > meetings[j + 1]->begin)
            {
                Swap(&meetings[j], &meetings[j + 1]);
            }
        }
    }
}

int BinarySearch(pMeeting *meetings, float begin, int length)
{
    int low = 0;
    int high = length - 1;
    int mid;
    while (low <= high)
    {
        mid = low + (high - low) / 2;

        if (begin == (meetings)[mid]->begin)
            return mid;

        if (begin > meetings[mid]->begin)
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

pCalendar CreateAD(int meetingsSize, int blockSize)
{
    pCalendar newAD;
    pMeeting *meetings;

    if (meetingsSize == 0 && blockSize == 0)
    {
        return NULL;
    }

    newAD = (pCalendar)malloc(sizeof(Calendar));
    if (newAD == NULL)
    {
        return NULL;
    }

    meetings = (pMeeting *)malloc(meetingsSize * sizeof(pMeeting));
    if (meetings == NULL)
    {
        return NULL;
    }

    newAD->meetings = meetings;
    newAD->blockSize = blockSize;
    newAD->meetingsSize = meetingsSize;
    newAD->numOfMeetings = 0;

    return newAD;
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

pMeeting FindMeeting(pCalendar calendar, float begin)
{
    pMeeting foundMeeting;
    int index;

    if (begin < MEETING_BEGIN_BOUNDARY || begin > MEETING_END_BOUNDARY || calendar == NULL || calendar->meetings == NULL)
    {
        foundMeeting = NULL;
    }

    index = BinarySearch((calendar->meetings), begin, calendar->numOfMeetings);

    if (index == -1)
    {
        foundMeeting = NULL;
    }
    else
    {
        foundMeeting = (calendar->meetings)[index];
    }

    return foundMeeting;
}

int IsOverlap(pCalendar calendar, float begin, float end)
{
    int result = FALSE;
    int index;
    pMeeting currMeeting;
    for (index = 0; index < calendar->numOfMeetings; index++)
    {
        currMeeting = calendar->meetings[index];
        if (begin < currMeeting->end && end > currMeeting->begin)
        {
            return TRUE;
        }
    }
    return result;
}

int InsertMeeting(pCalendar calendar, pMeeting meeting)
{
    pMeeting *temp;
    int newSize;
    int overlap;

    if (calendar == NULL || calendar->meetings == NULL || meeting == NULL)
    {
        return NULL_PTR_ERROR;
    }

    if (meeting->begin < MEETING_BEGIN_BOUNDARY || meeting->end > MEETING_END_BOUNDARY || meeting->begin >= meeting->end)
    {
        return INSERT_FAILED;
    }

    if (IsOverlap(calendar, meeting->begin, meeting->end))
    {
        return OVERLAP;
    }

    if (calendar->numOfMeetings == calendar->meetingsSize)
    {
        if (calendar->blockSize == 0)
        {
            return OVERFLOW;
        }

        newSize = calendar->meetingsSize + calendar->blockSize;
        temp = realloc(calendar->meetings, newSize * sizeof(pMeeting));
        if (temp == NULL)
        {
            return REALLOC_FAILED;
        }
        calendar->meetings = temp;
        calendar->meetingsSize = newSize;
    }

    calendar->meetings[calendar->numOfMeetings] = meeting;
    calendar->numOfMeetings++;

    SortMeetings(calendar->meetings, calendar->numOfMeetings);

    return OK;
}

int RemoveMeeting(pCalendar calendar, float begin)
{
    pMeeting foundMeeting;
    if (calendar == NULL || calendar->meetings == NULL)
    {
        return NULL_PTR_ERROR;
    }
    if (calendar->numOfMeetings == 0)
    {
        return UNDERFLOW;
    }

    foundMeeting = FindMeeting(calendar, begin);
    if (foundMeeting == NULL)
    {
        return NOT_FOUND;
    }
    foundMeeting->begin = MEETING_END_BOUNDARY + 1;
    SortMeetings(calendar->meetings, calendar->numOfMeetings);
    free(foundMeeting);

    foundMeeting = NULL;
    calendar->numOfMeetings--;

    return OK;
}

int PrintAD(pCalendar AD)
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

void DestroyAD(pCalendar *calendar)
{
    int index;
    if (calendar == NULL || (*calendar) == NULL)
    {
        return;
    }

    for (index = 0; index < (*calendar)->numOfMeetings; index++)
    {
        free(((*calendar)->meetings)[index]);
    }
    free((*calendar)->meetings);
    free(*calendar);
    *calendar == NULL;
}
