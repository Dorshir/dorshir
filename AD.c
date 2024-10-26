/* AD.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "appointment_diary.h"

#define MAX_PARTICIPANTS 10

/* Enum for menu options */
typedef enum {
    MENU_INVALID = 0,
    MENU_CREATE_AD,
    MENU_INSERT_MEETING,
    MENU_REMOVE_MEETING,
    MENU_FIND_MEETING,
    MENU_PRINT_AD,
    MENU_LOAD_AD,
    MENU_STORE_AD,
    MENU_QUIT
} MenuOption;

/* Function to print the main menu */
void PrintMenu() {
    printf("\n--- Appointment Diary Management ---\n");
    printf("Please choose an option:\n");
    printf("1. Create Appointment Diary\n");
    printf("2. Insert Meeting\n");
    printf("3. Remove Meeting\n");
    printf("4. Find Meeting\n");
    printf("5. Print Appointment Diary\n");
    printf("6. Load Appointment Diary from File\n");
    printf("7. Store Appointment Diary to File\n");
    printf("q. Quit\n");
    printf("Your choice: ");
}

/* Function to map input string to Participant enum */
Participant GetParticipantFromInput(const char* input) {
    if (strcmp(input, "ALICE") == 0) return ALICE;
    else if (strcmp(input, "BOB") == 0) return BOB;
    else if (strcmp(input, "ROBERT") == 0) return ROBERT;
    else if (strcmp(input, "EMILY") == 0) return EMILY;
    else if (strcmp(input, "GEORGE") == 0) return GEORGE;
    else if (strcmp(input, "ANGELINA") == 0) return ANGELINA;
    else if (strcmp(input, "MATHIEW") == 0) return MATHIEW;
    else if (strcmp(input, "HARRY") == 0) return HARRY;
    else return -1; // Invalid participant
}

/* Function to convert Participant enum to string */
const char* ParticipantToString(Participant p) {
    switch (p) {
        case ALICE: return "ALICE";
        case BOB: return "BOB";
        case ROBERT: return "ROBERT";
        case EMILY: return "EMILY";
        case GEORGE: return "GEORGE";
        case ANGELINA: return "ANGELINA";
        case MATHIEW: return "MATHIEW";
        case HARRY: return "HARRY";
        default: return "UNKNOWN";
    }
}

/* Function to map input string to Room enum */
Room GetRoomFromInput(const char* input) {
    if (strcmp(input, "JERUSALEM") == 0) return JERUSALEM;
    else if (strcmp(input, "PARIS") == 0) return PARIS;
    else if (strcmp(input, "LONDON") == 0) return LONDON;
    else if (strcmp(input, "MADRID") == 0) return MADRID;
    else if (strcmp(input, "KYIV") == 0) return KYIV;
    else if (strcmp(input, "ROME") == 0) return ROME;
    else if (strcmp(input, "VIENNA") == 0) return VIENNA;
    else if (strcmp(input, "COPENHAGEN") == 0) return COPENHAGEN;
    else return -1; // Invalid room
}

/* Function to convert Room enum to string */
const char* RoomToString(Room r) {
    switch (r) {
        case JERUSALEM: return "JERUSALEM";
        case PARIS: return "PARIS";
        case LONDON: return "LONDON";
        case MADRID: return "MADRID";
        case KYIV: return "KYIV";
        case ROME: return "ROME";
        case VIENNA: return "VIENNA";
        case COPENHAGEN: return "COPENHAGEN";
        default: return "UNKNOWN";
    }
}

/* Function to print all possible participants */
void PrintAllParticipants() {
    printf("Available participants: ");
    for (int i = ALICE; i <= HARRY; i++) {
        printf("%s", ParticipantToString((Participant)i));
        if (i != HARRY) printf(", ");
    }
    printf("\n");
}

