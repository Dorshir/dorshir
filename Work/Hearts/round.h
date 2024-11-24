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

/**
 * @brief Create a new Round instance.
 *
 * @param[in] _players - Array of pointers to Player instances participating in the round.
 * @param[in] _numOfPlayers - Number of players in the game.
 * @param[in] _scores - Array of current scores for each player.
 *
 * @return Pointer to the newly created Round instance, or NULL on failure.
 */
Round *CreateRound(Player **_players, size_t _numOfPlayers, size_t *_scores);

/**
 * @brief Play a round of the Hearts game.
 *
 * @param[in] _round - Pointer to the Round instance.
 * @param[in] _roundNum - The current round number.
 *
 * @return ROUND_SUCCESS on success, or an error code on failure.
 */
RoundResult Play(Round *_round, size_t _roundNum);

/**
 * @brief Check if there is a winner after the round.
 *
 * @param[in] _round - Pointer to the Round instance.
 * @param[out] _numWinners - Pointer to store the number of winners found.
 *
 * @return Array of indices representing the winners, or NULL if no winner.
 */
size_t *IsThereAWinner(Round *_round, size_t *_numWinners);

/**
 * @brief Print the current scores of all players.
 *
 * @param[in] _round - Pointer to the Round instance.
 */
void PrintScores(Round *_round);

/**
 * @brief Destroy a Round instance and free its resources.
 *
 * @param[in,out] _round - Pointer to the Round instance to be destroyed.
 */
void DestroyRound(Round **_round);

#endif /* __ROUND_H__ */
