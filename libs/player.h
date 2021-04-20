#include <allegro5/allegro.h>
#include "board.h"

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
    VertList *first_position;
} TronPlayer;

TronPlayer *tron_player_constructor();     //
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