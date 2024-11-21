#include "round.h"
#include "player.h"
#include "card.h"
#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define constants for testing */
#define MAX_PLAYERS 4
#define MAX_NAME_LENGTH 50

/* Function prototypes for test cases */
void TestCreateRound_ValidInput();
void TestCreateRound_InvalidInput();
void TestDestroyRound();
void TestPlay_ValidInput();
void TestPlay_InvalidInput();
void TestWinnerDetermination();

int main()
{
    TestCreateRound_ValidInput();
    TestCreateRound_InvalidInput();
    TestDestroyRound();
    TestPlay_ValidInput();
    TestPlay_InvalidInput();
    TestWinnerDetermination();

    printf("All tests completed.\n");
    return 0;
}

/* Test Functions */

void TestCreateRound_ValidInput()
{
    size_t numPlayers = 4;
    Player *players[MAX_PLAYERS];
    size_t scores[MAX_PLAYERS] = {0};

    /* Create players */
    for (size_t i = 0; i < numPlayers; i++)
    {
        char playerName[MAX_NAME_LENGTH];
        snprintf(playerName, sizeof(playerName), "Player%zu", i + 1);
        players[i] = CreatePlayer(MACHINE, playerName);
        if (players[i] == NULL)
        {
            printf("TestCreateRound_ValidInput: FAILED (Player creation failed)\n");
            return;
        }
    }

    Round *round = CreateRound(players, numPlayers, scores);
    if (round != NULL)
    {
        printf("TestCreateRound_ValidInput: PASSED\n");
    }
    else
    {
        printf("TestCreateRound_ValidInput: FAILED (CreateRound returned NULL)\n");
    }

    /* Clean up */
    DestroyRound(&round);
    for (size_t i = 0; i < numPlayers; i++)
    {
        DestroyPlayer(&players[i]);
    }
}

void TestCreateRound_InvalidInput()
{
    size_t numPlayers = 4;
    Player *players[MAX_PLAYERS] = {NULL};
    size_t scores[MAX_PLAYERS] = {0};

    /* Test with NULL players array */
    Round *round = CreateRound(NULL, numPlayers, scores);
    if (round == NULL)
    {
        printf("TestCreateRound_InvalidInput (NULL players): PASSED\n");
    }
    else
    {
        printf("TestCreateRound_InvalidInput (NULL players): FAILED\n");
        DestroyRound(&round);
    }

    /* Test with incorrect number of players */
    for (size_t i = 0; i < numPlayers; i++)
    {
        char playerName[MAX_NAME_LENGTH];
        snprintf(playerName, sizeof(playerName), "Player%zu", i + 1);
        players[i] = CreatePlayer(MACHINE, playerName);
        if (players[i] == NULL)
        {
            printf("TestCreateRound_InvalidInput: FAILED (Player creation failed)\n");
            return;
        }
    }
    round = CreateRound(players, 3, scores); // Should be 4 according to your implementation
    if (round == NULL)
    {
        printf("TestCreateRound_InvalidInput (incorrect numOfPlayers): PASSED\n");
    }
    else
    {
        printf("TestCreateRound_InvalidInput (incorrect numOfPlayers): FAILED\n");
        DestroyRound(&round);
    }

    /* Clean up */
    for (size_t i = 0; i < numPlayers; i++)
    {
        DestroyPlayer(&players[i]);
    }
}

void TestDestroyRound()
{
    size_t numPlayers = 4;
    Player *players[MAX_PLAYERS];
    size_t scores[MAX_PLAYERS] = {0};

    /* Create players */
    for (size_t i = 0; i < numPlayers; i++)
    {
        char playerName[MAX_NAME_LENGTH];
        snprintf(playerName, sizeof(playerName), "Player%zu", i + 1);
        players[i] = CreatePlayer(MACHINE, playerName);
    }

    Round *round = CreateRound(players, numPlayers, scores);
    if (round == NULL)
    {
        printf("TestDestroyRound: FAILED (CreateRound failed)\n");
        for (size_t i = 0; i < numPlayers; i++)
        {
            DestroyPlayer(&players[i]);
        }
        return;
    }

    DestroyRound(&round);
    if (round == NULL)
    {
        printf("TestDestroyRound: PASSED\n");
    }
    else
    {
        printf("TestDestroyRound: FAILED (DestroyRound did not set pointer to NULL)\n");
    }

    /* Clean up */
    for (size_t i = 0; i < numPlayers; i++)
    {
        DestroyPlayer(&players[i]);
    }
}

