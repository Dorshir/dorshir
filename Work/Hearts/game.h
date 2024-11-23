#ifndef __GAME_H__
#define __GAME_H__

#include <stddef.h> /* size_t */

typedef struct Game Game;

typedef enum Game_Result
{
    GAME_SUCCESS,
    GAME_UNINITIALIZED_ERROR,
    PLAYER_NOT_FOUND,
    GAME_ALLOCATION_ERROR,
    GAME_INVALID_NUM_OF_PLAYERS,
    GAME_UNKNOWN_ERROR

} GameResult;

Game *CreateGame(size_t _numOfHumanPlayers, size_t _numOfComputerPlayers, char **_playerNames);

void DestroyGame(Game **_game);

GameResult PlayGame(Game *_game);

#endif /* __GAME_H__ */