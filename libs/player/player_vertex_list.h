#ifndef __TRON_PLAYER_VERTEX_LIST_H__
#define __TRON_PLAYER_VERTEX_LIST_H__

typedef struct TronPosition
{
    size_t x;
    size_t y;
} TronPosition;

typedef struct TronPlayerVertexList
{
    TronPosition position;
    struct TronPlayerVertexList *next;
    struct TronPlayerVertexList **last;
} TronPlayerVertexList;

TronPlayerVertexList *tron_player_vertex_list_constructor();

void tron_player_vertex_list_free(TronPlayerVertexList *list);

void tron_player_vertex_add(TronPlayerVertexList *list, size_t x, size_t y);

#endif