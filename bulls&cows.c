#include "bulls&cows.h"

void PrintBoard(int board[])
{
    int position;
    printf("Board: [");
    for (position = 0; position < BOARD_SIZE - 1; position++)
    {
        printf("%d, ", board[position]);
    }
    printf("%d]", board[position]);
}

void MakeAGuessPlayer(int guessBoard[])
{
    int position;
    PrintBoard(guessBoard);

    for (position = 0; position < BOARD_SIZE; position++)
    {
        printf("Position[%d] is %d, overwrite it by entering a new digit or [k] to keep\n", position, guessBoard[position]);
        char input = getc(stdin);
        if (input == 'k')
        {
            continue;
        }
        else if (isdigit(input))
        {
            guessBoard[position] = input - '0';
        }
        else
        {
            printf("Invalid input!");
            position--;
        }
    }
    PrintBoard(guessBoard);
}

void InitBoard(int board[])
{
    int position;
    for (position = 0; position < BOARD_SIZE; position++)
    {
        board[position] = 0;
    }
}

void InitPlayer(Player *p)
{
    InitBoard(p->ownBoard);
    InitBoard(p->ownBoard);
    p->bulls = 0;
    p->cows = 0;
    p->myTurn = 0;
    p->MakeAGuess = MakeAGuessPlayer;
    p->name = ""; /*Should be an input name or random for the computer*/
}

void InitGame(Player *p1, Player *p2, int *winner)
{
    InitPlayer(p1);
    InitPlayer(p2);

    *winner = 0;
}

void ResetHits(Player *p)
{
    p->bulls = 0;
    p->cows = 0;
}

void PrintHits(Player *p)
{
    Printf("Player [%s] has %d bulls and %d cows by the last guess.", p->name, p->bulls, p->cows);
}

int BullOrCow(int guessBoard[], int position, int opponentBoard[], int tempBoard[])
{
    int index;
    int result = MISS;

    if (guessBoard[position] == opponentBoard[position])
    {
        result = BULL;
    }
    else
    {
        for (index = 0; index < BOARD_SIZE; index++)
        {
            if (guessBoard[position] == opponentBoard[index])
            {
                if (tempBoard[index] == 1)
                {
                    continue;
                }
                else
                {
                    tempBoard[index] = 1;
                    result = COW;
                    break;
                }
            }
        }
    }
    return result;
}

void CheckBoard(Player *owner, Player *opponent)
{
    int check;
    int position;
    int tempBoard[BOARD_SIZE] = {0};

    ResetHits(owner);

    for (position = 0; position < BOARD_SIZE; position++)
    {
        check = MISS;
        check = BullOrCow(owner->guessBoard, position, opponent->ownBoard, tempBoard);
        if (check == BULL)
        {
            owner->bulls++;
        }
        else if (check == COW)
        {
            owner->cows++;
        }
    }
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