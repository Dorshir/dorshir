#include "appointment_diary.c"

int main()
{
    pCalendar calendar;
    pMeeting meeting;
    int result;
    int initialSize = 2;
    int blockSize = 2;

    /* ------------------------------------------
       Test Case 1:
       Insertion When Number of Meetings Equals Size (Testing Reallocation)
    -------------------------------------------- */

    printf("Test Case 1: Insertion When Number of Meetings Equals Size (Testing Reallocation)\n");

    calendar = CreateAD(initialSize, blockSize);
    if (calendar == NULL)
    {
        printf("Failed to create appointment diary.\n");
        return 1;
    }

    /* Insert meetings to fill up the calendar */
    InsertMeeting(calendar, CreateMeeting(9.00, 10.00, 101));
    InsertMeeting(calendar, CreateMeeting(10.30, 11.30, 102));

    /* Insert another meeting to trigger reallocation */
    printf("Inserting meeting to trigger reallocation...\n");
    meeting = CreateMeeting(12.00, 13.00, 103);
    result = InsertMeeting(calendar, meeting);
    if (result == REALLOC_FAILED)
    {
        printf("Reallocation failed.\n");
    }
    else if (result != OK)
    {
        printf("Failed to insert meeting.\n");
    }
    else
    {
        printf("Meeting inserted successfully after reallocation.\n");
    }

    /* Print the calendar */
    printf("Calendar after insertion:\n");
    PrintAD(calendar);

    printf("Destroying calendar.\n");
    DestroyAD(&calendar);

    printf("\n");

    /* ------------------------------------------
       Test Case 2:
       Insertion When Block Size is 0 (Should Fail When Calendar is Full)
    -------------------------------------------- */

    printf("Test Case 2: Insertion When Block Size is 0 (Should Fail When Calendar is Full)\n");

    initialSize = 2;
    blockSize = 0;

    calendar = CreateAD(initialSize, blockSize);
    if (calendar == NULL)
    {
        printf("Failed to create appointment diary.\n");
        return 1;
    }

    /* Insert meetings to fill up the calendar */
    InsertMeeting(calendar, CreateMeeting(9.00, 10.00, 101));
    InsertMeeting(calendar, CreateMeeting(10.30, 11.30, 102));

    /* Attempt to insert another meeting, which should fail */
    printf("Attempting to insert another meeting...\n");
    meeting = CreateMeeting(12.00, 13.00, 103);
    result = InsertMeeting(calendar, meeting);
    if (result == OVERFLOW)
    {
        printf("Insertion failed due to overflow (block size is 0).\n");
        free(meeting);
    }
    else if (result != OK)
    {
        printf("Failed to insert meeting.\n");
    }
    else
    {
        printf("Meeting inserted successfully (unexpected).\n");
    }

    /* Print the calendar */
    printf("Calendar after insertion attempt:\n");
    PrintAD(calendar);

    printf("Destroying calendar.\n");
    DestroyAD(&calendar);

    printf("\n");

    /* ------------------------------------------
       Test Case 3:
       Removing Meetings When Begin Time Exists and When It Does Not Exist
    -------------------------------------------- */

    printf("Test Case 3: Removing Meetings When Begin Time Exists and When It Does Not Exist\n");

    calendar = CreateAD(5, 2);
    if (calendar == NULL)
    {
        printf("Failed to create appointment diary.\n");
        return 1;
    }

    /* Insert some meetings */
    InsertMeeting(calendar, CreateMeeting(9.00, 10.00, 101));
    InsertMeeting(calendar, CreateMeeting(11.00, 12.00, 102));
    InsertMeeting(calendar, CreateMeeting(14.00, 15.00, 103));

    /* Remove a meeting that exists */
    printf("Attempting to remove meeting starting at 11.00...\n");
    result = RemoveMeeting(calendar, 11.00);
    if (result == OK)
    {
        printf("Meeting removed successfully.\n");
    }
    else
    {
        printf("Failed to remove meeting.\n");
    }

    /* Print the calendar */
    printf("Calendar after removing 11.00:\n");
    PrintAD(calendar);

    /* Attempt to remove a meeting that does not exist */
    printf("Attempting to remove meeting starting at 13.00...\n");
    result = RemoveMeeting(calendar, 13.00);
    if (result == NOT_FOUND)
    {
        printf("Meeting not found. Cannot remove.\n");
    }
    else if (result != OK)
    {
        printf("Failed to remove meeting.\n");
    }
    else
    {
        printf("Meeting removed unexpectedly.\n");
    }

    /* Print the calendar */
    printf("Calendar after removal attempts:\n");
    PrintAD(calendar);

    DestroyAD(&calendar);

    printf("\n");

    /* ------------------------------------------
       Test Case 4:
       Testing Overlap Detection
    -------------------------------------------- */

    printf("Test Case 4: Testing Overlap Detection\n");

    calendar = CreateAD(5, 2);
    if (calendar == NULL)
    {
        printf("Failed to create appointment diary.\n");
        return 1;
    }

    /* Insert non-overlapping meetings */
    InsertMeeting(calendar, CreateMeeting(9.00, 10.00, 101));
    InsertMeeting(calendar, CreateMeeting(11.00, 12.00, 102));

    /* Attempt to insert an overlapping meeting */
    printf("Attempting to insert a meeting from 9.30 to 10.30 (should overlap)...\n");
    meeting = CreateMeeting(9.30, 10.30, 103);
    result = InsertMeeting(calendar, meeting);
    if (result == OVERLAP)
    {
        printf("Overlap detected. Meeting not inserted.\n");
        free(meeting);
    }
    else if (result != OK)
    {
        printf("Failed to insert meeting.\n");
    }
    else
    {
        printf("Meeting inserted successfully (unexpected).\n");
    }

    /* Attempt to insert a non-overlapping meeting */
    printf("Attempting to insert a meeting from 10.00 to 11.00 (should not overlap)...\n");
    meeting = CreateMeeting(10.00, 11.00, 104);
    result = InsertMeeting(calendar, meeting);
    if (result == OVERLAP)
    {
        printf("Overlap detected. Meeting not inserted.\n");
        free(meeting);
    }
    else if (result != OK)
    {
        printf("Failed to insert meeting.\n");
    }
    else
    {
        printf("Meeting inserted successfully.\n");
    }

    /* Print the calendar */
    printf("Calendar after overlap tests:\n");
    PrintAD(calendar);

    DestroyAD(&calendar);

    return 0;
}
