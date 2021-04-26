// THIS DOCUMENT CONTAINS:
// The menu functions used in the code

#include "menu.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "../board/board.h"
#include "../defs.h"
#include "../player/player.h"

static TronMenu menu;

// Function to load the menu gameplay
static void tron_menu_load_gameplay() {
    tron_player_list_flush(menu.player_list);
    menu.board->gameover = false;
    TronPlayer *player1 = tron_create_player("", TronPlayerBot, TronBottom, menu.board->width / 10, 10);
    tron_player_set_color(player1, COLOR_1);
    tron_player_list_add_player(menu.player_list, player1);

    TronPlayer *player2 = tron_create_player("", TronPlayerBot, TronBottom, menu.board->width - menu.board->width / 10, 10);
    tron_player_set_color(player2, COLOR_2);
    tron_player_list_add_player(menu.player_list, player2);

    TronPlayer *player3 = tron_create_player("", TronPlayerBot, TronTop, menu.board->width / 5, menu.board->height - 10);
    tron_player_set_color(player3, COLOR_3);
    tron_player_list_add_player(menu.player_list, player3);

    TronPlayer *player4 = tron_create_player("", TronPlayerBot, TronTop, menu.board->width - menu.board->width / 5, menu.board->height - 10);
    tron_player_set_color(player4, COLOR_4);
    tron_player_list_add_player(menu.player_list, player4);
}

// Function to unload fonts
static void unload_fonts() {
    if (menu.button_font) {
        al_destroy_font(menu.button_font);
    }
    if (menu.gameover_font) {
        al_destroy_font(menu.gameover_font);
    }

    menu.button_font = NULL;
}

// Function to load fonts
static void load_fonts() {
    unload_fonts();
    menu.button_font = al_load_font(FONT_PATH, BUTTON_FONT_SIZE * menu.board->scale, 0);

    if (!menu.button_font) {
        printf("Couldn't load font %s\n", FONT_PATH);
        exit(-1);
    }
    menu.gameover_font = al_load_font(FONT_PATH, BUTTON_FONT_SIZE * menu.board->scale, 0);

    if (!menu.gameover_font) {
        printf("Couldn't load font %s\n", FONT_PATH);
        exit(-1);
    }
}

// Function to load menu
void tron_menu_load() {
    menu.player_list = tron_create_player_list();
    menu.board = tron_create_board(0, 0, 600, 350);
    menu.board->sound = false;

    menu.logo_bitmap = al_load_bitmap(LOGO_BITMAP_PATH);

    if (!menu.logo_bitmap) {
        printf("Couldn't not load logo menu bitmap.'");
        exit(-1);
    }

    menu.about_bitmap = al_load_bitmap(ABOUT_BITMAP_PATH);

    if (!menu.about_bitmap) {
        printf("Couldn't not load about bitmap.'");
        exit(-1);
    }

    menu.button_bitmap = al_load_bitmap(BUTTON_BITMAP_PATH);

    if (!menu.button_bitmap) {
        printf("Couldn't not load button bitmap.'");
        exit(-1);
    }

    menu.button_single_player_hover = false;
    menu.button_two_player_hover = false;
    menu.button_font = NULL;
    menu.gameover_font = NULL;

    menu.event_queue = al_create_event_queue();
    al_register_event_source(menu.event_queue, al_get_mouse_event_source());

    tron_menu_load_gameplay();
}

// Function to unload menu
void tron_menu_unload() {
    tron_destroy_board(menu.board);
    tron_destroy_player_list(menu.player_list);

    al_destroy_bitmap(menu.logo_bitmap);
    al_destroy_bitmap(menu.button_bitmap);

    al_destroy_font(menu.button_font);
    al_destroy_font(menu.gameover_font);

    al_destroy_event_queue(menu.event_queue);
}

// Function to resize the menu
void tron_menu_resize(int width, int height) {
    menu.display_size.x = width;
    menu.display_size.y = height;

    float board_width_full = (menu.board->width - 2 * RECTANGLE_LINE_THICKNESS);
    float board_height_full = (menu.board->height - 2 * RECTANGLE_LINE_THICKNESS);

    float scale_width = width / board_width_full;
    float scale_height = height / board_height_full;

    if (scale_width > scale_height) {
        menu.board->scale = scale_width;
    } else {
        menu.board->scale = scale_height;
    }

    menu.board->x = ((width - RECTANGLE_LINE_THICKNESS * 2 - (menu.board->scale * board_width_full)) / 2) / menu.board->scale;
    menu.board->y = ((height - RECTANGLE_LINE_THICKNESS * 2 - (menu.board->scale * board_height_full)) / 2) / menu.board->scale;

    //ABOUT message position and size
    menu.about_size.x = (float)width / 4;
    menu.about_size.y = (double)menu.about_size.x / ABOUT_WIDTH * ABOUT_HEIGHT;
    menu.about_position.x = (double)(width - menu.about_size.x - 20);
    menu.about_position.y = (double)(height - menu.about_size.y - 20);

    //LOGO position and size
    menu.logo_size.x = (float)width / 4;
    menu.logo_size.y = (double)menu.logo_size.x / LOGO_WIDTH * LOGO_HEIGHT;
    menu.logo_position.x = (double)(width - menu.logo_size.x) / 2;
    menu.logo_position.y = 10 * menu.board->scale;

    //button position and size
    menu.button_size.x = (float)width / 4;
    menu.button_size.y = (double)menu.button_size.x / BUTTON_WIDTH * BUTTON_HEIGHT;
    menu.button_single_player_position.x = (double)(width - menu.button_size.x) / 2;
    menu.button_two_player_position.x = menu.button_single_player_position.x;

    menu.button_single_player_position.y = (double)(height - menu.button_size.y) / 2;
    menu.button_two_player_position.y = (double)(height - menu.button_size.y + menu.button_size.y - (double)BUTTON_HEIGHT * 0.05) / 2;

    load_fonts();
}

