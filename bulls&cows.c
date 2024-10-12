#include "bulls&cows.h"

void InitGame()
{

}

void GameManager(Player *p)
{
    int GameOn = TRUE;
    Player computer;

    /*
    gamemanager
    input
    init gameplay
    make a play
    check board and update
    */








    printf("[q] for quit\n");
    while (GameOn)
    {
        char input = getc(stdin);

        if (input == 'q')
        {
            GameOn = FALSE;
        }
    }
}