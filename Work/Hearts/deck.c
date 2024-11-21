#include "deck.h"
#include "card.h"
#include "genvec.h"
#include <stdlib.h> /* malloc, free, size_t */

struct Deck
{
    Vector *m_cards;
    size_t m_numOfCards;
};

static void InitCards(Deck *_deck, size_t _numOfDecks);
static Deck *AllocateMemoryDeck(size_t _numOfDecks, size_t _numOfCards);

/* API Functions */

Deck *CreateDeck(size_t _numOfDecks)
{
    Deck *newDeck;
    size_t numOfCards;
    if (_numOfDecks == 0)
    {
        return NULL;
    }
    numOfCards = DECK_SIZE * _numOfDecks;

    newDeck = AllocateMemoryDeck(_numOfDecks, numOfCards);
    if (newDeck == NULL)
    {
        return NULL;
    }

    InitCards(newDeck, _numOfDecks);

    newDeck->m_numOfCards = numOfCards;


    return newDeck;
}

DeckResult GetACard(Deck *_deck, void **_pValue)
{
    DeckResult res;
    if (_deck == NULL || _deck->m_numOfCards == 0 || _pValue == NULL)
    {
        res = DECK_UNINITIALIZED_ERROR;
    }
    else
    {
        res = VectorRemove(_deck->m_cards, _pValue);
        if (res == DECK_SUCCESS)
        {
            --_deck->m_numOfCards;
        }
    }
    return res;
}

void DestroyDeck(Deck **_deck)
{
    if (_deck != NULL && *_deck != NULL)
    {
        VectorDestroy(&((*_deck)->m_cards), free);
        free(*_deck);
        *_deck = NULL;
    }
}

/* Static Functions */

static void InitCards(Deck *_deck, size_t _numOfDecks)
{
    Card *card;
    size_t index = 0;
    for (size_t deckNum = 0; deckNum < _numOfDecks; deckNum++)
    {
        for (size_t suit = HEARTS; suit < NUMBER_OF_SUITS; suit++)
        {
            for (size_t rank = TWO; rank < NUMBER_OF_RANKS; rank++)
            {
                VectorGet(_deck->m_cards, index, (void **)&card);
                card->m_rank = rank;
                card->m_suit = suit;
                ++index;
            }
        }
    }
}

static Deck *AllocateMemoryDeck(size_t _numOfDecks, size_t _numOfCards)
{
    Deck *newDeck = malloc(sizeof(Deck));
    if (newDeck == NULL)
    {
        return NULL;
    }

    newDeck->m_cards = VectorCreate(_numOfCards, DECK_SIZE);
    if (newDeck->m_cards == NULL)
    {
        free(newDeck);
        return NULL;
    }

    for (size_t i = 0; i < _numOfCards; i++)
    {
        Card *newCard = malloc(sizeof(Card));
        if (newCard == NULL)
        {
            DestroyDeck(&newDeck);
            return NULL;
        }
        VectorAppend(newDeck->m_cards, newCard);
    }

    return newDeck;
}