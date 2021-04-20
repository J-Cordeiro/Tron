#include <allegro5/allegro.h>

typedef struct
{
    size_t x;
    size_t y;
} TronPosition;

typedef struct
{
    size_t width;
    size_t height;
} TronBoard;

void tron_board_draw(TronBoard *board);
