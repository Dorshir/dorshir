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

Deck *CreateDeck(size_t _numOfDecks);
void DestroyDeck(Deck **_deck);

DeckResult GetACard(Deck *_deck, void **_pValue);

#endif /* __DECK_H__ */