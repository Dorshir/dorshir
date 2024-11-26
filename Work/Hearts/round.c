#include "round.h"
#include "player.h"
#include "card.h"
#include "deck.h"
#include "ui.h"
#include <stdlib.h> /* malloc, free, size_t */
#include <stdint.h> /* SIZE_MAX */
#include <string.h> /* strcat */
#include <stdio.h>  /* sprintf */

#define HEARTS_NUM_OF_PLAYERS 4
#define NUM_OF_CARDS_EACH_PLAYER 13
#define NUM_OF_CARDS_PASS 3
#define NUM_OF_TRICKS 13
#define END_GAME_SCORE 10
#define MAX_SYMBOL_SIZE 10
#define MAX_RANK_SIZE 10
#define MAX_MESSAGE_SIZE 30
#define ROUND_NOT_FOUND -1
#define NUM_OF_DECKS 1

#define IS_FIRST_TRICK(trick) (trick == 0)
#define IS_HEART_SUIT(card) (card->m_suit == HEARTS)
#define IS_QUEEN_OF_SPADES(card) (card->m_suit == SPADES && card->m_rank == QUEEN)
#define IS_EMPTY_TABLE(rules) (rules->m_numOfCardsOnTable == 0)
#define ARE_SAME_SUIT(firstCard, secondCard) (firstCard->m_suit == secondCard->m_suit)
#define GET_RELATIVE_INDEX(playerNum, rulesContext, round) ((rulesContext->m_openingPlayerNum + playerNum) % round->m_numOfPlayers)
#define HEARTS_BROKEN(rules) (rules->m_heartsHasBeenBroken == 1)
#define NEEDS_TO_BREAK(tableCards, playerNum) (tableCards[playerNum] != NULL && tableCards[playerNum]->m_suit == HEARTS)
#define IS_TWO_OF_CLUBS(card) (card->m_suit == CLUBS && card->m_rank == TWO)

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
    Bool m_heartsHasBeenBroken;
    Bool m_hasTrickSuitCards;
} RulesContext;

typedef struct StrategyContext
{
    size_t m_trickNum;
    Bool m_heartsHasBeenBroken;
} StrategyContext;

static RoundResult DealCards(Round *_round, Deck *_deck);
static void GetRelativeNextPos(RoundNum _roundNum, int *_relativeNext);
static RoundResult PassCards(Round *_round, RoundNum _roundNum, Card **_tableCards, StrategyContext *_strategyContext);
static size_t UpdateScore(Round *_round, Card **_tableCards, RulesContext *_rulesContext);
static RoundResult PlayTrick(Round *_round, Card **_tableCards, StrategyContext *_strategyContext, RulesContext *_rulesContext);
static RoundResult StartRound(Round *_round, RoundNum _roundNum);
size_t *IsThereAWinner(Round *_round, size_t *_numWinners);
static void DestroyAlreadyGivenCards(Round *_round, size_t _lastPlayerIndex);
static void DestroyTableCards(Card **_tableCards, size_t _length);
static size_t WhoHasTwoClubs(Round *_round);
int IsValidCard(Card *_card, Card **_table, void *_context);
static void PrintCard(Card *_card);
static RoundResult PassToAnotherPlayer(Round *_round, Card ***_cardsToPass, size_t _relativeNext);
static size_t GetLoser(Round *_round, Card **_tableCards, size_t _openingPlayerNum, size_t *_score, RulesContext *_rulesContext);
static size_t CountSuitCards(Card **_cards, Suit _suit);
static void SortCardsDescending(Card **_cards, size_t _numCards);
static size_t GetNumOfCards(Card **_cards);
static Card *GetLowestHigherCard(Card **_cards, size_t _numCards, Card *_refCard);
static Card *GetHighestCardOfSuit(Card **_cards, size_t _numCards, Suit _suit);
static Card *GetHighestCard(Card **_cards, size_t _numCards);
static Card *GetLowestCard(Card **_cards, size_t _numCards);
static Card *GetLowestCardOfSuit(Card **_cards, size_t _numCards, Suit _suit);
Card *GetOptCard(Card **_cards, Card **_table, void *_context);
Card *GetOptCardPass(Card **_cards, Card **_table, void *_context);

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

