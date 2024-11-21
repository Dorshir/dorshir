// test_player.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include Header Files */
#include "player.h"
#include "card.h"
#include "genvec.h"
#include "bstree.h"

int DummyRulesFunction(Card *_card, Card **_table, void *_context)
{
    return 1; // Always returns true, indicating the card is valid
}

Card *DummyStrategyFunction(Card **_validCards, Card ** _tableCards, void *_context)
{
    // For testing, return the first valid card
    if (_validCards == NULL || _tableCards == NULL)
    {
        return NULL;
    }
    return _validCards[0];
}

const char *PlayerGetName(const Player *_player);

PlayerType PlayerGetType(const Player *_player);

size_t PlayerGetNumOfCards(const Player *_player);


/* Utility Functions */
static Card* CreateSampleCard(Rank rank, Suit suit);
static void PrintPlayerDetails(const Player *player);
static void FreeCard(void *card);

/* Test Function Declarations */
/* CreatePlayer Tests */
static void TestCreatePlayer_ValidHuman();
static void TestCreatePlayer_ValidMachine();
static void TestCreatePlayer_NullName();
static void TestCreatePlayer_InvalidType();

/* DestroyPlayer Tests */
static void TestDestroyPlayer_Valid();
static void TestDestroyPlayer_NullPlayer();
static void TestDestroyPlayer_DoubleDestroy();

/* ReceiveCard Tests */
static void TestReceiveCard_Valid();
static void TestReceiveCard_NullPlayer();
static void TestReceiveCard_NullCardPointer();
static void TestReceiveCard_NullCard();

/* ThrowCard Tests */
static void TestThrowCard_Valid();
static void TestThrowCard_NullPlayer();
static void TestThrowCard_NullValuePointer();
static void TestThrowCard_EmptyHand();

/* Getter Functions Tests */
static void TestPlayerGetName_Valid();
static void TestPlayerGetName_NullPlayer();
static void TestPlayerGetType_Valid();
static void TestPlayerGetType_NullPlayer();
static void TestPlayerGetNumOfCards_Valid();
static void TestPlayerGetNumOfCards_NullPlayer();

/* Main Function */
int main()
{
    /* CreatePlayer Tests */
    TestCreatePlayer_ValidHuman();
    TestCreatePlayer_ValidMachine();
    TestCreatePlayer_NullName();
    TestCreatePlayer_InvalidType();

    /* DestroyPlayer Tests */
    TestDestroyPlayer_Valid();
    TestDestroyPlayer_NullPlayer();
    TestDestroyPlayer_DoubleDestroy();

    /* ReceiveCard Tests */
    TestReceiveCard_Valid();
    TestReceiveCard_NullPlayer();
    TestReceiveCard_NullCardPointer();
    TestReceiveCard_NullCard();

    /* ThrowCard Tests */
    TestThrowCard_Valid();
    TestThrowCard_NullPlayer();
    TestThrowCard_NullValuePointer();
    TestThrowCard_EmptyHand();

    /* Getter Functions Tests */
    TestPlayerGetName_Valid();
    TestPlayerGetName_NullPlayer();
    TestPlayerGetType_Valid();
    TestPlayerGetType_NullPlayer();
    TestPlayerGetNumOfCards_Valid();
    TestPlayerGetNumOfCards_NullPlayer();

    return 0;
}

/* Utility Functions */

static Card* CreateSampleCard(Rank rank, Suit suit)
{
    Card *newCard = malloc(sizeof(Card));
    if (newCard == NULL)
    {
        return NULL;
    }
    newCard->m_rank = rank;
    newCard->m_suit = suit;
    return newCard;
}

static void FreeCard(void *card)
{
    free(card);
}

static void PrintPlayerDetails(const Player *player)
{
    if (player == NULL)
    {
        printf("Player is NULL.\n");
        return;
    }

    const char *name = PlayerGetName(player);
    if (name != NULL)
    {
        printf("Player Name: %s\n", name);
    }
    else
    {
        printf("Player Name: (null)\n");
    }

    PlayerType type = PlayerGetType(player);
    printf("Player Type: %s\n", type == HUMAN ? "Human" : "Machine");

    size_t numCards = PlayerGetNumOfCards(player);
    printf("Number of Cards: %zu\n", numCards);
}

/* Test Function Implementations */

/* CreatePlayer Tests */

