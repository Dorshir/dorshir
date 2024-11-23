#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "card.h"

typedef struct Player Player;
typedef int (*RulesFunction)(Card *_card, Card **_table, void *_context);
typedef Card *(*StrategyFunction)(Card **_cards, Card **_table, void *_context);
typedef void(*PrintCardFunc)(Card *_card);


typedef enum Player_Result
{
    PLAYER_SUCCESS,
    PLAYER_ALLOCATION_ERROR,
    PLAYER_UNINITIALIZED,
    PLAYER_REC_CRAD_FAILED,
    PLAYER_EMPTY_CARDS,
    PLAYER_THROW_CARD_FAILED,
    PLAYER_CARD_NOT_FOUND

} PlayerResult;

typedef enum PlayerType
{
    MACHINE,
    HUMAN
} PlayerType;

Player *CreatePlayer(PlayerType _type, char *_name);

void DestroyPlayer(Player **_player);

PlayerResult ReceiveCard(Player *_player, Card **_card);

PlayerResult ThrowCard(Player *_player, Card **_pValue, Card **_table,PrintCardFunc _printFunc, RulesFunction _rulesFunc, StrategyFunction _strategyFunc, void *_rulesContext, void *_strategyContext);

int FindCard(Player *_player, Card *_desiredCard);

const char *PlayerGetName(const Player *_player);

#endif /* __PLAYER_H__ */