void TestPlay_ValidInput()
{
    size_t numPlayers = 4;
    Player *players[MAX_PLAYERS];
    size_t scores[MAX_PLAYERS] = {0};
    size_t winnerIndex = 0;

    /* Create players */
    for (size_t i = 0; i < numPlayers; i++)
    {
        char playerName[MAX_NAME_LENGTH];
        snprintf(playerName, sizeof(playerName), "Player%zu", i + 1);
        players[i] = CreatePlayer(MACHINE, playerName);
        if (players[i] == NULL)
        {
            printf("TestPlay_ValidInput: FAILED (Player creation failed)\n");
            return;
        }
    }

    Round *round = CreateRound(players, numPlayers, scores);
    if (round == NULL)
    {
        printf("TestPlay_ValidInput: FAILED (CreateRound failed)\n");
        for (size_t i = 0; i < numPlayers; i++)
        {
            DestroyPlayer(&players[i]);
        }
        return;
    }

    RoundResult result = Play(round, 0, &winnerIndex);
    if (result == ROUND_SUCCESS)
    {
        printf("TestPlay_ValidInput: PASSED\n");
    }
    else
    {
        printf("TestPlay_ValidInput: FAILED (Play returned error code %d)\n", result);
    }

    /* Clean up */
    DestroyRound(&round);
    for (size_t i = 0; i < numPlayers; i++)
    {
        DestroyPlayer(&players[i]);
    }
}

void TestPlay_InvalidInput()
{
    size_t winnerIndex = 0;
    RoundResult result;

    /* Test with NULL round */
    result = Play(NULL, 0, &winnerIndex);
    if (result == ROUND_UNINITIALIZED_ERROR)
    {
        printf("TestPlay_InvalidInput (NULL round): PASSED\n");
    }
    else
    {
        printf("TestPlay_InvalidInput (NULL round): FAILED\n");
    }

    /* Create a valid round */
    size_t numPlayers = 4;
    Player *players[MAX_PLAYERS];
    size_t scores[MAX_PLAYERS] = {0};

    for (size_t i = 0; i < numPlayers; i++)
    {
        char playerName[MAX_NAME_LENGTH];
        snprintf(playerName, sizeof(playerName), "Player%zu", i + 1);
        players[i] = CreatePlayer(MACHINE, playerName);
    }
    Round *round = CreateRound(players, numPlayers, scores);

    /* Test with NULL winnerIndex */
    result = Play(round, 0, NULL);
    if (result == ROUND_UNINITIALIZED_ERROR)
    {
        printf("TestPlay_InvalidInput (NULL winnerIndex): PASSED\n");
    }
    else
    {
        printf("TestPlay_InvalidInput (NULL winnerIndex): FAILED\n");
    }

    /* Clean up */
    DestroyRound(&round);
    for (size_t i = 0; i < numPlayers; i++)
    {
        DestroyPlayer(&players[i]);
    }
}
void TestWinnerDetermination()
{
    size_t numPlayers = 4;
    Player *players[MAX_PLAYERS];
    size_t scores[MAX_PLAYERS] = {110, 80, 90, 70}; // Player 0 has exceeded END_GAME_SCORE
    size_t winnerIndex = 0; // Declare a local variable for the winner index

    /* Create players */
    for (size_t i = 0; i < numPlayers; i++)
    {
        char playerName[MAX_NAME_LENGTH];
        snprintf(playerName, sizeof(playerName), "Player%zu", i + 1);
        players[i] = CreatePlayer(MACHINE, playerName);
        if (players[i] == NULL)
        {
            printf("TestWinnerDetermination: FAILED (Player creation failed)\n");
            return;
        }
    }

    Round *round = CreateRound(players, numPlayers, scores);
    if (round == NULL)
    {
        printf("TestWinnerDetermination: FAILED (CreateRound failed)\n");
        for (size_t i = 0; i < numPlayers; i++)
        {
            DestroyPlayer(&players[i]);
        }
        return;
    }

    RoundResult result = Play(round, 0, &winnerIndex); // Pass the address of winnerIndex
    if (result == ROUND_SUCCESS && winnerIndex == 3) // Assuming Player 4 (index 3) wins with lowest score
    {
        printf("TestWinnerDetermination: PASSED\n");
    }
    else
    {
        printf("TestWinnerDetermination: FAILED (Incorrect winnerIndex: %zu)\n", winnerIndex);
    }

    /* Clean up */
    DestroyRound(&round);
    for (size_t i = 0; i < numPlayers; i++)
    {
        DestroyPlayer(&players[i]);
    }
}

