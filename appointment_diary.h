#ifndef __appointment_diary__
#define __appointment_diary__

#include <stdlib.h>
#include <stdio.h>
#define MEETING_BEGIN_BOUNDARY 0.0
#define MEETING_END_BOUNDARY 23.99
#define OK 1
#define NULL_PTR_ERROR -1
#define UNDERFLOW -2
#define REALLOC_FAILED -3
#define OVERFLOW -4
#define NOT_FOUND -5
#define INSERT_FAILED -6
#define OVERLAP -7

#define FALSE 0
#define TRUE 1

typedef struct Meeting_t
{
    float begin;
    float end;
    int room;

} Meeting, *pMeeting;

typedef struct Calendar_t
{
    Meeting **meetings;
    int numOfMeetings;
    int meetingsSize;
    int blockSize;

} Calendar, *pCalendar;

pCalendar CreateAD(int meetingsSize, int blockSize);

pMeeting CreateMeeting(float begin, float end, int room);

pMeeting FindMeeting(pCalendar calendar, float begin);

int InsertMeeting(pCalendar calendar, pMeeting meeting);

int RemoveMeeting(pCalendar calendar, float begin);

int PrintAD(pCalendar AD);

void DestroyAD(pCalendar *calendar);

#endif /* __appointment_diary__ */