/* Function to print all possible rooms */
void PrintAllRooms() {
    printf("Available rooms: ");
    for (int i = JERUSALEM; i <= COPENHAGEN; i++) {
        printf("%s", RoomToString((Room)i));
        if (i != COPENHAGEN) printf(", ");
    }
    printf("\n");
}

/* Function to clear the input buffer */
void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Function prototypes for menu actions */
void CreateAppointmentDiary(pCalendar* calendar);
void InsertMeetingOption(pCalendar calendar);
void RemoveMeetingOption(pCalendar calendar);
void FindMeetingOption(pCalendar calendar);
void PrintAppointmentDiary(pCalendar calendar);
void LoadAppointmentDiary(pCalendar* calendar);
void StoreAppointmentDiary(pCalendar calendar);

/* Main function */
int main() {
    pCalendar calendar = NULL;
    char choice[10];
    int running = 1;

    while (running) {
        PrintMenu();
        scanf("%9s", choice);
        ClearInputBuffer();

        MenuOption option = MENU_INVALID;

        /* Map input to menu option */
        if (strcmp(choice, "q") == 0 || strcmp(choice, "Q") == 0) {
            option = MENU_QUIT;
        } else {
            int choice_num = atoi(choice);
            if (choice_num >= MENU_CREATE_AD && choice_num <= MENU_STORE_AD) {
                option = (MenuOption)choice_num;
            } else {
                option = MENU_INVALID;
            }
        }

        /* Switch-case structure for menu options */
        switch (option) {
            case MENU_CREATE_AD:
                CreateAppointmentDiary(&calendar);
                break;
            case MENU_INSERT_MEETING:
                if (calendar == NULL) {
                    printf("No appointment diary exists. Please create one first.\n");
                } else {
                    InsertMeetingOption(calendar);
                }
                break;
            case MENU_REMOVE_MEETING:
                if (calendar == NULL) {
                    printf("No appointment diary exists. Please create one first.\n");
                } else {
                    RemoveMeetingOption(calendar);
                }
                break;
            case MENU_FIND_MEETING:
                if (calendar == NULL) {
                    printf("No appointment diary exists. Please create one first.\n");
                } else {
                    FindMeetingOption(calendar);
                }
                break;
            case MENU_PRINT_AD:
                if (calendar == NULL) {
                    printf("No appointment diary exists.\n");
                } else {
                    PrintAppointmentDiary(calendar);
                }
                break;
            case MENU_LOAD_AD:
                LoadAppointmentDiary(&calendar);
                break;
            case MENU_STORE_AD:
                if (calendar == NULL) {
                    printf("No appointment diary exists.\n");
                } else {
                    StoreAppointmentDiary(calendar);
                }
                break;
            case MENU_QUIT:
                running = 0;
                printf("Exiting the program.\n");
                if (calendar != NULL) {
                    DestroyAD(&calendar);
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

/* Function implementations */

/* Create Appointment Diary */
void CreateAppointmentDiary(pCalendar* calendar) {
    int meetingsSize, blockSize;
    printf("Enter initial meetings size: ");
    scanf("%d", &meetingsSize);
    printf("Enter block size for resizing: ");
    scanf("%d", &blockSize);
    ClearInputBuffer();

    if (*calendar != NULL) {
        DestroyAD(calendar);
    }
    *calendar = CreateAD(meetingsSize, blockSize);
    if (*calendar == NULL) {
        printf("Failed to create appointment diary.\n");
    } else {
        printf("Appointment diary created successfully.\n");
    }
}

/* Insert Meeting */
void InsertMeetingOption(pCalendar calendar) {
    float begin, end;
    char roomInput[50];
    char participantsInput[256];
    int numOfParticipants = 0;
    Participant participants[MAX_PARTICIPANTS];

    printf("Enter meeting start time (e.g., 9.00): ");
    scanf("%f", &begin);
    printf("Enter meeting end time (e.g., 10.00): ");
    scanf("%f", &end);
    ClearInputBuffer();

    /* Print available rooms */
    PrintAllRooms();
    printf("Enter meeting room: ");
    fgets(roomInput, sizeof(roomInput), stdin);
    roomInput[strcspn(roomInput, "\n")] = 0; // Remove newline character

    Room room = GetRoomFromInput(roomInput);
    if (room == -1) {
        printf("Invalid room.\n");
        return;
    }

    /* Print available participants */
    PrintAllParticipants();
    printf("Enter participants separated by spaces: ");
    fgets(participantsInput, sizeof(participantsInput), stdin);
    participantsInput[strcspn(participantsInput, "\n")] = 0; // Remove newline character

    char *token = strtok(participantsInput, " ");
    while (token != NULL && numOfParticipants < MAX_PARTICIPANTS) {
        Participant p = GetParticipantFromInput(token);
        if (p == -1) {
            printf("Invalid participant: %s\n", token);
            return;
        }
        participants[numOfParticipants++] = p;
        token = strtok(NULL, " ");
    }
    if (numOfParticipants == 0) {
        printf("No valid participants entered.\n");
        return;
    }

    pMeeting meeting = CreateMeeting(begin, end, room, participants, numOfParticipants);
    if (meeting == NULL) {
        printf("Failed to create meeting.\n");
        return;
    }
    Status status = InsertMeeting(calendar, meeting);
    if (status == OK) {
        printf("Meeting inserted successfully.\n");
    } else {
        printf("Failed to insert meeting. Error code: %d\n", status);
        DestroyMeeting(&meeting);
    }
}

/* Remove Meeting */
void RemoveMeetingOption(pCalendar calendar) {
    float begin;
    printf("Enter the start time of the meeting to remove: ");
    scanf("%f", &begin);
    ClearInputBuffer();

    Status status = RemoveMeeting(calendar, begin);
    if (status == OK) {
        printf("Meeting removed successfully.\n");
    } else if (status == NOT_FOUND) {
        printf("Meeting not found.\n");
    } else {
        printf("Failed to remove meeting. Error code: %d\n", status);
    }
}

/* Find Meeting */
void FindMeetingOption(pCalendar calendar) {
    float begin;
    printf("Enter the start time of the meeting to find: ");
    scanf("%f", &begin);
    ClearInputBuffer();

    pMeeting meeting = FindMeeting(calendar, begin);
    if (meeting != NULL) {
        printf("Meeting found:\n");
        printf("Start time: %.2f, End time: %.2f\n", meeting->begin, meeting->end);
        printf("Room: %s\n", RoomToString((Room)meeting->room));
        printf("Participants: ");
        for (int i = 0; i < meeting->numOfParts; i++) {
            printf("%s", ParticipantToString(meeting->participants[i]));
            if (i != meeting->numOfParts -1) printf(", ");
        }
        printf("\n");
    } else {
        printf("Meeting not found.\n");
    }
}

/* Print Appointment Diary */
void PrintAppointmentDiary(pCalendar calendar) {
    Status status = PrintAD(calendar);
    if (status != OK) {
        printf("Failed to print appointment diary. Error code: %d\n", status);
    }
}

/* Load Appointment Diary from File */
void LoadAppointmentDiary(pCalendar* calendar) {
    char filename[256];
    printf("Enter the filename to load from: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline character

    Status status = LoadAD(calendar, filename);
    if (status == OK) {
        printf("Appointment diary loaded successfully.\n");
    } else {
        printf("Failed to load appointment diary. Error code: %d\n", status);
    }
}

/* Store Appointment Diary to File */
void StoreAppointmentDiary(pCalendar calendar) {
    char filename[256];
    printf("Enter the filename to save to: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline character

    Status status = StoreAD(calendar, filename);
    if (status == OK) {
        printf("Appointment diary saved successfully.\n");
    } else {
        printf("Failed to save appointment diary. Error code: %d\n", status);
    }
}
