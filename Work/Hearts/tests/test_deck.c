#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "deck.h"
#include "genvec.h"

/* Function to print card details */
void PrintCard(const Card *card)
{
    const char *suitNames[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const char *rankNames[] = {
        [TWO] = "Two",
        [THREE] = "Three",
        [FOUR] = "Four",
        [FIVE] = "Five",
        [SIX] = "Six",
        [SEVEN] = "Seven",
        [EIGHT] = "Eight",
        [NINE] = "Nine",
        [TEN] = "Ten",
        [JACK] = "Jack",
        [QUEEN] = "Queen",
        [KING] = "King",
        [ACE] = "Ace"
    };

    printf("Card: %s of %s\n", rankNames[card->m_rank], suitNames[card->m_suit]);
}

int main()
{
    size_t numOfDecks = 1; /* Change this to test multiple decks */
    Deck *deck = CreateDeck(numOfDecks);
    if (!deck)
    {
        printf("Failed to create deck.\n");
        return EXIT_FAILURE;
    }

    printf("Deck created successfully with %zu deck(s).\n", numOfDecks);

    void *cardPtr;
    DeckResult res;
    size_t cardCount = 0;

    /* Draw all cards from the deck */
    while ((res = GetACard(deck, &cardPtr)) == DECK_SUCCESS)
    {
        Card *drawnCard = (Card *)cardPtr;
        PrintCard(drawnCard);
        free(drawnCard); /* Free the card after use */
        cardCount++;
    }

    if (res == DECK_EMPTY_ERROR || res == DECK_UNINITIALIZED_ERROR)
    {
        printf("All cards have been drawn. Total cards drawn: %zu\n", cardCount);
    }
    else
    {
        printf("An error occurred while drawing cards.\n");
    }

    DestroyDeck(&deck);
    if (!deck)
    {
        printf("Deck destroyed successfully.\n");
    }
    else
    {
        printf("Failed to destroy deck.\n");
    }

    return EXIT_SUCCESS;
}
