#include <allegro5/allegro.h>

#ifndef __TRON_PLAYER_H__
#define __TRON_PLAYER_H__

#include "../player/player.h"
#include "player_vertex_list.h"

/**
 * @brief Enumerar as direções
 * @enum
 */
typedef enum TronDirection
{
    TronLeft,
    TronRight,
    TronTop,
    TronBotton,
} TronDirection;

/**
 * @brief 
 */
typedef struct
{
    char *name;
    int score;
    int color;
    TronDirection current_direction;
    TronPosition current_position;
    TronPlayerVertexList *first_position;
} TronPlayer;

TronPlayer *tron_player_constructor(char *name, int color);
/**
 * @brief Free player allocation.
 * 
 * @param player 
 */
void tron_player_free(TronPlayer *player);

/**
 * @brief Desenha na tela o jogador.
 * @param player O jogador a ser desenhado.
 */
void tron_player_draw(TronPlayer *player);

void tron_player_set_current_position(TronPlayer *player, size_t x, size_t y);

void tron_player_move_player(TronPlayer *player, TronDirection new_direction);
#endif