// THIS DOCUMENT CONTAINS: 
// The menu structs used in the code

#ifndef __TRON_MENU_H__
#define __TRON_MENU_H__

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "../board/board.h"
#include "../defs.h"
#include "../player/player.h"
#include "../player/player_vertex_list.h"

typedef enum TronState {
    MainMenu,
    SinglePlayer,
    MultiPlayer
} TronState;

typedef struct {
    TronPosition display_size;
    ALLEGRO_EVENT_QUEUE *event_queue;

    ALLEGRO_BITMAP *logo_bitmap;
    ALLEGRO_BITMAP *button_bitmap;

    ALLEGRO_FONT *button_font;
    ALLEGRO_FONT *gameover_font;

    TronPosition logo_position;
    TronPosition logo_size;

    TronPosition button_size;
    TronPosition button_single_player_position;
    TronPosition button_two_player_position;
    bool button_single_player_hover;
    bool button_two_player_hover;

    TronBoard *board;
    TronPlayerList *player_list;

    TronPosition mouse;

} TronMenu;

void tron_menu_load();

void tron_menu_unload();

void tron_menu_render();

TronState tron_menu_update();

void tron_menu_resize(int width, int height);

#endif
