#include <stdio.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

typedef struct Player
{
    int board[4];
    int myTurn;
    void (*changeBoard)(struct Player *);
    int bulls;
    int cows;
} Player;

void GameManager(Player* p);