void TestCreatePlayer_ValidHuman()
{
    printf("TestCreatePlayer_ValidHuman: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    if (player != NULL && PlayerGetType(player) == HUMAN && strcmp(PlayerGetName(player), "Alice") == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

void TestCreatePlayer_ValidMachine()
{
    printf("TestCreatePlayer_ValidMachine: ");
    Player *player = CreatePlayer(MACHINE, "Bot");
    if (player != NULL && PlayerGetType(player) == MACHINE && strcmp(PlayerGetName(player), "Bot") == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

void TestCreatePlayer_NullName()
{
    printf("TestCreatePlayer_NullName: ");
    Player *player = CreatePlayer(HUMAN, NULL);
    if (player == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        DestroyPlayer(&player);
    }
}

void TestCreatePlayer_InvalidType()
{
    printf("TestCreatePlayer_InvalidType: ");
    Player *player = CreatePlayer((PlayerType)(-1), "Invalid");
    if (player == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        DestroyPlayer(&player);
    }
}

/* DestroyPlayer Tests */

void TestDestroyPlayer_Valid()
{
    printf("TestDestroyPlayer_Valid: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    DestroyPlayer(&player);
    if (player == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

void TestDestroyPlayer_NullPlayer()
{
    printf("TestDestroyPlayer_NullPlayer: ");
    DestroyPlayer(NULL);
    printf("PASS\n");
}

void TestDestroyPlayer_DoubleDestroy()
{
    printf("TestDestroyPlayer_DoubleDestroy: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    DestroyPlayer(&player);
    DestroyPlayer(&player); // Should handle gracefully
    if (player == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

/* ReceiveCard Tests */

void TestReceiveCard_Valid()
{
    printf("TestReceiveCard_Valid: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    Card *card = CreateSampleCard(ACE, SPADES);
    if (ReceiveCard(player, &card) == PLAYER_SUCCESS && PlayerGetNumOfCards(player) == 1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player); // Cards should be freed in DestroyPlayer
}

void TestReceiveCard_NullPlayer()
{
    printf("TestReceiveCard_NullPlayer: ");
    Card *card = CreateSampleCard(ACE, SPADES);
    if (ReceiveCard(NULL, &card) == PLAYER_UNINITIALIZED)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    FreeCard(card);
}

void TestReceiveCard_NullCardPointer()
{
    printf("TestReceiveCard_NullCardPointer: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    if (ReceiveCard(player, NULL) == PLAYER_UNINITIALIZED)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

void TestReceiveCard_NullCard()
{
    printf("TestReceiveCard_NullCard: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    Card *card = NULL;
    if (ReceiveCard(player, &card) == PLAYER_UNINITIALIZED)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

/* ThrowCard Tests */

void TestThrowCard_Valid()
{
    printf("TestThrowCard_Valid: ");
    Player *player = CreatePlayer(MACHINE, "Computer1");
    Card *card1 = CreateSampleCard(ACE, SPADES);
    Card *card2 = CreateSampleCard(KING, HEARTS);
    ReceiveCard(player, &card1);
    ReceiveCard(player, &card2);

    Card *thrownCard;
    Card *table[4] = {NULL};
    void *rulesContext = NULL;
    void *strategyContext = NULL;

    if (ThrowCard(player, &thrownCard, table, DummyRulesFunction, DummyStrategyFunction, rulesContext, strategyContext) == PLAYER_SUCCESS && PlayerGetNumOfCards(player) == 1)
    {
        printf("PASS\n");
        FreeCard(thrownCard);
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

void TestThrowCard_NullPlayer()
{
    printf("TestThrowCard_NullPlayer: ");
    Card *thrownCard;
    Card *table[4] = {NULL};
    void *rulesContext = NULL;
    void *strategyContext = NULL;

    if (ThrowCard(NULL, &thrownCard, table, DummyRulesFunction, DummyStrategyFunction, rulesContext, strategyContext) == PLAYER_UNINITIALIZED)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

void TestThrowCard_NullValuePointer()
{
    printf("TestThrowCard_NullValuePointer: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    Card *table[4] = {NULL};
    void *rulesContext = NULL;
    void *strategyContext = NULL;

    if (ThrowCard(player, NULL, table, DummyRulesFunction, DummyStrategyFunction, rulesContext, strategyContext) == PLAYER_UNINITIALIZED)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}


void TestThrowCard_EmptyHand()
{
    printf("TestThrowCard_EmptyHand: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    Card *thrownCard;
    Card* table;
    void *rulesContext = NULL;
    void *strategyContext = NULL;
    if (ThrowCard(player, &thrownCard, &table, DummyRulesFunction, DummyStrategyFunction, rulesContext, strategyContext) == PLAYER_EMPTY_CARDS)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

/* Getter Functions Tests */

void TestPlayerGetName_Valid()
{
    printf("TestPlayerGetName_Valid: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    if (strcmp(PlayerGetName(player), "Alice") == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

void TestPlayerGetName_NullPlayer()
{
    printf("TestPlayerGetName_NullPlayer: ");
    if (PlayerGetName(NULL) == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

void TestPlayerGetType_Valid()
{
    printf("TestPlayerGetType_Valid: ");
    Player *player = CreatePlayer(MACHINE, "Bot");
    if (PlayerGetType(player) == MACHINE)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

void TestPlayerGetType_NullPlayer()
{
    printf("TestPlayerGetType_NullPlayer: ");
    if (PlayerGetType(NULL) == MACHINE) // Assuming MACHINE is default
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

void TestPlayerGetNumOfCards_Valid()
{
    printf("TestPlayerGetNumOfCards_Valid: ");
    Player *player = CreatePlayer(HUMAN, "Alice");
    Card *card = CreateSampleCard(ACE, SPADES);
    ReceiveCard(player, &card);
    if (PlayerGetNumOfCards(player) == 1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    DestroyPlayer(&player);
}

void TestPlayerGetNumOfCards_NullPlayer()
{
    printf("TestPlayerGetNumOfCards_NullPlayer: ");
    if (PlayerGetNumOfCards(NULL) == 0)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

