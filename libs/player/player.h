// THIS DOCUMENT CONTAINS:
// The player structures used in the code

#include <allegro5/allegro.h>

#ifndef __TRON_PLAYER_H__
#define __TRON_PLAYER_H__

#include "../board/board.h"
#include "player_vertex_list.h"

/**
 * @brief Enumerar as direções
 * @enum
 */
typedef enum TronDirection {
    TronLeft,
    TronRight,
    TronTop,
    TronBottom,
} TronDirection;

typedef enum TronPlayerType {
    TronPlayerKeyboard1,
    TronPlayerKeyboard2,
    TronPlayerBot
} TronPlayerType;

/**
 * Player struct
 */
typedef struct {
    char *name;
    int score;
    ALLEGRO_COLOR color;
    bool live;
    TronDirection current_direction;
    TronPosition current_position;
    TronPlayerVertexList *first_position;
    TronPlayerType type;
} TronPlayer;

/**
 * Estruta para criar a lista "linkada"
 */
typedef struct TronPlayerListItem {
    TronPlayer *player;
    struct TronPlayerListItem *next;
} TronPlayerListItem;

/**
 * Estruta lista player
 */
typedef struct TronPlayerList {
    //Quantidade de jogadores na lista.
    size_t size;
    //Primeiro jogador da lista.
    TronPlayerListItem *first;
} TronPlayerList;

void tron_player_load();

TronPlayer *tron_create_player(char *name, TronPlayerType type, TronDirection direction, float x, float y);
/**
 * @brief Free player allocation.
 *
 * @param player
 */
void tron_destroy_player(TronPlayer *player);

// Function to set player score
void tron_player_set_score(TronPlayer *player, int score);

// Function to check the rectangles collision
// Verfica em todos os traços do jogodor se jogador passou pelo retângulo (point_1 e point_2)
bool tron_player_check_collision_at_rect(TronPlayer *player, TronPosition *point_1, TronPosition *point_2);

// Function that verifies if the bot has to turn
void tron_player_update_bot(TronPlayer *player, TronPlayerList *list, TronBoard *board);

// Function to set player's new direction
void tron_player_set_new_direction(TronPlayer *player, TronDirection new_direction);

// Function to set player's new color.
void tron_player_set_color(TronPlayer *player, ALLEGRO_COLOR color);

// Desenha na tela o jogador.
void tron_player_draw(TronPlayer *player, TronBoard *board);

// Function to check if a player lost
bool tron_player_check_if_player_lost(TronPlayer *player, TronPlayerList *list, TronBoard *board, float size_check);

void tron_player_move(TronPlayer *player);

// Function to unload player assets
void tron_player_unload();
// Create a empty player list
TronPlayerList *tron_create_player_list();

void tron_destroy_player_list(TronPlayerList *list);

void tron_player_list_flush(TronPlayerList *list);

void tron_player_list_add_player(TronPlayerList *list, TronPlayer *player);

#endif