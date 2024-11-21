#include "round.h"
#include "player.h"
#include "card.h"
#include "deck.h"
#include <stdlib.h> /* malloc, free, size_t */
#include <stdint.h> /* SIZE_MAX */

#define HEARTS_NUM_OF_PLAYERS 4
#define NUM_OF_CARDS_EACH_PLAYER 13
#define NUM_OF_CARDS_PASS 3
#define NUM_OF_TRICKS 13

#define IS_FIRST_TRICK(trick) (trick == 0)
#define IS_HEART_SUIT(card) (card->m_suit == HEARTS)
#define IS_QUEEN_OF_SPADES(card) (card->m_suit == SPADES && card->m_rank == QUEEN)
#define IS_EMPTY_TABLE(rules) (rules->m_numOfCardsOnTable != 0)
#define ARE_SAME_SUIT(firstCard, secondCard) (firstCard->m_suit == secondCard->m_suit)
#define ARE_NOT_SAME_SUIT(firstCard, secondCard) (!ARE_SAME_SUIT(firstCard, secondCard))

#define GET_RELATIVE_INDEX(playerNum, rulesContext, round) ((playerNum + rulesContext->m_openingPlayerNum) % round->m_numOfPlayers)
#define NOT_FOUND -1

#define DUMMY_SCORE 10
#define END_GAME_SCORE 100

typedef enum Bool
{
    FALSE,
    TRUE
} Bool;

typedef enum RoundNum
{
    FIRST,
    SECOND,
    THIRD,
    FOURTH
} RoundNum;

struct Round
{
    Player **m_players;
    size_t m_numOfPlayers;
    size_t *m_scores;
};

typedef struct RulesContext
{
    Suit m_trickSuit;
    size_t m_trickNum;
    size_t m_openingPlayerNum;
    size_t m_numOfCardsOnTable;
    int m_hasTrickSuitCards;
} RulesContext;

typedef struct StrategyContext
{
} StrategyContext;

static RoundResult DealCards(Round *_round, Deck *_deck);
static void GetRelativeNextPos(RoundNum _roundNum, int *_relativeNext);
static void PassCardsNextPlayer(Player *_first, Card **_tempCards, Player *_second, Card **_tableCards, StrategyContext *_strategyContext);
static void PassCards(Round *_round, RoundNum _roundNum, Card **_tableCards, StrategyContext *_strategyContext);
static void UpdateScore(Round *_round, Card **_tableCards, size_t _openningPlayerNum);
static void PlayTrick(Round *_round, Card **_tableCards, StrategyContext *_strategyContext, RulesContext *_rulesContext);
static void StartRound(Round *_round, size_t _roundNum);
static size_t IsThereAWinner(Round *_round);
int IsValidCard(Card *_card, Card **_table, void *_context);

Card *GetOptCard(Card *_card, Card **_table, void *_context)
{
    
}

Card *GetOptCardPass(Card *_card, Card **_table, void *_context)
{
}

/* API Functions */

Round *CreateRound(Player **_players, size_t _numOfPlayers, size_t *_scores)
{
    Round *newRound;
    if (_players == NULL || _numOfPlayers != HEARTS_NUM_OF_PLAYERS)
    {
        return NULL;
    }

    newRound = malloc(sizeof(Round));
    if (newRound == NULL)
    {
        return NULL;
    }

    newRound->m_numOfPlayers = _numOfPlayers;
    newRound->m_players = _players;
    newRound->m_scores = _scores;

    return newRound;
}

void DestroyRound(Round **_round)
{
    if (_round != NULL && *_round != NULL)
    {
        free(*_round);
        *_round = NULL;
    }
}

RoundResult Play(Round *_round, size_t _roundNum, size_t *_winnerIndex)
{

    if (_round == NULL || _winnerIndex == NULL)
    {
        return ROUND_UNINITIALIZED_ERROR;
    }

    Deck *deck = CreateDeck(1);
    if (deck == NULL)
    {
        return ROUND_ALLOCATION_ERROR;
    }

    if (DealCards(_round, deck) != ROUND_SUCCESS)
    {
        DestroyDeck(&deck);
    }

    StartRound(_round, _roundNum);

    *_winnerIndex = IsThereAWinner(_round);

    DestroyDeck(&deck);
    return ROUND_SUCCESS;
}

/* Static Functions */

static RoundResult DealCards(Round *_round, Deck *_deck)
{
    for (size_t i = 0; i < _round->m_numOfPlayers; i++)
    {
        Player *currPlayer = _round->m_players[i];
        for (size_t j = 0; j < NUM_OF_CARDS_EACH_PLAYER; j++)
        {
            Card *card;
            if (GetACard(_deck, (void **)&card) != DECK_SUCCESS)
            {
                DestroyDeck(&_deck);
                return ROUND_DECKS_SET_FAILURE;
            }
            ReceiveCard(currPlayer, &card);
        }
    }

    return ROUND_SUCCESS;
}

static void GetRelativeNextPos(RoundNum _roundNum, int *_relativeNext)
{
    switch (_roundNum)
    {
    case FIRST:
        *_relativeNext = -1;
        break;
    case SECOND:
        *_relativeNext = 1;
        break;
    case THIRD:
        *_relativeNext = 2;
        break;
    default:
        break;
    }
}

static void PassCardsNextPlayer(Player *_first, Card **_tempCards, Player *_second, Card **_tableCards, StrategyContext *_strategyContext)
{
    for (size_t cardNum = 0; cardNum < NUM_OF_CARDS_PASS; cardNum++)
    {
        ThrowCard(_first, (void **)&_tempCards[cardNum], _tableCards, IsValidCard, GetOptCardPass, NULL, _strategyContext);
    }
    for (size_t cardNum = 0; cardNum < NUM_OF_CARDS_PASS; cardNum++)
    {
        ReceiveCard(_second, &_tempCards[cardNum]);
    }
}

