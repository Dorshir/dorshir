#include "appointment_diary.h"
#define MEETING_BEGIN_BOUNDARY 0.0
#define MEETING_END_BOUNDARY 23.99
#define MAX_LINE_LENGTH 200
#define INITIAL_BLOCK_SIZE 2
#define INITIAL_MEETINGS_SIZE 4

static void Swap(pMeeting *m1, pMeeting *m2);
static void SortMeetings(pMeeting *meetings, int numOfMeetings);
static int BinarySearch(pMeeting *meetings, float begin, int length);
static Bool TimeOverlap(pMeeting newMeeting, pMeeting currMeeting, pMeeting nextMeeting);
static Bool PartOverlap(pMeeting newMeeting, pMeeting currMeeting);
static Bool RoomOverlap(pMeeting newMeeting, pMeeting currMeeting);
static Status IsOverlap(pCalendar calendar, pMeeting newMeeting);
static Status CheckInputInsert(pCalendar calendar, pMeeting meeting);
static Status CheckInputRemove(pCalendar calendar, float begin);
static Status ParseParticipants(FILE *fp, int numOfParts, Participant **participants);
static Status ValidateRoom(int room);
static Status ParseMeeting(FILE *fp, pMeeting *newMeeting);
static Status LoadMeetings(FILE *fp, pCalendar calendar);
static void MeetingToFile(FILE *fp, pMeeting meeting);
static void PrintMeeting(pMeeting meeting);
static void FreeMeetings(pCalendar calendar);
static void DestroyMeeting(pMeeting *meeting);

/************************************** Main functions ***************************************/

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
        free(newAD);
        return NULL;
    }

    newAD->meetings = meetings;
    newAD->blockSize = blockSize;
    newAD->meetingsSize = meetingsSize;
    newAD->numOfMeetings = 0;

    return newAD;
}

