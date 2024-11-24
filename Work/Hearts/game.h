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

/**
 * @brief Create a new Game instance.
 *
 * @param[in] _numOfHumanPlayers - Number of human players in the game.
 * @param[in] _numOfComputerPlayers - Number of computer players in the game.
 * @param[in] _playerNames - Array of names for the players.
 *
 * @return Pointer to the newly created Game instance, or NULL on failure.
 */
Game *CreateGame(size_t _numOfHumanPlayers, size_t _numOfComputerPlayers, char **_playerNames);

/**
 * @brief Destroy a Game instance and free its resources.
 *
 * @param[in,out] _game - Pointer to the Game instance to be destroyed.
 */
void DestroyGame(Game **_game);

/**
 * @brief Play the Hearts game.
 *
 * @param[in] _game - Pointer to the Game instance.
 *
 * @return GAME_SUCCESS on success, or an error code on failure.
 */
GameResult PlayGame(Game *_game);

#endif /* __GAME_H__ */
