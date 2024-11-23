#include "player.h"
#include "genvec.h"
#include "bstree.h"
#include "card.h"
#include "ui.h"
#include <stdlib.h> /* malloc, free, size_t */
#include <string.h> /* strdup */
#include <stdio.h>

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
static PlayerResult CheckInputThrowCard(Player *_player, Card **_pValue, Card **_table, PrintCardFunc _printFunc, RulesFunction _rulesFunc, StrategyFunction _strategyFunc);
static BSTreeItr ChooseCard(Player *_player, PrintCardFunc _printFunc);
static BSTreeItr GetFirstCardExist(Player *_player);
static void RevealCards(Player *_player, PrintCardFunc _printFunc);
static void BuildCardIndexArray(Player *_player, BSTreeItr *cardItrArray);
static size_t GetUserInput(size_t numOfCards);
static BSTreeItr GetValidCard(Player *_player, Card **_table, PrintCardFunc _printFunc, RulesFunction _rulesFunc, void *_rulesContext);
static size_t GetValidCardsMachine(Player *_player, Card **_table, Card **_validCards, RulesFunction _rulesFunc, void *_rulesContext);
static Card **GetValidCardListMachine(Player *_player, Card **_table, RulesFunction _rulesFunc, void *_rulesContext);
static size_t GetValidCardItersMachine(Player *_player, Card **_table, BSTreeItr *validCardIters, RulesFunction _rulesFunc, void *_rulesContext);

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
        VectorDestroy(&((*_player)->m_cards), NULL);
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

PlayerResult ThrowCard(Player *_player, Card **_pValue, Card **_table, PrintCardFunc _printFunc, RulesFunction _rulesFunc, StrategyFunction _strategyFunc, void *_rulesContext, void *_strategyContext)
{
    BSTreeItr cardToThrow;
    PlayerResult inputCheck = CheckInputThrowCard(_player, _pValue, _table, _printFunc, _rulesFunc, _strategyFunc);
    if (inputCheck != PLAYER_SUCCESS)
    {
        return inputCheck;
    }

    if (_player->m_type == HUMAN)
    {
        cardToThrow = GetValidCard(_player, _table, _printFunc, _rulesFunc, _rulesContext);
    }
    else /* MACHINE PLAYER */
    {
        BSTreeItr *validCardIters = malloc(_player->m_numOfCards * sizeof(BSTreeItr));
        if (validCardIters == NULL)
        {
            return PLAYER_THROW_CARD_FAILED;
        }

        size_t validCardCount = GetValidCardItersMachine(_player, _table, validCardIters, _rulesFunc, _rulesContext);

        if (validCardCount == 0)
        {
            free(validCardIters);
            return PLAYER_THROW_CARD_FAILED;
        }

        cardToThrow = validCardIters[0]; /* For now, pick the first valid card */

        free(validCardIters);
    }

    if (cardToThrow == NULL)
    {
        return PLAYER_EMPTY_CARDS;
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
        return PLAYER_CARD_NOT_FOUND;
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
            break;
        }
    }
    return cardToThrow;
}

static PlayerResult CheckInputThrowCard(Player *_player, Card **_pValue, Card **_table, PrintCardFunc _printFunc, RulesFunction _rulesFunc, StrategyFunction _strategyFunc)
{
    PlayerResult result = PLAYER_SUCCESS;
    if (_player == NULL || _pValue == NULL || _printFunc == NULL)
    {
        result = PLAYER_UNINITIALIZED;
    }
    else if (_player->m_numOfCards == 0)
    {
        result = PLAYER_EMPTY_CARDS;
    }
    return result;
}

static void RevealCards(Player *_player, PrintCardFunc _printFunc)
{
    size_t displayIndex = 1;
    BSTree *currBSTreeSuit;

    PrintMessage("| ");

    for (size_t suit = 0; suit < NUMBER_OF_SUITS; suit++)
    {
        VectorGet(_player->m_cards, suit, (void **)&currBSTreeSuit);
        BSTreeItr begin = BSTreeItrBegin(currBSTreeSuit);
        BSTreeItr end = BSTreeItrEnd(currBSTreeSuit);

        while (!BSTreeItrEquals(begin, end))
        {
            Card *currCard = BSTreeItrGet(begin);

            char message[30] = "";
            sprintf(message, "%zu: ", displayIndex++);
            PrintMessage(message);
            _printFunc(currCard);
            PrintMessage(" | ");

            begin = BSTreeItrNext(begin);
        }
    }
    PrintMessage("\n");
}

