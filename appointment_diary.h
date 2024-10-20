#ifndef __appointment_diary__
#define __appointment_diary__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MEETING_BEGIN_BOUNDARY 0.0
#define MEETING_END_BOUNDARY 23.99
#define MAX_LINE_LENGTH 200
#define INITIAL_BLOCK_SIZE 2
#define INITIAL_MEETINGS_SIZE 4

/**
 * @brief Enum representing participants in meetings.
 */
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

/**
 * @brief Enum representing status codes returned by functions.
 */
typedef enum Status
{
    INVALID_PARTICIPANT_DATA = -20,
    INVALID_PARTICIPANT_NUM,
    PARTICIPANTS_PARSING_FAILED,
    INVALID_MEETING_DATA,
    MEETING_PARSING_FAILED,
    ALLOCATE_MEMORY_FAILED,
    INVALID_ROOM_NUM,
    MEETING_CREATION_FAILED,
    CALENDAR_CREATION_FAILED,
    INVALID_FILE,
    OPEN_FILE_FAILED = -7,
    OVERLAP,
    INSERT_FAILED,
    NOT_FOUND,
    OVERFLOW,
    REALLOC_FAILED,
    UNDERFLOW,
    NULL_PTR_ERROR,
    OK
} Status;

/**
 * @brief Enum representing available meeting rooms.
 */
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

/**
 * @brief Structure representing a meeting.
 */
typedef struct Meeting_t
{
    float begin;
    float end;
    int room;
    int numOfParts;
    Participant *participants;
} Meeting, *pMeeting;

/**
 * @brief Structure representing an appointment diary.
 */
typedef struct Calendar_t
{
    Meeting **meetings;
    int numOfMeetings;
    int meetingsSize;
    int blockSize;
} Calendar, *pCalendar;

/**
 * @brief Creates a new appointment diary.
 *
 * @param meetingsSize  Initial size of the meetings array.
 * @param blockSize     Size increment for resizing the meetings array.
 *
 * @return      Pointer to the created appointment diary, or NULL on failure.
 */
pCalendar CreateAD(int meetingsSize, int blockSize);

/**
 * @brief Creates a new meeting.
 *
 * @param begin       Start time of the meeting.
 * @param end         End time of the meeting.
 * @param room        Meeting room.
 * @param parts       Array of participants.
 * @param numOfParts  Number of participants.
 *
 * @return  Pointer to the created meeting, or NULL on failure.
 */
pMeeting CreateMeeting(float begin, float end, Room room, Participant *parts, int numOfParts);

/**
 * @brief Finds a meeting in the appointment diary by its start time.
 *
 * @param calendar Pointer to the appointment diary.
 * @param begin    Start time of the meeting to find.
 *
 * @return Pointer to the found meeting, or NULL if not found.
 */
pMeeting FindMeeting(pCalendar calendar, float begin);

/**
 * @brief Inserts a meeting into the appointment diary.
 *
 * @param calendar Pointer to the appointment diary.
 * @param meeting  Pointer to the meeting to insert.
 *
 * @return `OK` on success,
 *         `NULL_PTR_ERROR` if a null pointer is provided.
 *         `INSERT_FAILED` if the meeting time is invalid.
 *         `OVERLAP` if the meeting overlaps with an existing meeting.
 *         `OVERFLOW` if the calendar is full and CANNOT add more meetings.
 *         `REALLOC_FAILED` if memory reallocation failed during resizing the meetings array.
 */
Status InsertMeeting(pCalendar calendar, pMeeting meeting);

/**
 * @brief Removes a meeting from the appointment diary by its start time.
 *
 * @param calendar Pointer to the appointment diary.
 * @param begin    Start time of the meeting to remove.
 *
 * @return `OK` on success.
 *         `NULL_PTR_ERROR` if a null pointer is provided.
 *         `UNDERFLOW` if the calendar is empty.
 *         `NOT_FOUND` if the meeting is not found.
 */
Status RemoveMeeting(pCalendar calendar, float begin);

/**
 * @brief Loads an appointment diary from a file.
 *
 * Loads an appointment diary from the specified file into the provided calendar.
 * If the calendar already contains meetings, they will be destroyed and replaced with the new data.
 *
 * @param calendar Pointer to the appointment diary to load into.
 * @param fileName The name of the file to load the appointment diary from.
 *
 * @return `OK` on successful loading.
 *         `NULL_PTR_ERROR` if the `calendar` pointer is `NULL`.
 *         `OPEN_FILE_FAILED` if the file cannot be opened.
 *         `CALENDAR_CREATION_FAILED` if creating a new appointment diary fails.
 *         `INVALID_PARTICIPANT_NUM` if the number of participants is invalid.
 *         `INVALID_PARTICIPANT_DATA` if participant data is invalid.
 *         `PARTICIPANTS_PARSING_FAILED` if parsing participants failed.
 *         `INVALID_MEETING_DATA` if meeting data is invalid.
 *         `MEETING_PARSING_FAILED` if parsing meeting failed.
 *         `MEETING_CREATION_FAILED` if meeting creation failed.
 *         `INVALID_ROOM_NUM` if the room number is invalid.
 *         `OVERLAP` if meetings overlap.
 *         `REALLOC_FAILED` if memory reallocation failed.
 *         `ALLOCATE_MEMORY_FAILED` if memory allocation failed.
 */
Status LoadAD(pCalendar* calendar, char *fileName);

/**
 * @brief Saves the appointment diary to a file.
 *
 * @param calendar Pointer to the appointment diary to save.
 * @param fileName The name of the file to save the appointment diary to.
 *
 * @return `OK` on success,
 *         `NULL_PTR_ERROR` if a null pointer is provided,
 *         `OPEN_FILE_FAILED` if the file cannot be opened for writing.
 */
Status StoreAD(pCalendar calendar, const char* fileName);

/**
 * @brief Prints all meetings in the appointment diary.
 *
 * @param AD Pointer to the appointment diary.
 *
 * @return `OK` on success.
 *         `NULL_PTR_ERROR` if a null pointer is provided.
 */
Status PrintAD(pCalendar AD);

/**
 * @brief Destroys the appointment diary and frees allocated memory.
 *
 * @param calendar Pointer to the appointment diary to destroy.
 */
void DestroyAD(pCalendar *calendar);

#endif /* __appointment_diary__ */
