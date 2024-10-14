#ifndef __appointment_diary__
#define __appointment_diary__

#include <stdlib.h>
#include <stdio.h>
#define MEETING_BEGIN_BOUNDARY 0.0
#define MEETING_END_BOUNDARY 23.99
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

typedef enum
{
    OVERLAP = -7,
    INSERT_FAILED,
    NOT_FOUND,
    OVERFLOW,
    REALLOC_FAILED,
    UNDERFLOW,
    NULL_PTR_ERROR,
    OK = 1
} Status;

pCalendar CreateAD(int meetingsSize, int blockSize);

pMeeting CreateMeeting(float begin, float end, int room);

pMeeting FindMeeting(pCalendar calendar, float begin);

Status InsertMeeting(pCalendar calendar, pMeeting meeting);

Status RemoveMeeting(pCalendar calendar, float begin);

Status PrintAD(pCalendar AD);

Status DestroyAD(pCalendar *calendar);

#endif /* __appointment_diary__ */