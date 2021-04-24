#ifndef __TRON_PLAYER_LIST_H__
#define __TRON_PLAYER_LIST_H__
#include "player.h"

typedef struct TronPlayerListItem
{
    TronPlayer *player;
    struct TronPlayerListItem *next;
} TronPlayerListItem;

typedef struct TronPlayerList
{
    size_t size;
    TronPlayerListItem *first;
} TronPlayerList;

TronPlayerList *tron_player_list_constructor();

void tron_player_list_free(TronPlayerList *list);

void tron_player_list_add_player(TronPlayerList *list, TronPlayer *player);

void tron_player_list_remove_player(TronPlayerList *list, TronPlayer *player);

#endif