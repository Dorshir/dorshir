#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 4

#define FALSE 0
#define TRUE 1

#define MISS 0
#define BULL 1
#define COW 2

typedef struct Player
{
    int ownBoard[BOARD_SIZE];
    int guessBoard[BOARD_SIZE];
    int myTurn;
    void (*MakeAGuess)(int guessBoard[]);
    int bulls;
    int cows;
    char* name;
} Player;

void GameManager(Player *p);
