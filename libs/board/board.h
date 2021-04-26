// THIS DOCUMENT CONTAINS: 
// The board structs used in the code

#include <allegro5/allegro.h>
#ifndef __TRON_BOARD_H__
#define __TRON_BOARD_H__
typedef struct TronPlayerList TronPlayerList;

typedef struct {
    float x;
    float y;

    bool gameover;
    bool sound;

    float scale;
    float width;
    float height;

    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;

} TronBoard;

TronBoard *tron_create_board(float x, float y, float width, float height);
void tron_destroy_board(TronBoard *board);

void tron_board_load();
void tron_board_unload();

void tron_board_update(TronBoard *board, TronPlayerList *list);

void tron_board_draw(TronBoard *board);
#endif