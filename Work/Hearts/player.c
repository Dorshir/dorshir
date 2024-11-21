#include "player.h"
#include "genvec.h"
#include "bstree.h"
#include "card.h"
#include "ui.h"
#include <stdlib.h> /* malloc, free, size_t */
#include <string.h> /* strdup */


#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1
#define HAS_CARDS_FOR_THIS_SUIT(begin, end) (!BSTreeItrEquals(begin, end))
#define ARE_CARDS_EQUAL(first, second) (first->m_suit == second->m_suit && first->m_rank == second->m_rank)

struct Player
{
    char *m_name;
    PlayerType m_type;
    Vector *m_cards;
    size_t m_numOfCards;
};

static PlayerResult InitCardsVector(Player *_player);
static int CardComparator(void *_left, void *_right);
static PlayerResult CheckInputThrowCard(Player *_player, void **_pValue);
static BSTreeItr ChooseCard(Player *_player);
static BSTreeItr GetFirstCardExist(Player *_player);
static void RevealCards(Player *_player);
static void BuildCardMessage(Card *_card, char *_cardDetails);
static BSTreeItr GetValidCard(Player *_player, Card **_table, RulesFunction _rulesFunc, void *_rulesContext);
static void GetValidCardsMachine(Player *_player, Card **_table, Card **_validCards, RulesFunction _rulesFunc, void *_rulesContext);
static Card **GetValidCardListMachine(Player *_player, Card **_table, RulesFunction _rulesFunc, void *_rulesContext);


/* API Functions */

Player *CreatePlayer(PlayerType _type, char *_name)
{
    if (_type > HUMAN || _type < MACHINE || _name == NULL)
    {
        return NULL;
    }

    Player *newPlayer = malloc(sizeof(Player));
    if (newPlayer == NULL)
    {
        return NULL;
    }

    newPlayer->m_cards = VectorCreate(NUMBER_OF_SUITS, NUMBER_OF_SUITS);
    if (newPlayer->m_cards == NULL)
    {
        free(newPlayer);
        return NULL;
    }

    if (InitCardsVector(newPlayer) != PLAYER_SUCCESS)
    {
        DestroyPlayer(&newPlayer);
        return NULL;
    }

    newPlayer->m_type = _type;
    newPlayer->m_numOfCards = 0;
    newPlayer->m_name = strdup(_name);

    return newPlayer;
}

void DestroyPlayer(Player **_player)
{
    if (_player != NULL && *_player != NULL)
    {
        BSTree *tree;
        for (size_t i = 0; i < NUMBER_OF_SUITS; i++)
        {
            VectorGet((*_player)->m_cards, i, (void **)&tree);
            BSTreeDestroy(&tree, free); /* MAYBE CARDS WILL BE DESTROYED SOMWHERE ELSE? */
        }

        free((*_player)->m_cards);
        free((*_player)->m_name);
        free(*_player);
        *_player = NULL;
    }
}

PlayerResult ReceiveCard(Player *_player, Card **_card)
{
    BSTree *tree;
    if (_player == NULL || _card == NULL || *_card == NULL)
    {
        return PLAYER_UNINITIALIZED;
    }

    Suit suit = (*_card)->m_suit;

    VectorGet(_player->m_cards, suit, (void **)&tree);

    BSTreeItr end = BSTreeItrEnd(tree);
    BSTreeItr itr = BSTreeInsert(tree, (*_card));

    if (itr == end || itr == NULL)
    {
        return PLAYER_REC_CRAD_FAILED;
    }

    ++_player->m_numOfCards;

    return PLAYER_SUCCESS;
}

PlayerResult ThrowCard(Player *_player, Card **_pValue, Card **_table, RulesFunction _rulesFunc, StrategyFunction _strategyFunc, void *_rulesContext, void *_strategyContext)
{
    BSTreeItr cardToThrow;
    PlayerResult inputCheck = CheckInputThrowCard(_player, _pValue);
    if (inputCheck != PLAYER_SUCCESS)
    {
        return inputCheck;
    }

    if (_player->m_type == HUMAN)
    {
        cardToThrow = GetValidCard(_player, _table, _rulesFunc, _rulesContext);
    }
    else /* MACHINE */
    {
        Card **validCards = GetValidCardListMachine(_player, _table, _rulesFunc, _rulesContext);
        if (validCards == NULL)
        {
            return PLAYER_THROW_CARD_FAILED;
        }

        /* Currently just throwing the first card you see... */
        cardToThrow = GetFirstCardExist(_player);
    }

    *_pValue = BSTreeItrRemove(cardToThrow);
    --_player->m_numOfCards;
    return PLAYER_SUCCESS;
}

int FindCard(Player *_player, Card *_desiredCard)
{
    BSTree *currBSTreeSuit;
    size_t cardIndex = 0;
    if (_player == NULL || _desiredCard == NULL)
    {
        return NOT_FOUND;
    }

    for (size_t suit = HEARTS; suit < NUMBER_OF_SUITS; suit++)
    {
        VectorGet(_player->m_cards, suit, (void **)&currBSTreeSuit);
        BSTreeItr begin = BSTreeItrBegin(currBSTreeSuit);
        BSTreeItr end = BSTreeItrEnd(currBSTreeSuit);

        while (begin != end)
        {
            Card *currCard = BSTreeItrGet(begin);

            if (ARE_CARDS_EQUAL(currCard, _desiredCard))
            {
                return cardIndex;
            }

            begin = BSTreeItrNext(begin);
            ++cardIndex;
        }
    }
    return NOT_FOUND;
}

/* Static Functions */

