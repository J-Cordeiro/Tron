#include <allegro5/allegro.h>
#include "player.h"
#include <stdlib.h>
#include <string.h>

TronPlayer *tron_player_constructor(char *name, int color)
{
    TronPlayer *player = malloc(sizeof(TronPlayer));
    strcpy(player->name, name);
    player->score = 0;
    player->color = color;
    player->current_direction = TronDirection.TronTop;
    player->current_position.x = 0;
    player->current_position.y = 0;
    player->first_position = NULL;
}

void tron_player_set_current_position(TronPlayer *player, size_t x, size_t y)
{
    player->x = x;
    player->y = y;
}

void tron_player_move_player(TronPlayer *player, TronDirection new_direction)
{
    if (player->current_direction != new_direction)
    {
        player->current_direction = new_direction;
        }
}

void tron_player_free(TronPlayer *player)
{
    free(player->name);
    free(player);
}

void tron_player_draw(TronPlayer *player)
{
}