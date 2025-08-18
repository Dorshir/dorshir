#include "game.h"
#include "ui.h"
#include <stdio.h>  /* fgets */
#include <string.h> /* strcspn */

#define MAX_LENGTH_NAME 30
#define NUM_OF_PLAYERS 4

int main()
{
    char name[MAX_LENGTH_NAME] = {0};
    char *playerNames[NUM_OF_PLAYERS];


    PrintMessage("Please enter a name: ");
    GetInput(name, MAX_LENGTH_NAME);    

    name[strlen(name) - 1] = '\0';
    playerNames[0] = name;
    playerNames[1] = "Alice";
    playerNames[2] = "Bob";
    playerNames[3] = "Robert";

    PrintMessage("Welcome to Hearts ");
    PrintMessage(name);
    PrintMessage("!\n");

    Game *game = CreateGame(1, 3, playerNames);
    if (game == NULL)
    {
        PrintMessage("Failed on creating a game.\n");
    }
    else if (PlayGame(game) != GAME_SUCCESS)
    {
        PrintMessage("Failed on playing the game.\n");
        DestroyGame(&game);
    }

    return 0;
}
