#ifndef __DECK_H__
#define __DECK_H__

#include <stddef.h> /* size_t */

typedef struct Deck Deck;

typedef enum Deck_Result
{
    DECK_SUCCESS,
    DECK_UNINITIALIZED_ERROR,
    DECK_EMPTY_ERROR
} DeckResult;

/**
 * @brief Create a new Deck instance.
 *
 * @param[in] _numOfDecks - Number of standard decks to include.
 *
 * @return Pointer to the newly created Deck instance, or NULL on failure.
 */
Deck *CreateDeck(size_t _numOfDecks);

/**
 * @brief Destroy a Deck instance and free its resources.
 *
 * @param[in,out] _deck - Pointer to the Deck instance to be destroyed.
 */
void DestroyDeck(Deck **_deck);

/**
 * @brief Draw a card from the deck.
 *
 * @param[in] _deck - Pointer to the Deck instance.
 * @param[out] _pValue - Pointer to store the drawn card.
 *
 * @return DECK_SUCCESS on success, or an error code on failure.
 */
DeckResult GetACard(Deck *_deck, void **_pValue);

/**
 * @brief Shuffle the deck.
 *
 * @param[in] _deck - Pointer to the Deck instance.
 *
 * @return DECK_SUCCESS on success, or an error code on failure.
 */
DeckResult ShuffleDeck(Deck *_deck);

#endif /* __DECK_H__ */