static void PassCards(Round *_round, RoundNum _roundNum, Card **_tableCards, StrategyContext *_strategyContext)
{
    int relativeNext;
    if (_roundNum == FOURTH)
    {
        return;
    }

    Card **tempCards = malloc(sizeof(Card *));
    if (tempCards == NULL)
    {
        return;
    }

    GetRelativeNextPos(_roundNum, &relativeNext);

    for (size_t playerNum = 0; playerNum < _round->m_numOfPlayers; playerNum++)
    {
        size_t nextPlayerIndex = (playerNum + relativeNext) % _round->m_numOfPlayers;
        PassCardsNextPlayer(_round->m_players[playerNum], tempCards, _round->m_players[nextPlayerIndex], _tableCards, _strategyContext);
    }

    free(tempCards);
}

static size_t GetLoser(Round *_round, Card **_tableCards, size_t _openningPlayerNum, size_t *_score)
{
    Card *firstTableCard = _tableCards[0];
    size_t loserIndex = _openningPlayerNum;
    for (size_t index = 1; index < _round->m_numOfPlayers; index++)
    {
        Card *currCard = _tableCards[index];
        if (ARE_SAME_SUIT(firstTableCard, currCard) && currCard->m_rank > firstTableCard->m_rank)
        {
            loserIndex = (index + _openningPlayerNum) % _round->m_numOfPlayers;
        }
        if (IS_HEART_SUIT(currCard))
        {
            *_score += 1;
        }
        else if (IS_QUEEN_OF_SPADES(currCard))
        {
            *_score += 13;
        }
    }
    return loserIndex;
}

static void UpdateScore(Round *_round, Card **_tableCards, size_t _openningPlayerNum)
{
    size_t score = 0;
    size_t loserPlayerNum = GetLoser(_round, _tableCards, _openningPlayerNum, &score);
    _round->m_scores[loserPlayerNum] += score;
}

static DestroyTableCards(Card **_tableCards, size_t _length)
{
    for (size_t cardNum = 0; cardNum < _length; cardNum++)
    {
        if (_tableCards[cardNum] != NULL)
        {
            free(_tableCards[cardNum]);
            _tableCards[cardNum] = NULL;
        }
    }
}

static void PlayTrick(Round *_round, Card **_tableCards, StrategyContext *_strategyContext, RulesContext *_rulesContext)
{
    for (size_t playerNum = 0; playerNum < _round->m_numOfPlayers; playerNum++)
    {
        size_t relativeIndex = GET_RELATIVE_INDEX(playerNum, _rulesContext, _round);
        ThrowCard(_round->m_players[relativeIndex], (void **)&_tableCards[relativeIndex], _tableCards, IsValidCard, GetOptCard, _rulesContext, _strategyContext);
    }

    UpdateScore(_round, _tableCards, _rulesContext->m_openingPlayerNum);

    DestroyTableCards(_tableCards, _round->m_numOfPlayers);
}

static size_t WhoHasTwoClubs(Round *_round)
{
    Card twoClubs = {TWO, CLUBS};
    int isFound = NOT_FOUND;
    size_t playerNum = 0;

    while (isFound == NOT_FOUND && playerNum < _round->m_numOfPlayers)
    {
        isFound = FindCard(_round->m_players[playerNum++], &twoClubs);
    }

    return playerNum - 1;
}

static void StartRound(Round *_round, size_t _roundNum)
{
    RulesContext rulesContext = {0};
    StrategyContext strategyContext = {0};

    Card **tableCards = malloc(_round->m_numOfPlayers * sizeof(Card *));
    if (tableCards == NULL)
    {
        return;
    }

    PassCards(_round, _roundNum, tableCards, &strategyContext);

    rulesContext.m_openingPlayerNum = WhoHasTwoClubs(_round);
    rulesContext.m_trickSuit = CLUBS;

    for (size_t trickNum = 0; trickNum < NUM_OF_TRICKS; trickNum++)
    {
        PlayTrick(_round, tableCards, &strategyContext, &rulesContext);
    }

    free(tableCards);
}

static size_t IsThereAWinner(Round *_round)
{
    int isWinner = FALSE;
    size_t winnerIndex;
    size_t minScore = SIZE_MAX;
    for (size_t playerNum = 0; playerNum < _round->m_numOfPlayers; playerNum++)
    {
        if (_round->m_scores[playerNum] < minScore)
        {
            minScore = _round->m_scores[playerNum];
            winnerIndex = playerNum;
        }
        if (_round->m_scores[playerNum] >= END_GAME_SCORE)
        {
            isWinner = TRUE;
        }
    }

    return (isWinner == TRUE) ? winnerIndex : _round->m_numOfPlayers;
}

/* Rules and Strategy */

int IsValidCard(Card *_card, Card **_table, void *_context)
{
    if (_context == NULL)
    {
        return TRUE;
    }
    RulesContext *rules = (RulesContext *)(_context);

    if (IS_FIRST_TRICK(rules->m_trickNum) && (IS_HEART_SUIT(_card) || IS_QUEEN_OF_SPADES(_card)))
    {
        return FALSE;
    }
    else if (IS_EMPTY_TABLE(rules))
    {
        if (IS_HEART_SUIT(_card))
        {
            return FALSE;
        }
        rules->m_trickSuit = _card->m_suit;
    }
    else
    {
        Card *openingCard = _table[rules->m_openingPlayerNum];
        if (ARE_NOT_SAME_SUIT(_card, openingCard) && rules->m_hasTrickSuitCards)
        {
            return FALSE;
        }
    }

    ++rules->m_numOfCardsOnTable;
    return TRUE;
}