static int CardComparator(void *_left, void *_right)
{
    if (_left == NULL || _right == NULL)
    {
        return 0;
    }
    Card *leftCard = (Card *)_left;
    Card *rightCard = (Card *)_right;

    if (leftCard->m_rank > rightCard->m_rank)
    {
        return -1;
    }
    else if (leftCard->m_rank < rightCard->m_rank)
    {
        return 1;
    }
    return 0;
}

static PlayerResult InitCardsVector(Player *_player)
{
    BSTree *tree;
    for (size_t i = 0; i < NUMBER_OF_SUITS; i++)
    {
        tree = BSTreeCreate(CardComparator);
        if (tree == NULL)
        {
            return PLAYER_UNINITIALIZED;
        }
        VectorAppend(_player->m_cards, tree);
    }
    return PLAYER_SUCCESS;
}

static BSTreeItr GetFirstCardExist(Player *_player)
{
    BSTreeItr cardToThrow = NULL;
    BSTree *currBSTreeSuit;
    for (size_t suit = HEARTS; suit < NUMBER_OF_SUITS; suit++)
    {

        VectorGet(_player->m_cards, suit, (void **)&currBSTreeSuit);
        BSTreeItr begin = BSTreeItrBegin(currBSTreeSuit);
        BSTreeItr end = BSTreeItrEnd(currBSTreeSuit);

        if (HAS_CARDS_FOR_THIS_SUIT(begin, end))
        {
            cardToThrow = begin;
        }
    }
    return cardToThrow;
}

static PlayerResult CheckInputThrowCard(Player *_player, void **_pValue)
{
    PlayerResult result = PLAYER_SUCCESS;
    if (_player == NULL || _pValue == NULL)
    {
        result = PLAYER_UNINITIALIZED;
    }
    else if (_player->m_numOfCards == 0)
    {
        result = PLAYER_EMPTY_CARDS;
    }
    return result;
}

static void RevealCards(Player *_player)
{
    BSTree *currBSTreeSuit;

    PrintMessage("Revealing cards of player: ");
    PrintMessage(_player->m_name);
    PrintMessage("\n");

    for (size_t suit = HEARTS; suit < NUMBER_OF_SUITS; suit++)
    {
        VectorGet(_player->m_cards, suit, (void **)&currBSTreeSuit);
        BSTreeItr begin = BSTreeItrBegin(currBSTreeSuit);
        BSTreeItr end = BSTreeItrEnd(currBSTreeSuit);

        while (begin != end)
        {
            Card *currCard = BSTreeItrGet(begin);
            char cardDetails[50] = "";

            BuildCardMessage(currCard, cardDetails);
            PrintMessage(cardDetails);

            begin = BSTreeItrNext(begin);
        }
    }
}

static BSTreeItr ChooseCard(Player *_player)
{
    BSTreeItr cardToThrow = NULL;
    RevealCards(_player);

    return cardToThrow;
}

static BSTreeItr GetValidCard(Player *_player, Card **_table, RulesFunction _rulesFunc, void *_rulesContext)
{
    BSTreeItr cardToThrow;
    if (_rulesFunc == NULL || _rulesContext == NULL)
    {
        return ChooseCard(_player);
    }

    while (TRUE)
    {
        cardToThrow = ChooseCard(_player);
        if (_rulesFunc(cardToThrow, _table, _rulesContext))
        {
            break;
        }
    }
    return cardToThrow;
}

static void GetValidCardsMachine(Player *_player, Card **_table, Card **_validCards, RulesFunction _rulesFunc, void *_rulesContext)
{
    BSTree *currBSTreeSuit;
    size_t cardIndex = 0;
    for (size_t suit = HEARTS; suit < NUMBER_OF_SUITS; suit++)
    {
        VectorGet(_player->m_cards, suit, (void **)&currBSTreeSuit);
        BSTreeItr begin = BSTreeItrBegin(currBSTreeSuit);
        BSTreeItr end = BSTreeItrEnd(currBSTreeSuit);

        while (begin != end)
        {
            Card *currCard = BSTreeItrGet(begin);
            if (_rulesFunc(currCard, _table, _rulesContext))
            {
                _validCards[cardIndex++] = currCard;
            }
            begin = BSTreeItrNext(begin);
        }
    }
}

static Card **GetValidCardListMachine(Player *_player, Card **_table, RulesFunction _rulesFunc, void *_rulesContext)
{
    Card **validCards;
    if (_player->m_numOfCards == 0)
    {
        return NULL;
    }

    validCards = malloc(sizeof(Card *) * _player->m_numOfCards);
    if (validCards == NULL)
    {
        return NULL;
    }

    GetValidCardsMachine(_player, _table, validCards, _rulesFunc, _rulesContext);

    return validCards;
}

/* Getter Functions */

const char *PlayerGetName(const Player *_player)
{
    if (_player == NULL)
    {
        return NULL;
    }
    return _player->m_name;
}

PlayerType PlayerGetType(const Player *_player)
{
    if (_player == NULL)
    {
        return MACHINE; // Default value
    }
    return _player->m_type;
}

size_t PlayerGetNumOfCards(const Player *_player)
{
    if (_player == NULL)
    {
        return 0;
    }
    return _player->m_numOfCards;
}

static void BuildCardMessage(Card *_card, char *_cardDetails)
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
        [ACE] = "Ace"};

    strcat(_cardDetails, " [");
    strcat(_cardDetails, suitNames[_card->m_suit]);
    strcat(_cardDetails, " : ");
    strcat(_cardDetails, rankNames[_card->m_rank]);
    strcat(_cardDetails, "]\n");
}
