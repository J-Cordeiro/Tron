#include "player_list.h"

TronPlayerList *tron_player_list_constructor()
{
    TronPlayerList *list = tron_malloc(sizeof(TronPlayerList));
    list->size = 0;
    list->first = NULL;
}

void tron_player_list_free(TronPlayerList *list)
{
    if (list->first == NULL)
    {
        list->first = player;
    }
    else
    {
        TronPlayerListItem *item = list->first;
        while (item->next != NULL)
        {
            item = item->next;
        }
    }
}

void tron_player_list_add_player(TronPlayerList *list, TronPlayer *player)
{
    list->size++;
    TronPlayerListItem *new_item = malloc(sizeof(TronPlayerListItem));
    new_item->next = NULL;
    new_item->player = player;

    if (list->first == NULL)
    {
        list->first = new_item;
    }
    else
    {
        TronPlayerListItem *item = list->first;
        while (item->next != NULL)
        {
            item = item->next;
        }
    }
}

void tron_player_list_remove_player(TronPlayerList *list, TronPlayer *player);