RoundResult Play(Round *_round, size_t _roundNum)
{
    RoundResult status = ROUND_SUCCESS;
    if (_round == NULL)
    {
        return ROUND_UNINITIALIZED_ERROR;
    }

    Deck *deck = CreateDeck(NUM_OF_DECKS);
    if (deck == NULL)
    {
        return ROUND_ALLOCATION_ERROR;
    }

    if (ShuffleDeck(deck) != DECK_SUCCESS)
    {
        DestroyDeck(&deck);
        return status;
    }

    status = DealCards(_round, deck);
    if (status != ROUND_SUCCESS)
    {
        DestroyDeck(&deck);
        return status;
    }

    status = StartRound(_round, _roundNum);
    if (status != ROUND_SUCCESS)
    {
        DestroyDeck(&deck);
        return status;
    }

    DestroyDeck(&deck);
    return ROUND_SUCCESS;
}

size_t *IsThereAWinner(Round *_round, size_t *_numWinners)
{
    if (_round == NULL || _numWinners == NULL)
    {
        return NULL;
    }

    *_numWinners = 0;

    Bool isGameOver = FALSE;
    for (size_t i = 0; i < _round->m_numOfPlayers; i++)
    {
        if (_round->m_scores[i] >= END_GAME_SCORE)
        {
            isGameOver = TRUE;
            break;
        }
    }

    if (!isGameOver)
    {
        return NULL;
    }

    size_t minScore = SIZE_MAX;
    for (size_t i = 0; i < _round->m_numOfPlayers; i++)
    {
        if (_round->m_scores[i] < minScore)
        {
            minScore = _round->m_scores[i];
        }
    }

    size_t *winnerIndices = malloc(_round->m_numOfPlayers * sizeof(size_t));
    if (winnerIndices == NULL)
    {
        return NULL;
    }

    size_t winnerCount = 0;
    for (size_t i = 0; i < _round->m_numOfPlayers; i++)
    {
        if (_round->m_scores[i] == minScore)
        {

            winnerIndices[winnerCount++] = i;
        }
    }
    *_numWinners = winnerCount;
    return winnerIndices;
}

void PrintScores(Round *_round)
{
    if (_round == NULL)
    {
        return;
    }

    PrintMessage("Scores:\n");
    for (size_t index = 0; index < _round->m_numOfPlayers; index++)
    {
        char message[MAX_MESSAGE_SIZE];
        sprintf(message, "%s : %zu\n", PlayerGetName(_round->m_players[index]), _round->m_scores[index]);
        PrintMessage(message);
    }
    PrintMessage("\n");
}

/* Static Functions */

static void DestroyAlreadyGivenCards(Round *_round, size_t _lastPlayerIndex)
{
    for (size_t index = 0; index < _lastPlayerIndex; index++)
    {
        Player *player = _round->m_players[index];
        DestroyPlayer(&player);
    }
}

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
                DestroyAlreadyGivenCards(_round, i);
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
    case FOURTH:
        *_relativeNext = 0;
        break;
    default:
        break;
    }
}

static RoundResult InitCardsToPass(Card ***_cardsToPass, size_t _numOfPlayers)
{
    for (size_t playerNum = 0; playerNum < _numOfPlayers; playerNum++)
    {
        _cardsToPass[playerNum] = malloc(NUM_OF_CARDS_PASS * sizeof(Card *));
        if (_cardsToPass[playerNum] == NULL)
        {
            /* free previouse  */
            for (size_t i = 0; i < playerNum; i++)
            {
                free(_cardsToPass[i]);
            }
            free(_cardsToPass);
            return ROUND_ALLOCATION_ERROR;
        }
    }
    return ROUND_SUCCESS;
}

static RoundResult PassCards(Round *_round, RoundNum _roundNum, Card **_tableCards, StrategyContext *_strategyContext)
{
    int relativeNext;
    RoundResult status;
    if (_roundNum == FOURTH)
    {
        return ROUND_SUCCESS;
    }

    PrintMessage("Choose 3 cards to pass another player, before the tricks start on\n");

    Card ***cardsToPass = malloc(_round->m_numOfPlayers * sizeof(Card **));
    if (cardsToPass == NULL)
    {
        return ROUND_ALLOCATION_ERROR;
    }

    if (InitCardsToPass(cardsToPass, _round->m_numOfPlayers) != ROUND_SUCCESS)
    {
        return ROUND_ALLOCATION_ERROR;
    }

    status = GetChosenCards(_round, cardsToPass, _tableCards, _strategyContext);
    if (status != ROUND_SUCCESS)
    {
        return status;
    }

    GetRelativeNextPos(_roundNum, &relativeNext);

    status = PassToAnotherPlayer(_round, cardsToPass, relativeNext);
    if (status != ROUND_SUCCESS)
    {
        return status;
    }

    DestroyCardsToPass(&cardsToPass, _round->m_numOfPlayers);

    return ROUND_SUCCESS;
}

