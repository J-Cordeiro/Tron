// THIS DOCUMENT CONTAINS: 
// The player vertex list functions used in the code

#include "player_vertex_list.h"

#include "../utils/utils.h"

// Function to create a plaer vertex list
TronPlayerVertexList *tron_create_player_vertex_list(float x, float y) {
    TronPlayerVertexList *list = tron_malloc(sizeof(TronPlayerVertexList));
    TronPlayerVertexList **last = tron_malloc(sizeof(TronPlayerVertexList *));
    (*last) = list;
    list->position.x = x;
    list->position.y = y;
    list->next = NULL;
    list->last = last;
    return list;
}

// Function to add a vertex to a vertex list
void tron_player_vertex_add(TronPlayerVertexList *list, float x, float y) {
    TronPlayerVertexList *next = tron_malloc(sizeof(TronPlayerVertexList));
    next->position.x = x;
    next->position.y = y;
    next->next = NULL;
    next->last = list->last;
    (*list->last)->next = next;
    (*list->last) = next;
}

// Function to destroy a vertex list
void tron_player_vertex_list_destroy(TronPlayerVertexList *list) {
    TronPlayerVertexList *next;
    free(list->last);
    while (list != NULL) {
        next = list->next;
        free(list);
        list = next;
    }
}