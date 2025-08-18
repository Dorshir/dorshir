#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "card.h"

typedef struct Player Player;

/**
 * @brief Function pointer type for rule checking functions.
 *
 * @param[in] _card - The card to validate.
 * @param[in] _table - Array representing the current state of the table.
 * @param[in] _context - Additional context needed for rule validation.
 *
 * @return Non-zero if the card is valid according to the rules, zero otherwise.
 */
typedef int (*RulesFunction)(Card *_card, Card **_table, void *_context);

/**
 * @brief Function pointer type for strategy functions.
 *
 * @param[in] _cards - Array of available cards to choose from.
 * @param[in] _table - Array representing the current state of the table.
 * @param[in] _context - Additional context needed for strategy execution.
 *
 * @return Pointer to the selected Card.
 */
typedef Card *(*StrategyFunction)(Card **_cards, Card **_table, void *_context);

/**
 * @brief Function pointer type for printing a card.
 *
 * @param[in] _card - The card to print.
 */
typedef void (*PrintCardFunc)(Card *_card);


typedef enum Player_Result
{
    PLAYER_SUCCESS,
    PLAYER_ALLOCATION_ERROR,
    PLAYER_UNINITIALIZED,
    PLAYER_REC_CRAD_FAILED,
    PLAYER_EMPTY_CARDS,
    PLAYER_THROW_CARD_FAILED,
    PLAYER_CARD_NOT_FOUND

} PlayerResult;

typedef enum PlayerType
{
    MACHINE,
    HUMAN
} PlayerType;

/**
 * @brief Create a new Player instance.
 *
 * @param[in] _type - The type of the player (MACHINE or HUMAN).
 * @param[in] _name - The name of the player.
 *
 * @return Pointer to the newly created Player instance, or NULL on failure.
 */
Player *CreatePlayer(PlayerType _type, char *_name);

/**
 * @brief Destroy a Player instance and free its resources.
 *
 * @param[in,out] _player - Pointer to the Player instance to be destroyed.
 */
void DestroyPlayer(Player **_player);

/**
 * @brief Add a card to the player's hand.
 *
 * @param[in] _player - Pointer to the Player instance.
 * @param[in] _card - Pointer to the Card to add.
 *
 * @return PLAYER_SUCCESS on success, or an error code on failure.
 */
PlayerResult ReceiveCard(Player *_player, Card **_card);

/**
 * @brief Player throws a card onto the table.
 *
 * @param[in] _player - Pointer to the Player instance.
 * @param[out] _pValue - Pointer to store the card thrown.
 * @param[in,out] _table - Array representing the current state of the table.
 * @param[in] _printFunc - Function to print a card.
 * @param[in] _rulesFunc - Function to validate the move according to the rules.
 * @param[in] _strategyFunc - Function defining the player's strategy.
 * @param[in] _rulesContext - Context for the rules function.
 * @param[in] _strategyContext - Context for the strategy function.
 *
 * @return PLAYER_SUCCESS on success, or an error code on failure.
 */
PlayerResult ThrowCard(Player *_player, Card **_pValue, Card **_table,
                       PrintCardFunc _printFunc, RulesFunction _rulesFunc,
                       StrategyFunction _strategyFunc, void *_rulesContext,
                       void *_strategyContext);

/**
 * @brief Find a specific card in the player's hand.
 *
 * @param[in] _player - Pointer to the Player instance.
 * @param[in] _desiredCard - Pointer to the Card to find.
 *
 * @return Index of the card in the player's hand, or PLAYER_CARD_NOT_FOUND.
 */
int FindCard(Player *_player, Card *_desiredCard);

/**
 * @brief Get the name of the player.
 *
 * @param[in] _player - Pointer to the Player instance.
 *
 * @return Pointer to the player's name string.
 */
const char *PlayerGetName(const Player *_player);

/**
 * @brief Check if the player has any cards of a given suit.
 *
 * @param[in] _player - Pointer to the Player instance.
 * @param[in] _suit - The suit to check for.
 *
 * @return Non-zero if the player has cards of the given suit, zero otherwise.
 */
int PlayerHasSuit(Player *_player, Suit _suit);

#endif /* __PLAYER_H__ */