static RoundResult GetChosenCards(Round *_round, Card ***_cardsToPass, Card **_tableCards, StrategyContext *_strategyContext)
{
    for (size_t playerNum = 0; playerNum < _round->m_numOfPlayers; playerNum++)
    {
        for (size_t cardNum = 0; cardNum < NUM_OF_CARDS_PASS; cardNum++)
        {
            if (ThrowCard(_round->m_players[playerNum], &_cardsToPass[playerNum][cardNum], _tableCards, PrintCard, IsValidCard, GetOptCardPass, NULL, _strategyContext) != PLAYER_SUCCESS)
            {
                for (size_t i = 0; i <= playerNum; i++)
                {
                    free(_cardsToPass[i]);
                }
                free(_cardsToPass);
                return ROUND_PASS_CARDS_FAILED;
            }
        }
    }
    return ROUND_SUCCESS;
}

static void DestroyCardsToPass(Card ****_cardsToPass, size_t _numOfPlayers)
{
    for (size_t playerNum = 0; playerNum < _numOfPlayers; playerNum++)
    {
        free(*(_cardsToPass[playerNum]));
    }
    free(*_cardsToPass);
}

static RoundResult PassToAnotherPlayer(Round *_round, Card ***_cardsToPass, size_t _relativeNext)
{
    for (size_t playerNum = 0; playerNum < _round->m_numOfPlayers; playerNum++)
    {
        size_t nextPlayerIndex = (_round->m_numOfPlayers + playerNum + _relativeNext) % _round->m_numOfPlayers;
        for (size_t cardNum = 0; cardNum < NUM_OF_CARDS_PASS; cardNum++)
        {
            PlayerResult res = ReceiveCard(_round->m_players[nextPlayerIndex], &_cardsToPass[playerNum][cardNum]);
            if (res != PLAYER_SUCCESS)
            {
                return StatusConverter(res);
            }
        }
    }
    return ROUND_SUCCESS;
}

static RoundResult StatusConverter(PlayerResult _status)
{
    RoundResult result;
    switch (_status)
    {
    case PLAYER_UNINITIALIZED:
        result = ROUND_UNINITIALIZED_ERROR;
        break;
    case PLAYER_REC_CRAD_FAILED:
        result = ROUND_REC_CARD_FAILURE;
        break;
    default:
        break;
    }
    return result;
}

