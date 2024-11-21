#ifndef __CARD_H__
#define __CARD_H__

#define DECK_SIZE (NUMBER_OF_RANKS * NUMBER_OF_SUITS)

typedef enum Rank
{
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
    NUMBER_OF_RANKS
} Rank;

typedef enum Suit
{
    HEARTS,
    DIAMONDS,
    SPADES,
    CLUBS,
    NUMBER_OF_SUITS
} Suit;

typedef struct Card
{
    Rank m_rank;
    Suit m_suit;
} Card;

#endif /* __CARD_H__ */