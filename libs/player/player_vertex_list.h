// THIS DOCUMENT CONTAINS: 
// The player vertex list structs used in the code

#ifndef __TRON_PLAYER_VERTEX_LIST_H__
#define __TRON_PLAYER_VERTEX_LIST_H__

typedef struct TronPosition {
    float x;
    float y;
} TronPosition;

typedef struct TronPlayerVertexList {
    TronPosition position;
    struct TronPlayerVertexList *next;
    struct TronPlayerVertexList **last;
} TronPlayerVertexList;

TronPlayerVertexList *tron_create_player_vertex_list(float x, float y);

void tron_player_vertex_list_destroy(TronPlayerVertexList *list);

void tron_player_vertex_add(TronPlayerVertexList *list, float x, float y);

#endif