static size_t GetLoser(Round *_round, Card **_tableCards, size_t _openingPlayerNum, size_t *_score, RulesContext *_rulesContext)
{
    Card *highestCard = _tableCards[0];
    size_t highestPlayerNum = 0;

    for (size_t i = 1; i < _round->m_numOfPlayers; i++)
    {
        Card *currCard = _tableCards[i];
        if (_rulesContext->m_trickSuit == currCard->m_suit && currCard->m_rank > highestCard->m_rank)
        {
            highestCard = currCard;
            highestPlayerNum = i;
        }
    }

    size_t loserIndex = GET_RELATIVE_INDEX(highestPlayerNum, _rulesContext, _round);

    for (size_t i = 0; i < _round->m_numOfPlayers; i++)
    {
        Card *currCard = _tableCards[i];
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

static size_t UpdateScore(Round *_round, Card **_tableCards, RulesContext *_rulesContext)
{
    size_t score = 0;
    size_t loserPlayerNum = GetLoser(_round, _tableCards, _rulesContext->m_openingPlayerNum, &score, _rulesContext);
    _round->m_scores[loserPlayerNum] += score;
    return loserPlayerNum;
}

static void DestroyTableCards(Card **_tableCards, size_t _length)
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

static void GetSymb(char *_symb, Suit _cardSuit)
{
    _symb[0] = '\0';
    switch (_cardSuit)
    {
    case HEARTS:
        strcat(_symb, HEART_SYMB);
        break;
    case SPADES:
        strcat(_symb, SPADE_SYMB);
        break;
    case DIAMONDS:
        strcat(_symb, DIAMOND_SYMB);
        break;
    case CLUBS:
        strcat(_symb, CLUB_SYMB);
        break;
    default:
        break;
    }
}

static void PrintCard(Card *_card)
{
    if (_card == NULL)
    {
        return;
    }
    char symb[MAX_SYMBOL_SIZE] = "";
    char rank[MAX_RANK_SIZE] = "";
    char message[MAX_MESSAGE_SIZE] = "";

    GetSymb(symb, _card->m_suit);

    const char *rankStr = NULL;
    switch (_card->m_rank + 2)
    {
    case 14:
        rankStr = "A";
        break;
    case 11:
        rankStr = "J";
        break;
    case 12:
        rankStr = "Q";
        break;
    case 13:
        rankStr = "K";
        break;
    default:
        sprintf(rank, "%d", _card->m_rank + 2);
        rankStr = rank;
        break;
    }

    sprintf(message, "%s%s", symb, rankStr);

    PrintMessage((_card->m_suit == HEARTS || _card->m_suit == DIAMONDS) ? RED : BLACK);
    PrintMessage(message);
    PrintMessage(NORMAL);
}

static void PrintTable(Card **_tableCards, size_t __numCardsOnTable)
{
    if (__numCardsOnTable == 0)
    {
        return;
    }

    PrintMessage("Table: ");
    for (size_t index = 0; index < __numCardsOnTable; index++)
    {
        if (_tableCards[index] != NULL)
        {
            PrintCard(_tableCards[index]);
            PrintMessage(" ");
        }
    }
    PrintMessage("\n");
}

static RoundResult PlayTrick(Round *_round, Card **_tableCards, StrategyContext *_strategyContext, RulesContext *_rulesContext)
{
    for (size_t playerNum = 0; playerNum < _round->m_numOfPlayers; playerNum++)
    {

        size_t relativeIndex = GET_RELATIVE_INDEX(playerNum, _rulesContext, _round);
        _rulesContext->m_hasTrickSuitCards = FALSE;
        if (_rulesContext->m_numOfCardsOnTable > 0)
        {
            _rulesContext->m_hasTrickSuitCards = PlayerHasSuit(_round->m_players[relativeIndex], _rulesContext->m_trickSuit) ? TRUE : FALSE;
        }
        if (ThrowCard(_round->m_players[relativeIndex], &_tableCards[playerNum], _tableCards,
                      PrintCard, IsValidCard, GetOptCard, _rulesContext, _strategyContext) != PLAYER_SUCCESS)
        {
            return ROUND_PLAY_TRICK_FAILED;
        }

        if (NEEDS_TO_BREAK(_tableCards, playerNum))
        {
            _rulesContext->m_heartsHasBeenBroken = TRUE;
        }

        ++_rulesContext->m_numOfCardsOnTable;

        PrintTable(_tableCards, _rulesContext->m_numOfCardsOnTable);
    }

    ++_strategyContext->m_trickNum;
    _strategyContext->m_heartsHasBeenBroken = _rulesContext->m_heartsHasBeenBroken;

    _rulesContext->m_openingPlayerNum = UpdateScore(_round, _tableCards, _rulesContext);

    DestroyTableCards(_tableCards, _round->m_numOfPlayers);

    return ROUND_SUCCESS;
}

static size_t WhoHasTwoClubs(Round *_round)
{
    Card twoClubs = {TWO, CLUBS};
    int isFound = ROUND_NOT_FOUND;
    size_t playerNum = 0;

    while (isFound == ROUND_NOT_FOUND && playerNum < _round->m_numOfPlayers)
    {
        isFound = FindCard(_round->m_players[playerNum++], &twoClubs);
    }

    return playerNum - 1;
}

static RoundResult StartRound(Round *_round, RoundNum _roundNum)
{
    static size_t roundCounter = 1;

    RulesContext rulesContext = {0};
    StrategyContext strategyContext = {0};

    Card **tableCards = calloc(_round->m_numOfPlayers, sizeof(Card *));
    if (tableCards == NULL)
    {
        return ROUND_ALLOCATION_ERROR;
    }

    if (PassCards(_round, _roundNum, tableCards, &strategyContext) != ROUND_SUCCESS)
    {
        free(tableCards);
        return ROUND_PASS_CARDS_FAILED;
    }
    rulesContext.m_openingPlayerNum = WhoHasTwoClubs(_round);

    for (size_t trickNum = 0; trickNum < NUM_OF_TRICKS; trickNum++)
    {
        rulesContext.m_numOfCardsOnTable = 0;
        rulesContext.m_trickNum = trickNum;

        char message[MAX_MESSAGE_SIZE];
        sprintf(message, "Round [%zu] | Trick [%zu/%d]\n", roundCounter, trickNum + 1, NUM_OF_TRICKS);
        PrintMessage(message);

        if (PlayTrick(_round, tableCards, &strategyContext, &rulesContext) != ROUND_SUCCESS)
        {
            free(tableCards);
            return ROUND_PLAY_TRICK_FAILED;
        }
        PrintScores(_round);
    }

    ++roundCounter;

    free(tableCards);

    return ROUND_SUCCESS;
}

static size_t CountSuitCards(Card **_cards, Suit _suit)
{
    size_t count = 0;
    size_t _numCards = GetNumOfCards(_cards);
    for (size_t i = 0; i < _numCards; i++)
    {
        if (_cards[i]->m_suit == _suit)
        {
            count++;
        }
    }
    return count;
}

static void SortCardsDescending(Card **_cards, size_t _numCards)
{
    for (size_t i = 1; i < _numCards; i++)
    {
        Card *key = _cards[i];
        size_t j = i;
        while (j > 0 && _cards[j - 1]->m_rank < key->m_rank)
        {
            _cards[j] = _cards[j - 1];
            j--;
        }
        _cards[j] = key;
    }
}

static size_t GetNumOfCards(Card **_cards)
{
    size_t count = 0;
    if (_cards == NULL)
    {
        return 0;
    }
    while (_cards[count] != NULL)
    {
        count++;
    }
    return count - 1;
}

static Card *GetLowestHigherCard(Card **_cards, size_t _numCards, Card *_refCard)
{
    Card *candidate = NULL;
    for (size_t i = 0; i < _numCards; i++)
    {
        if (_cards[i]->m_suit == _refCard->m_suit && _cards[i]->m_rank > _refCard->m_rank)
        {
            if (!candidate || _cards[i]->m_rank < candidate->m_rank)
            {
                candidate = _cards[i];
            }
        }
    }
    return candidate;
}

static Card *GetHighestCardOfSuit(Card **_cards, size_t _numCards, Suit _suit)
{
    Card *highest = NULL;
    for (size_t i = 0; i < _numCards; i++)
    {
        if (_cards[i]->m_suit == _suit)
        {
            if (!highest || _cards[i]->m_rank > highest->m_rank)
            {
                highest = _cards[i];
            }
        }
    }
    return highest;
}

static Card *GetHighestCard(Card **_cards, size_t _numCards)
{
    Card *highest = _cards[0];
    for (size_t i = 1; i < _numCards; i++)
    {
        if (_cards[i]->m_rank > highest->m_rank)
        {
            highest = _cards[i];
        }
    }
    return highest;
}

static Card *GetLowestCard(Card **_cards, size_t _numCards)
{
    Card *lowest = _cards[0];
    for (size_t i = 1; i < _numCards; i++)
    {
        if (_cards[i]->m_rank < lowest->m_rank)
        {
            lowest = _cards[i];
        }
    }
    return lowest;
}

static Card *GetLowestCardOfSuit(Card **_cards, size_t _numCards, Suit _suit)
{
    Card *lowest = NULL;
    for (size_t i = 0; i < _numCards; i++)
    {
        if (_cards[i]->m_suit == _suit)
        {
            if (!lowest || _cards[i]->m_rank < lowest->m_rank)
            {
                lowest = _cards[i];
            }
        }
    }
    return lowest;
}

/* Rules */

int IsValidCard(Card *_card, Card **_table, void *_context)
{
    if (_context == NULL)
    {
        return TRUE;
    }
    RulesContext *rules = (RulesContext *)(_context);

    if (IS_FIRST_TRICK(rules->m_trickNum))
    {
        if (IS_EMPTY_TABLE(rules) && !IS_TWO_OF_CLUBS(_card))
        {
            PrintMessage("First trick must start with Two of Clubs.\n");
            return FALSE;
        }
        else if (IS_HEART_SUIT(_card) || IS_QUEEN_OF_SPADES(_card))
        {
            PrintMessage("Cannot play Hearts or Queen of Spades on the first trick.\n");
            return FALSE;
        }
        Card *openingCard = _table[rules->m_openingPlayerNum];
        if (openingCard != NULL && !ARE_SAME_SUIT(_card, openingCard) && rules->m_hasTrickSuitCards)
        {
            PrintMessage("Must follow suit if you have a card of the same suit.\n");
            return FALSE;
        }
    }
    else if (IS_EMPTY_TABLE(rules))
    {
        if (IS_HEART_SUIT(_card) && !HEARTS_BROKEN(rules))
        {
            // PrintMessage("Cannot lead with a Heart until Hearts have been broken.\n");
            return FALSE;
        }
        rules->m_trickSuit = _card->m_suit;
    }
    else
    {
        Card *openingCard = _table[rules->m_openingPlayerNum];
        if (!ARE_SAME_SUIT(_card, openingCard) && rules->m_hasTrickSuitCards)
        {
            // PrintMessage("Must follow suit if you have a card of the same suit.\n");
            return FALSE;
        }
    }

    return TRUE;
}

/* Strategy */

Card *GetOptCard(Card **_cards, Card **_table, void *_context)
{

    RulesContext *rules = (RulesContext *)_context;
    size_t _numCards = GetNumOfCards(_cards);

    /* 1. If leading the trick */
    if (IS_EMPTY_TABLE(rules))
    {
        Card *selectedCard = NULL;
        /* 1.1 Find suits with less than 2 cards */
        Suit suits[] = {HEARTS, DIAMONDS, CLUBS, SPADES};
        for (int s = 0; s < NUMBER_OF_SUITS; s++)
        {
            size_t suitCount = CountSuitCards(_cards, suits[s]);
            if (suitCount > 0 && suitCount < 2)
            {
                selectedCard = GetLowestCardOfSuit(_cards, _numCards, suits[s]);
                break;
            }
        }
        if (!selectedCard)
        {
            /* 1.2 No suit with less than 2 cards, pick lowest card from any suit */
            selectedCard = GetLowestCard(_cards, _numCards);
        }
        return selectedCard;
    }
    else
    {
        /* 2. Following the trick */
        // Card *highestTableCard = GetHighestCardOfSuit(_table, rules->m_numOfCardsOnTable, rules->m_trickSuit);
        Card *selectedCard = NULL;

        // /* 2.1 Check if player has cards of the leading suit */
        // int hasLeadingSuit = CountSuitCards(_cards, rules->m_trickSuit) > 0;

        // if (hasLeadingSuit)
        // {
        //     /* 2.1.1 Try to play the lowest card higher than highest on table */
        //     selectedCard = GetLowestHigherCard(_cards, _numCards, highestTableCard);
        //     if (!selectedCard)
        //     {
        //         /* 2.1.2 No higher card, play lowest card of leading suit */
        //         selectedCard = GetLowestCardOfSuit(_cards, _numCards, rules->m_trickSuit);
        //     }
        // }
        // else
        // {
        // /* 2.2 Dont have leading suit, try play highest hearts */
        // size_t heartCount = CountSuitCards(_cards, HEARTS);
        // if (heartCount > 0)
        // {
        //     selectedCard = GetHighestCardOfSuit(_cards, _numCards, HEARTS);
        // }
        // else
        // {
        /* 2.3 Play highest of others */
        selectedCard = GetHighestCard(_cards, _numCards);
        // }
        // }
        return selectedCard;
    }
}

Card *GetOptCardPass(Card **_cards, Card **_table, void *_context)
{

    if (_cards == NULL)
    {
        return NULL;
    }

    size_t _numCards = GetNumOfCards(_cards);

    /* 1. Pass the Queen of Spades if present */
    for (size_t i = 0; i < _numCards; i++)
    {
        if (IS_QUEEN_OF_SPADES(_cards[i]))
        {
            return _cards[i];
        }
    }

    /* 2. Pass suits with less than 3 cards */
    Suit suits[] = {HEARTS, DIAMONDS, CLUBS, SPADES};
    for (int s = 0; s < NUMBER_OF_SUITS; s++)
    {
        size_t suitCount = CountSuitCards(_cards, suits[s]);
        if (suitCount > 0 && suitCount < 3)
        {
            for (size_t i = 0; i < _numCards; i++)
            {
                if (_cards[i]->m_suit == suits[s])
                {
                    return _cards[i];
                }
            }
        }
    }

    /* 3. Pass highest cards if still need to pass more */

    /* Sort remaining cards in descending order */
    SortCardsDescending(_cards, _numCards);

    return _cards[0];
}
