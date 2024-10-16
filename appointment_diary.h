#ifndef __appointment_diary__
#define __appointment_diary__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MEETING_BEGIN_BOUNDARY 0.0
#define MEETING_END_BOUNDARY 23.99

/*******************  Should Add Doc *************************/

typedef enum Participant
{
    ALICE,
    BOB,
    ROBERT,
    EMILY,
    GEORGE,
    ANGELINA,
    MATHIEW,
    HARRY
} Participant;

typedef struct Meeting_t
{
    float begin;
    float end;
    int room;
    int numOfParts;
    Participant *participants;
} Meeting, *pMeeting;

typedef struct Calendar_t
{
    Meeting **meetings;
    int numOfMeetings;
    int meetingsSize;
    int blockSize;

} Calendar, *pCalendar;

typedef enum Status
{
    OVERLAP = -6,
    INSERT_FAILED,
    NOT_FOUND,
    OVERFLOW,
    REALLOC_FAILED,
    UNDERFLOW,
    NULL_PTR_ERROR,
    OK
} Status;

typedef enum Room
{
    JERUSALEM,
    PARIS,
    LONDON,
    MADRID,
    KYIV,
    ROME,
    VIENNA,
    COPENHAGEN
} Room;

typedef enum Bool
{
    FALSE,
    TRUE
} Bool;

pCalendar CreateAD(int meetingsSize, int blockSize);

pMeeting CreateMeeting(float begin, float end, Room room, Participant *parts, int numOfParts);

pMeeting FindMeeting(pCalendar calendar, float begin);

Status InsertMeeting(pCalendar calendar, pMeeting meeting);

Status RemoveMeeting(pCalendar calendar, float begin);

Status PrintAD(pCalendar AD);

void DestroyAD(pCalendar *calendar);

#endif /* __appointment_diary__ */