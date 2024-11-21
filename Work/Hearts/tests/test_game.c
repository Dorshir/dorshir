#include "game.h"
#include "player.h"
#include "round.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define constants for testing */
#define MAX_PLAYERS 10
#define MAX_NAME_LENGTH 50

/* Function prototypes for test cases */
void TestCreateGame_ValidInput();
void TestCreateGame_InvalidInput();
void TestPlayGame_Normal();
void TestPlayGame_Human();
void TestDestroyGame();
void TestErrorHandling();

int main()
{
    TestCreateGame_ValidInput();
    TestCreateGame_InvalidInput();
    TestPlayGame_Normal();
    TestPlayGame_Human();
    TestDestroyGame();
    TestErrorHandling();

    printf("All tests completed.\n");
    return 0;
}

void TestCreateGame_ValidInput()
{
    size_t numHumanPlayers = 2;
    size_t numComputerPlayers = 2;
    char *playerNames[] = {"Alice", "Bob", "Computer1", "Computer2"};
    Game *game = CreateGame(numHumanPlayers, numComputerPlayers, playerNames);

    if (game == NULL)
    {
        printf("TestCreateGame_ValidInput: FAILED (Game creation returned NULL)\n");
        return;
    }

    /* Additional checks can be performed here */
    printf("TestCreateGame_ValidInput: PASSED\n");

    DestroyGame(&game);
}

void TestCreateGame_InvalidInput()
{
    Game *game;

    /* Test with zero players */
    game = CreateGame(0, 0, NULL);
    if (game == NULL)
    {
        printf("TestCreateGame_InvalidInput (zero players): PASSED\n");
    }
    else
    {
        printf("TestCreateGame_InvalidInput (zero players): FAILED\n");
        DestroyGame(&game);
    }

    /* Test with NULL player names */
    game = CreateGame(1, 1, NULL);
    if (game == NULL)
    {
        printf("TestCreateGame_InvalidInput (NULL names): PASSED\n");
    }
    else
    {
        printf("TestCreateGame_InvalidInput (NULL names): FAILED\n");
        DestroyGame(&game);
    }
}

void TestPlayGame_Normal()
{
    size_t numHumanPlayers = 0;
    size_t numComputerPlayers = 4;
    char *playerNames[] = {"Computer1", "Computer2", "Computer3", "Computer4"};

    Game *game = CreateGame(numHumanPlayers, numComputerPlayers, playerNames);
    if (game == NULL)
    {
        printf("TestPlayGame_Normal: FAILED (Game creation failed)\n");
        return;
    }

    GameResult result = PlayGame(game);
    if (result == GAME_SUCCESS)
    {
        printf("TestPlayGame_Normal: PASSED\n");
    }
    else
    {
        printf("TestPlayGame_Normal: FAILED (PlayGame returned error code %d)\n", result);
    }

    DestroyGame(&game);
}

void TestPlayGame_Human()
{
    size_t numHumanPlayers = 2;
    size_t numComputerPlayers = 2;
    char *playerNames[] = {"Computer1", "Computer2", "Human1", "Human2"};

    Game *game = CreateGame(numHumanPlayers, numComputerPlayers, playerNames);
    if (game == NULL)
    {
        printf("TestPlayGame_Normal: FAILED (Game creation failed)\n");
        return;
    }

    GameResult result = PlayGame(game);
    if (result == GAME_SUCCESS)
    {
        printf("TestPlayGame_Normal: PASSED\n");
    }
    else
    {
        printf("TestPlayGame_Normal: FAILED (PlayGame returned error code %d)\n", result);
    }

    DestroyGame(&game);
}

void TestDestroyGame()
{
    size_t numHumanPlayers = 2;
    size_t numComputerPlayers = 2;
    char *playerNames[] = {"Alice", "Bob", "Computer1", "Computer2"};
    Game *game = CreateGame(numHumanPlayers, numComputerPlayers, playerNames);

    if (game == NULL)
    {
        printf("TestDestroyGame: FAILED (Game creation failed)\n");
        return;
    }

    DestroyGame(&game);

    if (game == NULL)
    {
        printf("TestDestroyGame: PASSED\n");
    }
    else
    {
        printf("TestDestroyGame: FAILED (Game was not set to NULL)\n");
    }
}

void TestErrorHandling()
{
    GameResult result;
    Game *game = NULL;

    /* Test PlayGame with NULL game */
    result = PlayGame(game);
    if (result != GAME_UNINITIALIZED_ERROR)
    {
        printf("TestErrorHandling (PlayGame with NULL): FAILED\n");
    }
    else
    {
        printf("TestErrorHandling (PlayGame with NULL): PASSED\n");
    }

    /* Test DestroyGame with NULL pointer */
    DestroyGame(&game); // Should handle gracefully without crashing

    printf("TestErrorHandling: Completed\n");
}