static void BuildCardIndexArray(Player *_player, BSTreeItr *cardItrArray)
{
    size_t index = 0;
    BSTree *currBSTreeSuit;

    for (size_t suit = 0; suit < NUMBER_OF_SUITS; suit++)
    {
        VectorGet(_player->m_cards, suit, (void **)&currBSTreeSuit);
        BSTreeItr begin = BSTreeItrBegin(currBSTreeSuit);
        BSTreeItr end = BSTreeItrEnd(currBSTreeSuit);

        while (!BSTreeItrEquals(begin, end))
        {
            cardItrArray[index++] = begin;
            begin = BSTreeItrNext(begin);
        }
    }
}

static size_t GetUserInput(size_t numOfCards)
{
    size_t chosenIndex = 0;
    int validInput = 0;
    while (!validInput)
    {
        PrintMessage("Please choose a card to throw (by index): ");
        char input[100];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            PrintMessage("Error reading input. Please try again.\n");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';
        char *endptr;
        long inputIndex = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0')
        {
            PrintMessage("Invalid input. Please enter a number.\n");
            continue;
        }
        if (inputIndex < 1 || inputIndex > numOfCards)
        {
            PrintMessage("Invalid index.\n");
            continue;
        }
        chosenIndex = (size_t)(inputIndex - 1);
        validInput = 1;
    }

    PrintMessage("\n");

    return chosenIndex;
}

static BSTreeItr ChooseCard(Player *_player, PrintCardFunc _printFunc)
{
    if (_player == NULL || _printFunc == NULL)
    {
        return NULL;
    }

    size_t numOfCards = _player->m_numOfCards;
    if (numOfCards == 0)
    {
        return NULL;
    }

    BSTreeItr *cardItrArray = malloc(numOfCards * sizeof(BSTreeItr));
    if (cardItrArray == NULL)
    {
        return NULL;
    }

    RevealCards(_player, _printFunc);

    BuildCardIndexArray(_player, cardItrArray);

    size_t chosenIndex = GetUserInput(numOfCards);

    BSTreeItr cardToThrow = cardItrArray[chosenIndex];

    free(cardItrArray);

    return cardToThrow;
}

static BSTreeItr GetValidCard(Player *_player, Card **_table, PrintCardFunc _printFunc, RulesFunction _rulesFunc, void *_rulesContext)
{
    BSTreeItr cardItr;
    if (_rulesFunc == NULL || _rulesContext == NULL)
    {
        return ChooseCard(_player, _printFunc);
    }

    while (TRUE)
    {
        cardItr = ChooseCard(_player, _printFunc);

        Card *selectedCard = BSTreeItrGet(cardItr);
        if (_rulesFunc(selectedCard, _table, _rulesContext))
        {
            break;
        }
    }
    return cardItr;
}

static size_t GetValidCardItersMachine(Player *_player, Card **_table, BSTreeItr *validCardIters, RulesFunction _rulesFunc, void *_rulesContext)
{
    BSTree *currBSTreeSuit;
    size_t cardIndex = 0;
    for (size_t suit = 0; suit < NUMBER_OF_SUITS; suit++)
    {
        VectorGet(_player->m_cards, suit, (void **)&currBSTreeSuit);
        BSTreeItr begin = BSTreeItrBegin(currBSTreeSuit);
        BSTreeItr end = BSTreeItrEnd(currBSTreeSuit);

        while (!BSTreeItrEquals(begin, end))
        {
            Card *currCard = BSTreeItrGet(begin);
            if (_rulesFunc(currCard, _table, _rulesContext))
            {
                validCardIters[cardIndex++] = begin;
            }
            begin = BSTreeItrNext(begin);
        }
    }
    return cardIndex;
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
