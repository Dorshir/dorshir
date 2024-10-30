#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 4

#define MISS 0
#define BULL 1
#define COW 2

typedef struct Player
{
    int ownBoard[BOARD_SIZE];
    int guessBoard[BOARD_SIZE];
    Bool isMyTurn;
    void (*MakeAGuess)(int guessBoard[]);
    int bulls;
    int cows;
    char* name;
    Bool isComputer;
} Player;

typedef struct Game
{
    Player* p1;
    Player* p2;
    int winner;
} Game;

typedef enum Bool{
    FALSE,
    TRUE
}Bool;

typedef enum Vs
{
    PLAYER,
    COMPUTER
} Vs;


void GameManager(Game* game, Player *p);
