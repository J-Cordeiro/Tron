#include "player_vertex_list.h"

#include "../utils/utils.h"

TronPlayerVertexList *tron_player_vertex_list_constructor()
{
    TronPlayerVertexList *list = tron_malloc(sizeof(TronPlayerVertexList));
    TronPlayerVertexList **last = tron_malloc(sizeof(TronPlayerVertexList *));
    (*last) = NULL;
    list->position.x = 0;
    list->position.y = 0;
    list->next = NULL;
    list->last = last;
    return list;
}

void tron_player_vertex_add(TronPlayerVertexList *list, size_t x, size_t y)
{
    TronPlayerVertexList *next = tron_malloc(sizeof(TronPlayerVertexList));
    next->position.x = x;
    next->position.y = y;
    next->next = NULL;
    next->last = list->last;
    if (*list->last != NULL)
    {
        (*list->last)->next = next;
    }
    *list->last = next;
}

void tron_player_vertex_list_free(TronPlayerVertexList *list)
{
    TronPlayerVertexList *next;
    free(list->last);
    while (list != NULL)
    {
        next = list->next;
        free(list);
        list = next;
    }
}