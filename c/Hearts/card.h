#ifndef __CARD_H__
#define __CARD_H__

#define DECK_SIZE (NUMBER_OF_RANKS * NUMBER_OF_SUITS)

/* cards suites symbols */
#define HEART "\xE2\x99\xA5"
#define SPADE "\xE2\x99\xA0"
#define DIAMOND "\xE2\x99\xA6"
#define CLUB "\xE2\x99\xA3"
/* colors */
#define NORMAL "\x1B[0m"
#define BLACK "\x1b[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define GRAY "\x1b[90m"

/* cards suites symbols */
#define HEART_SYMB "\xE2\x99\xA5"
#define SPADE_SYMB "\xE2\x99\xA0"
#define DIAMOND_SYMB "\xE2\x99\xA6"
#define CLUB_SYMB "\xE2\x99\xA3"

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