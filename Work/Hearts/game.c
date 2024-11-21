#include "game.h"
#include "player.h"
#include "round.h"
#include <stdint.h> /* SIZE_MAX */
#include <stdlib.h> /* calloc, malloc, free, size_t */
#include <string.h> /* strcmp */

#include <stdio.h> /* TEMP */

struct Game
{
    Player **m_players;
    Round *m_round;
    size_t *m_scores;
    size_t m_numOfPlayers;
};

char *PlayerGetName(Player *_player);

static void DestroyPlayers(Game *_game);
static PlayerResult CreatePlayers(Game *_game, size_t _numOfComputerPlayers, size_t _numOfHumanPlayers, char **_playerNames);
static size_t FindPlayerIndex(Game *_game, char *_name);
static Game *AllocateMemoryGame(size_t _numOfHumanPlayers, size_t _numOfComputerPlayers, char **_playerNames);
static GameResult StatusConverter(RoundResult _status);

/* API Functions */

Game *CreateGame(size_t _numOfHumanPlayers, size_t _numOfComputerPlayers, char **_playerNames)
{
    size_t totalNumOfPlayers = _numOfHumanPlayers + _numOfComputerPlayers;

    if (totalNumOfPlayers == 0 || _playerNames == NULL)
    {
        return NULL;
    }

    Game *newGame = AllocateMemoryGame(_numOfHumanPlayers, _numOfComputerPlayers, _playerNames);
    if (newGame == NULL)
    {
        return NULL;
    }

    return newGame;
}

GameResult PlayGame(Game *_game)
{
    size_t winnerIndex;
    size_t roundNum;
    RoundResult playResult;
    if (_game == NULL)
    {
        return GAME_UNINITIALIZED_ERROR;
    }

    winnerIndex = _game->m_numOfPlayers;
    roundNum = 0;
    while (winnerIndex == _game->m_numOfPlayers)
    {
        playResult = Play(_game->m_round, roundNum, &winnerIndex);
        if (playResult != ROUND_SUCCESS)
        {
            return StatusConverter(playResult);
        }
        roundNum = (roundNum + 1) % _game->m_numOfPlayers;
    }
    printf("Winner is: %s !\n", PlayerGetName(_game->m_players[winnerIndex]));
    printf("Scores:\n%s with score of %ld\n%s with score of %ld\n", PlayerGetName(_game->m_players[0]), _game->m_scores[0], PlayerGetName(_game->m_players[1]), _game->m_scores[1]);
    return GAME_SUCCESS;
}

void DestroyGame(Game **_game)
{
    if (_game != NULL && *_game != NULL)
    {
        DestroyPlayers(*_game);
        DestroyRound(&((*_game)->m_round));
        if ((*_game)->m_scores != NULL)
        {
            free((*_game)->m_scores);
        }
        free(*_game);
        *_game = NULL;
    }
}

/* Static Functions */

static void DestroyPlayers(Game *_game)
{
    if (_game->m_players != NULL)
    {
        for (size_t i = 0; i < _game->m_numOfPlayers; i++)
        {
            DestroyPlayer(&_game->m_players[i]);
        }
        free(_game->m_players);
    }
}

static PlayerResult CreatePlayers(Game *_game, size_t _numOfComputerPlayers, size_t _numOfHumanPlayers, char **_playerNames)
{
    size_t playerNum = 0;
    for (; playerNum < _numOfComputerPlayers; playerNum++)
    {
        _game->m_players[playerNum] = CreatePlayer(MACHINE, _playerNames[playerNum]);
        if (_game->m_players[playerNum] == NULL)
        {
            return PLAYER_ALLOCATION_ERROR;
        }
    }

    for (; playerNum < _numOfComputerPlayers + _numOfHumanPlayers; playerNum++)
    {
        _game->m_players[playerNum] = CreatePlayer(HUMAN, _playerNames[playerNum]);
        if (_game->m_players[playerNum] == NULL)
        {
            return PLAYER_ALLOCATION_ERROR;
        }
    }
    return PLAYER_SUCCESS;
}

static size_t FindPlayerIndex(Game *_game, char *_name)
{
    size_t index;
    char *playerName;
    for (index = 0; index < _game->m_numOfPlayers; index++)
    {
        playerName = PlayerGetName((_game->m_players[index]));
        if (strcmp(playerName, _name) == 0)
        {
            break;
        }
    }
    return index;
}

static Game *AllocateMemoryGame(size_t _numOfHumanPlayers, size_t _numOfComputerPlayers, char **_playerNames)
{
    size_t totalNumOfPlayers;
    Game *newGame = calloc(1, sizeof(Game));
    if (newGame == NULL)
    {
        return NULL;
    }

    totalNumOfPlayers = _numOfHumanPlayers + _numOfComputerPlayers;
    newGame->m_numOfPlayers = totalNumOfPlayers;

    newGame->m_players = calloc(totalNumOfPlayers, sizeof(Player *));
    if (newGame->m_players == NULL)
    {
        DestroyGame(&newGame);
        return NULL;
    }

    newGame->m_scores = calloc(totalNumOfPlayers, sizeof(size_t));
    if (newGame->m_scores == NULL)
    {
        DestroyGame(&newGame);
        return NULL;
    }

    if (CreatePlayers(newGame, _numOfComputerPlayers, _numOfHumanPlayers, _playerNames) != PLAYER_SUCCESS)
    {
        DestroyGame(&newGame);
        return NULL;
    }

    newGame->m_round = CreateRound(newGame->m_players, totalNumOfPlayers, newGame->m_scores);
    if (newGame->m_round == NULL)
    {
        DestroyGame(&newGame);
        return NULL;
    }

    return newGame;
}

static GameResult StatusConverter(RoundResult _status)
{
    GameResult result = GAME_UNKNOWN_ERROR;
    switch (_status)
    {
    case ROUND_ALLOCATION_ERROR:
        result = GAME_ALLOCATION_ERROR;
        break;
    case ROUND_UNINITIALIZED_ERROR:
        result = GAME_UNINITIALIZED_ERROR;
        break;
    default:
        break;
    }
    return result;
}