// Function to update button state
void update_button_state(TronPosition *position, bool *state) {
    float start_x = position->x + BUTTON_BOX_DELTA_X * menu.board->scale;
    float start_y = position->y + BUTTON_BOX_DELTA_Y * menu.board->scale;
    float end_x = position->x + (BUTTON_BOX_DELTA_X + BUTTON_BOX_WIDTH) * menu.board->scale;
    float end_y = position->y + (BUTTON_BOX_DELTA_Y + BUTTON_BOX_HEIGHT) * menu.board->scale;
    if (start_x <= menu.mouse.x && menu.mouse.x <= end_x && start_y <= menu.mouse.y && menu.mouse.y <= end_y) {
        *state = true;
    } else {
        *state = false;
    }
}

// Function to update the menu
TronState tron_menu_update() {
    TronState new_state = MainMenu;
    ALLEGRO_EVENT event;
    while (al_get_next_event(menu.event_queue, &event)) {
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            menu.mouse.x = event.mouse.x;
            menu.mouse.y = event.mouse.y;
            update_button_state(&menu.button_single_player_position, &menu.button_single_player_hover);
            update_button_state(&menu.button_two_player_position, &menu.button_two_player_hover);
        }
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button & 1) {
            menu.mouse.x = event.mouse.x;
            menu.mouse.y = event.mouse.y;
            update_button_state(&menu.button_single_player_position, &menu.button_single_player_hover);

            if (menu.button_single_player_hover) {
                new_state = SinglePlayer;
            }

            update_button_state(&menu.button_two_player_position, &menu.button_two_player_hover);

            if (menu.button_two_player_hover) {
                new_state = MultiPlayer;
            }
            al_flush_event_queue(menu.event_queue);
        }
    }
    tron_board_update(menu.board, menu.player_list);
    if (menu.board->gameover) {
        tron_menu_load_gameplay();
    }
    return new_state;
}

// Function to draw a button
static void draw_button(TronPosition *position, char *label, bool hover) {
    al_draw_scaled_bitmap(menu.button_bitmap,
                          0, 0, BUTTON_WIDTH, BUTTON_HEIGHT,
                          position->x,
                          position->y,
                          menu.button_size.x,
                          menu.button_size.y, 0);

    if (hover) {
        al_draw_rectangle(position->x + BUTTON_BOX_DELTA_X * menu.board->scale,
                          position->y + BUTTON_BOX_DELTA_Y * menu.board->scale,
                          position->x + (BUTTON_BOX_DELTA_X + BUTTON_BOX_WIDTH) * menu.board->scale,
                          position->y + (BUTTON_BOX_DELTA_Y + BUTTON_BOX_HEIGHT) * menu.board->scale,
                          al_map_rgb(43, 255, 255), 1 * menu.board->scale);
    }

    al_draw_text(menu.button_font, al_map_rgba(255, 255, 255, 192),
                 position->x + menu.button_size.x / 2,
                 position->y + menu.button_size.y / 2 - menu.board->scale * BUTTON_FONT_SIZE / 2,
                 ALLEGRO_ALIGN_CENTRE, label);
}

// Function to render the menu
void tron_menu_render() {
    tron_board_draw(menu.board);

    TronPlayerListItem *item = menu.player_list->first;

    while (item != NULL) {
        tron_player_draw(item->player, menu.board);
        item = item->next;
    }

    al_draw_filled_rectangle(0, 0, menu.display_size.x, menu.display_size.y, al_map_rgba(0, 0, 0, 128));
    al_draw_scaled_bitmap(menu.logo_bitmap,
                          0, 0, LOGO_WIDTH, LOGO_HEIGHT,
                          menu.logo_position.x,
                          menu.logo_position.y,
                          menu.logo_size.x,
                          menu.logo_size.y, 0);

    al_draw_scaled_bitmap(menu.about_bitmap,
                          0, 0, ABOUT_WIDTH, ABOUT_HEIGHT,
                          menu.about_position.x,
                          menu.about_position.y,
                          menu.about_size.x,
                          menu.about_size.y, 0);

    draw_button(&menu.button_single_player_position, "1 Jogador", menu.button_single_player_hover);
    draw_button(&menu.button_two_player_position, "2 Jogadores", menu.button_two_player_hover);
}