pMeeting CreateMeeting(float begin, float end, Room room, Participant *parts, int numOfParts)
{
    pMeeting newMeeting;

    if (begin >= end || end > MEETING_END_BOUNDARY || begin < MEETING_BEGIN_BOUNDARY || parts == NULL)
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
    newMeeting->numOfParts = numOfParts;

    newMeeting->participants = (Participant *)malloc(numOfParts * sizeof(Participant));
    if (newMeeting->participants == NULL)
    {
        free(newMeeting);
        return NULL;
    }

    memcpy(newMeeting->participants, parts, numOfParts * sizeof(Participant));
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

Status InsertMeeting(pCalendar calendar, pMeeting meeting)
{
    pMeeting *temp;
    int newSize;
    int inputCheck;

    inputCheck = CheckInputInsert(calendar, meeting);
    if (inputCheck != OK)
    {
        return inputCheck;
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

Status RemoveMeeting(pCalendar calendar, float begin)
{
    pMeeting foundMeeting;
    int inputCheck;
    if (inputCheck != OK)
    {
        return inputCheck;
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

Status LoadAD(pCalendar *calendar, char *fileName)
{
    FILE *fp;
    Status status = OK;

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        return OPEN_FILE_FAILED;
    }
    if (calendar == NULL || *calendar == NULL)
    {
        return NULL_PTR_ERROR;
    }
    if ((*calendar)->meetingsSize > 0)
    {
        printf("Warning! The diary had meetings that were deleted and replaced with the new data.\n");
        DestroyAD(calendar);
    }

    *calendar = CreateAD(INITIAL_MEETINGS_SIZE, INITIAL_BLOCK_SIZE);
    if (*calendar == NULL)
    {
        fclose(fp);
        return CALENDAR_CREATION_FAILED;
    }

    printf("%d", (*calendar)->meetingsSize);

    status = LoadMeetings(fp, *calendar);
    if (status != OK)
    {
        DestroyAD(calendar);
    }

    fclose(fp);
    return status;
}

Status StoreAD(pCalendar calendar, const char *fileName)
{
    FILE *fp;
    int index;
    if ((fp = fopen(fileName, "w")) == NULL)
    {
        return OPEN_FILE_FAILED;
    }

    for (index = 0; index < calendar->numOfMeetings; index++)
    {
        MeetingToFile(fp, calendar->meetings[index]);
    }

    fclose(fp);
    return OK;
}

Status PrintAD(pCalendar AD)
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
    if (calendar == NULL || (*calendar) == NULL)
    {
        return;
    }
    FreeMeetings(*calendar);
    (*calendar)->numOfMeetings = 0;
    (*calendar)->meetingsSize = 0;
    (*calendar)->blockSize = 0;

    free(*calendar);
    *calendar = NULL;
}

/************************************** Sub functions ***************************************/

static void Swap(pMeeting *m1, pMeeting *m2)
{
    pMeeting temp = *m1;
    *m1 = *m2;
    *m2 = temp;
}

static void SortMeetings(pMeeting *meetings, int numOfMeetings)
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

static int BinarySearch(pMeeting *meetings, float begin, int length)
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

static Bool TimeOverlap(pMeeting newMeeting, pMeeting currMeeting, pMeeting nextMeeting)
{
    return !(newMeeting->end <= currMeeting->begin || newMeeting->begin >= currMeeting->end);
}

static Bool PartOverlap(pMeeting newMeeting, pMeeting currMeeting)
{
    int index1;
    int index2;
    for (index1 = 0; index1 < newMeeting->numOfParts; index1++)
    {
        for (index2 = 0; index2 < currMeeting->numOfParts; index2++)
        {
            if (newMeeting->participants[index1] == currMeeting->participants[index2])
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

static Bool RoomOverlap(pMeeting newMeeting, pMeeting currMeeting)
{
    return newMeeting->room == currMeeting->room;
}

static Status IsOverlap(pCalendar calendar, pMeeting newMeeting)
{
    int result = FALSE;
    int timeOverlap;
    int partOverlap;
    int roomOverlap;
    int index;
    pMeeting currMeeting;
    pMeeting nextMeeting;

    for (index = 0; index < calendar->numOfMeetings; index++)
    {
        currMeeting = calendar->meetings[index];
        nextMeeting = (index < calendar->numOfMeetings - 1) ? calendar->meetings[index + 1] : NULL;
        timeOverlap = TimeOverlap(newMeeting, currMeeting, nextMeeting);
        if (timeOverlap)
        {
            partOverlap = PartOverlap(newMeeting, currMeeting);
            roomOverlap = RoomOverlap(newMeeting, currMeeting);
            if (roomOverlap || partOverlap)
            {
                result = TRUE;
                break;
            }
        }
    }
    return result;
}

static Status CheckInputInsert(pCalendar calendar, pMeeting meeting)
{
    Status check = OK;
    if (calendar == NULL || calendar->meetings == NULL || meeting == NULL)
    {
        check = NULL_PTR_ERROR;
    }
    else if (meeting->begin < MEETING_BEGIN_BOUNDARY || meeting->end > MEETING_END_BOUNDARY || meeting->begin >= meeting->end)
    {
        check = INSERT_FAILED;
    }
    else if (IsOverlap(calendar, meeting))
    {
        check = OVERLAP;
    }
    return check;
}

static Status CheckInputRemove(pCalendar calendar, float begin)
{
    Status check = OK;
    if (calendar == NULL || calendar->meetings == NULL)
    {
        check = NULL_PTR_ERROR;
    }
    if (calendar->numOfMeetings == 0)
    {
        check = UNDERFLOW;
    }
    return check;
}

static Status ParseParticipants(FILE *fp, int numOfParts, Participant **participants)
{
    int index;
    int participantId;
    Participant *p;
    if (numOfParts == 0)
    {
        return INVALID_PARTICIPANT_NUM;
    }

    *participants = (Participant *)malloc(numOfParts * sizeof(Participant));
    if (*participants == NULL)
    {
        return ALLOCATE_MEMORY_FAILED;
    }

    for (index = 0; index < numOfParts; index++)
    {
        if (fscanf(fp, "%d", &participantId) != 1)
        {
            free(*participants);
            return INVALID_PARTICIPANT_DATA;
        };

        if (participantId < ALICE || participantId > HARRY)
        {
            free(*participants);
            return INVALID_PARTICIPANT_DATA;
        }
        (*participants)[index] = (Participant)participantId;
    }
    return OK;
}

static Status ValidateRoom(int room)
{
    if (room < JERUSALEM || room > COPENHAGEN)
    {
        return INVALID_ROOM_NUM;
    }
    return OK;
}

static Status ParseMeeting(FILE *fp, pMeeting *newMeeting)
{
    float begin;
    float end;
    int room;
    int numOfParts;
    Participant *participants = NULL;
    *newMeeting = NULL;

    if (fscanf(fp, "%f %f %d %d", &begin, &end, &room, &numOfParts) != 4)
    {
        return INVALID_MEETING_DATA;
    }
    if (ValidateRoom(room) != OK)
    {
        return INVALID_ROOM_NUM;
    }
    if (ParseParticipants(fp, numOfParts, &participants) != OK)
    {
        return PARTICIPANTS_PARSING_FAILED;
    }

    *newMeeting = CreateMeeting(begin, end, (Room)room, participants, numOfParts);
    free(participants);

    if (*newMeeting == NULL)
    {
        return MEETING_CREATION_FAILED;
    }

    return OK;
}

static Status LoadMeetings(FILE *fp, pCalendar calendar)
{
    pMeeting newMeeting = NULL;
    Status status = OK;

    while (1)
    {
        status = ParseMeeting(fp, &newMeeting);
        if (newMeeting == NULL)
        {
            if (feof(fp))
            {
                break;
            }
            else
            {
                return status;
            }
        }
        status = InsertMeeting(calendar, newMeeting);
        if (status != OK)
        {
            DestroyMeeting(&newMeeting);
            return status;
        }

        newMeeting = NULL;
    }
    return OK;
}

static void MeetingToFile(FILE *fp, pMeeting meeting)
{
    int index = 0;
    fprintf(fp, "%f ", meeting->begin);
    fprintf(fp, "%f ", meeting->end);
    fprintf(fp, "%d ", meeting->room);
    fprintf(fp, "%d ", meeting->numOfParts);
    for (index = 0; index < meeting->numOfParts; index++)
    {
        fprintf(fp, "%d ", meeting->participants[index]);
    }
    fprintf(fp, "\n");
}

static void PrintMeeting(pMeeting meeting)
{
    if (meeting == NULL)
    {
        return;
    }
    printf("Meeting begin time: %f\nMeeting end time: %f\nMeeting room: %d\n", meeting->begin, meeting->end, meeting->room);
}

static void FreeMeetings(pCalendar calendar)
{
    int index;
    for (index = 0; index < calendar->numOfMeetings; index++)
    {
        free(calendar->meetings[index]->participants);
        free((calendar->meetings)[index]);
    }
    free(calendar->meetings);
    calendar->meetings = NULL;
}

static void DestroyMeeting(pMeeting *meeting)
{
    if (meeting != NULL && *meeting != NULL)
    {
        free((*meeting)->participants);
        free(*meeting);
        *meeting = NULL;
    }
}
