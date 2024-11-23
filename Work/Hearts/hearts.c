#include "game.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *playerNames[] = {"Bob", "Alice", "Robert", "Emily"};
    Game *game = CreateGame(1, 3, playerNames);

    if (game == NULL)
    {
        printf("Failed on creating a game.\n");
        return 0;
    }

    GameResult playRes = PlayGame(game);

    if (playRes != GAME_SUCCESS)
    {
        printf("Failed on PlayGame.");
    }

    return 0;
}
