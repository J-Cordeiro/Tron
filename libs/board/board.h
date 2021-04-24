#include <allegro5/allegro.h>

#ifndef __TRON_BOARD_H__
#define __TRON_BOARD_H__

typedef struct
{
    size_t width;
    size_t height;
} TronBoard;

void tron_board_draw(TronBoard *board);
#endif