#ifndef __appointment_diary__
#define __appointment_diary__

#include <stdlib.h>
#define MEETING_BEGIN_BOUNDARY 0.0
#define MEETING_END_BOUNDARY 23.99
#define OK 1
#define NULL_PTR_ERROR -1
#define UNDERFLOW -2
#define REALLOC_FAILED -3
#define OVERFLOW -4
#define NOT_FOUND -1
#define INSERT_FAILED -1

typedef struct Meeting_t
{
    float begin;
    float end;
    int room;

} Meeting, *pMeeting;

typedef struct Calander_t
{
    Meeting **meetings;
    int numOfMeetings;
    int meetingsSize;
    int blockSize;

} Calander, *pCalander;

pCalander CreateAD(int meetingsSize, int blockSize);

pMeeting CreateMeeting(float begin, float end, int room);

pMeeting FindMeeting(pCalander calander, int begin);

int InsertMeeting(pMeeting meeting);

int RemoveMeeting(pCalander calander, int begin);

int PrintAD(pCalander AD);

void DestroyAD(pCalander *calander);

#endif /* __appointment_diary__ */