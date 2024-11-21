#ifndef __ROUND_H__
#define __ROUND_H__
#include "player.h"
#include <stddef.h> /* size_t */

typedef struct Round Round;

typedef enum Round_Result
{
    ROUND_NOT_FOUND = -1,
    ROUND_SUCCESS,
    ROUND_ALLOCATION_ERROR,
    ROUND_INVALID_NUM_OF_PLAYERS,
    ROUND_UNINITIALIZED_PLAYERS,
    ROUND_UNINITIALIZED_ERROR,
    ROUND_DECKS_SET_FAILURE,
    ROUND_PLAY_TRICK_FAILED,
    ROUND_PASS_CARDS_FAILED

} RoundResult;

Round *CreateRound(Player **_players, size_t _numOfPlayers, size_t *_scores);

RoundResult Play(Round *_round, size_t _roundNum, size_t *_winnerIndex);

void DestroyRound(Round **_round);

#endif /* __ROUND_H__ */