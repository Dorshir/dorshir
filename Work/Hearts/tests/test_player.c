#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "card.h"
#include "genvec.h"
#include "bstree.h"

const char* PlayerGetName(const Player *_player);
PlayerType PlayerGetType(const Player *_player);
size_t PlayerGetNumOfCards(const Player *_player);

/* Function to print player details using getters */
void PrintPlayerDetails(const Player *player)
{
    if (player == NULL)
    {
        printf("Player is NULL.\n");
        return;
    }

    printf("Player Name: %s\n", PlayerGetName(player));
    printf("Player Type: %s\n", PlayerGetType(player) == HUMAN ? "Human" : "Machine");
    printf("Number of Cards: %zu\n", PlayerGetNumOfCards(player));
}

/* Function to create a sample card */
Card *CreateSampleCard(Rank rank, Suit suit)
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

int main()
{
    /* Test 1: Create a Human Player */
    printf("=== Test 1: Create a Human Player ===\n");
    Player *humanPlayer = CreatePlayer(HUMAN, "Alice");
    if (humanPlayer == NULL)
    {
        printf("Failed to create Human Player.\n");
        return EXIT_FAILURE;
    }
    PrintPlayerDetails(humanPlayer);
    printf("\n");

    /* Test 2: Create a Machine Player */
    printf("=== Test 2: Create a Machine Player ===\n");
    Player *machinePlayer = CreatePlayer(MACHINE, "Bot");
    if (machinePlayer == NULL)
    {
        printf("Failed to create Machine Player.\n");
        DestroyPlayer(&humanPlayer);
        return EXIT_FAILURE;
    }
    PrintPlayerDetails(machinePlayer);
    printf("\n");

    /* Test 3: Attempt to Create a Player with Invalid Type */
    printf("=== Test 3: Create Player with Invalid Type ===\n");
    Player *invalidPlayer = CreatePlayer((PlayerType)(-1), "Invalid");
    if (invalidPlayer == NULL)
    {
        printf("Correctly failed to create Player with invalid type.\n");
    }
    else
    {
        printf("Error: Created Player with invalid type.\n");
        DestroyPlayer(&invalidPlayer);
    }
    printf("\n");

    /* Test 4: Receive Cards for Human Player */
    printf("=== Test 4: Receive Cards for Human Player ===\n");
    Card *card1 = CreateSampleCard(TWO, HEARTS);
    Card *card2 = CreateSampleCard(ACE, SPADES);
    Card *card3 = CreateSampleCard(TEN, CLUBS);

    if (ReceiveCard(humanPlayer, &card1) != PLAYER_SUCCESS)
    {
        printf("Failed to receive card1 for Human Player.\n");
    }
    if (ReceiveCard(humanPlayer, &card2) != PLAYER_SUCCESS)
    {
        printf("Failed to receive card2 for Human Player.\n");
    }
    if (ReceiveCard(humanPlayer, &card3) != PLAYER_SUCCESS)
    {
        printf("Failed to receive card3 for Human Player.\n");
    }

    PrintPlayerDetails(humanPlayer);
    printf("\n");

    /* Test 5: Receive Cards for Machine Player */
    printf("=== Test 5: Receive Cards for Machine Player ===\n");
    Card *card4 = CreateSampleCard(KING, DIAMONDS);
    Card *card5 = CreateSampleCard(FOUR, HEARTS);

    if (ReceiveCard(machinePlayer, &card4) != PLAYER_SUCCESS)
    {
        printf("Failed to receive card4 for Machine Player.\n");
    }
    if (ReceiveCard(machinePlayer, &card5) != PLAYER_SUCCESS)
    {
        printf("Failed to receive card5 for Machine Player.\n");
    }

    PrintPlayerDetails(machinePlayer);
    printf("\n");

    /* Test 6: Attempt to Receive NULL Card */
    printf("=== Test 6: Receive NULL Card ===\n");
    if (ReceiveCard(humanPlayer, NULL) != PLAYER_SUCCESS)
    {
        printf("Correctly handled receiving a NULL card.\n");
    }
    else
    {
        printf("Error: Did not handle receiving a NULL card.\n");
    }
    printf("\n");

    /* Test 7: Destroy Players */
    printf("=== Test 7: Destroy Players ===\n");
    DestroyPlayer(&humanPlayer);
    if (humanPlayer == NULL)
    {
        printf("Human Player destroyed successfully.\n");
    }
    else
    {
        printf("Failed to destroy Human Player.\n");
    }

    DestroyPlayer(&machinePlayer);
    if (machinePlayer == NULL)
    {
        printf("Machine Player destroyed successfully.\n");
    }
    else
    {
        printf("Failed to destroy Machine Player.\n");
    }
    printf("\n");

    /* Test 8: Attempt to Destroy NULL Player */
    printf("=== Test 8: Destroy NULL Player ===\n");
    DestroyPlayer(NULL);
    printf("Handled destroying NULL Player without issues.\n");

    /* Test 9: Throw Cards for Human Player */
    printf("=== Test 9: Throw Cards for Human Player ===\n");
    humanPlayer = CreatePlayer(HUMAN, "Alice");
    if (humanPlayer == NULL)
    {
        printf("Failed to create Human Player.\n");
        return EXIT_FAILURE;
    }

    /* Add cards to the player */
    card1 = CreateSampleCard(TWO, HEARTS);
    card2 = CreateSampleCard(ACE, SPADES);
    card3 = CreateSampleCard(TEN, CLUBS);

    ReceiveCard(humanPlayer, &card1);
    ReceiveCard(humanPlayer, &card2);
    ReceiveCard(humanPlayer, &card3);

    PrintPlayerDetails(humanPlayer);
    printf("\n");

    /* Throw cards and validate them */
    void *thrownCard;
    PlayerResult throwRes = ThrowCard(humanPlayer, &thrownCard);
    if (throwRes == PLAYER_SUCCESS && thrownCard != NULL)
    {
        Card *card = (Card *)thrownCard;
        printf("First Thrown Card: Rank %d, Suit %d\n", card->m_rank, card->m_suit);
        free(card);
    }
    else
    {
        printf("Failed to throw the first card.\n");
    }

    throwRes = ThrowCard(humanPlayer, &thrownCard);
    if (throwRes == PLAYER_SUCCESS && thrownCard != NULL)
    {
        Card *card = (Card *)thrownCard;
        printf("Second Thrown Card: Rank %d, Suit %d\n", card->m_rank, card->m_suit);
        free(card);
    }
    else
    {
        printf("Failed to throw the second card.\n");
    }

    throwRes = ThrowCard(humanPlayer, &thrownCard);
    if (throwRes == PLAYER_SUCCESS && thrownCard != NULL)
    {
        Card *card = (Card *)thrownCard;
        printf("Third Thrown Card: Rank %d, Suit %d\n", card->m_rank, card->m_suit);
        free(card);
    }
    else
    {
        printf("Failed to throw the third card.\n");
    }

    /* Attempt to throw a card from an empty hand */
    throwRes = ThrowCard(humanPlayer, &thrownCard);
    if (throwRes == PLAYER_EMPTY_CARDS)
    {
        printf("Correctly handled throwing a card from an empty hand.\n");
    }
    else
    {
        printf("Error: Did not handle empty hand correctly.\n");
    }

    /* Cleanup */
    DestroyPlayer(&humanPlayer);
    printf("\n");


    return EXIT_SUCCESS;
}