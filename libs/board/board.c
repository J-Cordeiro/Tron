// THIS DOCUMENT CONTAINS:
// The board functions used in the code

#include "board.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "../defs.h"
#include "../player/player.h"
#include "../utils/utils.h"

static ALLEGRO_SAMPLE *board_gameover_audio = NULL;

// Function to load the tron board
void tron_board_load() {
    board_gameover_audio = al_load_sample(AUDIO_GAMEOVER_PATH);
    if (!board_gameover_audio) {
        printf("Could not load gameover audio.");
        exit(-1);
    }
}

// Function to unload the board
void tron_board_unload() {
    al_destroy_sample(board_gameover_audio);
}

// Function to read player2 keyboard keys
static void tron_player_read_keyboard_keys_2(ALLEGRO_EVENT *event, TronPlayer *player) {
    switch (event->keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            tron_player_set_new_direction(player, TronTop);
            break;
        case ALLEGRO_KEY_DOWN:
            tron_player_set_new_direction(player, TronBottom);
            break;
        case ALLEGRO_KEY_LEFT:
            tron_player_set_new_direction(player, TronLeft);
            break;
        case ALLEGRO_KEY_RIGHT:
            tron_player_set_new_direction(player, TronRight);
    }
}

// Function to read player1 keyboard keys
static void tron_player_read_keyboard_keys_1(ALLEGRO_EVENT *event, TronPlayer *player) {
    switch (event->keyboard.keycode) {
        case ALLEGRO_KEY_W:
            tron_player_set_new_direction(player, TronTop);
            break;
        case ALLEGRO_KEY_S:
            tron_player_set_new_direction(player, TronBottom);
            break;
        case ALLEGRO_KEY_A:
            tron_player_set_new_direction(player, TronLeft);
            break;
        case ALLEGRO_KEY_D:
            tron_player_set_new_direction(player, TronRight);
    }
}

// Function to create a board
TronBoard *tron_create_board(float x, float y, float width, float height) {
    TronBoard *board = (TronBoard *)tron_malloc(sizeof(TronBoard));
    board->x = x;
    board->gameover = false;
    board->y = y;
    board->sound = true;
    board->width = width;
    board->height = height;
    board->scale = BOARD_SCALE;

    board->event_queue = al_create_event_queue();
    board->timer = al_create_timer(1.0 / TPS);

    al_register_event_source(board->event_queue, al_get_keyboard_event_source());
    al_register_event_source(board->event_queue, al_get_timer_event_source(board->timer));

    al_start_timer(board->timer);

    return board;
}

// Function to destroy a board
void tron_destroy_board(TronBoard *board) {
    al_destroy_timer(board->timer);
    al_destroy_event_queue(board->event_queue);
    free(board);
}

// Function to update a board
void tron_board_update(TronBoard *board, TronPlayerList *list) {
    ALLEGRO_EVENT event;
    TronPlayerListItem *item;
    if (board->gameover) return;
    while (al_get_next_event(board->event_queue, &event)) {
        item = list->first;
        while (item != NULL) {
            if (event.type == ALLEGRO_EVENT_TIMER) {
                if (item->player->type == TronPlayerBot) {
                    tron_player_update_bot(item->player, list, board);
                }
            } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (item->player->type == TronPlayerKeyboard1) {
                    tron_player_read_keyboard_keys_1(&event, item->player);
                } else if (item->player->type == TronPlayerKeyboard2) {
                    tron_player_read_keyboard_keys_2(&event, item->player);
                }
            }
            if (event.type == ALLEGRO_EVENT_TIMER) {
                if (item->player->live && !tron_player_check_if_player_lost(item->player, list, board, PLAYER_SPEED)) {
                    tron_player_move(item->player);
                } else {
                    item->player->live = false;
                }
            }
            item = item->next;
        }

        item = list->first;
        int number_of_player_lost = 0;
        while (item != NULL) {
            if (!item->player->live) {
                number_of_player_lost++;
            }
            item = item->next;
        }
        if ((int)list->size - 1 <= number_of_player_lost) {
            board->gameover = true;
            // musica gameOver audio
            if (board->sound)
                al_play_sample(board_gameover_audio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
    }
}

// Function to draw a board
void tron_board_draw(TronBoard *board) {
    al_draw_filled_rectangle(board->x * board->scale, board->y * board->scale, (board->x + board->width) * board->scale, (board->y + board->height) * board->scale, al_map_rgb(51, 51, 51));  // Desenha o retângulo cinza de trás da tela

    for (int i = 1; i <= VERTICAL_LINES; i++)  // Desenha linhas verticais
    {
        al_draw_line((board->x * board->scale) + ((board->width * board->scale) / VERTICAL_LINES) * i, (board->y * board->scale),
                     (board->x * board->scale) + ((board->width * board->scale) / VERTICAL_LINES) * i,
                     (board->y + board->height) * board->scale,
                     al_map_rgb(35, 35, 35),
                     (RECTANGLE_LINE_THICKNESS * board->scale) / 2);
    }

    for (int i = 1; i < HORIZONTAL_LINES; i++)  // Desenha linhas horizontais
    {
        al_draw_line((board->x * board->scale), (board->y * board->scale) + ((board->height * board->scale) / HORIZONTAL_LINES) * i,
                     (board->x + board->width) * board->scale,
                     (board->y * board->scale) +
                         ((board->height * board->scale) / HORIZONTAL_LINES) * i,
                     al_map_rgb(35, 35, 35),
                     (RECTANGLE_LINE_THICKNESS * board->scale) / 2);
    }

    al_draw_rectangle(board->x * board->scale, board->y * board->scale,
                      (board->x + board->width) * board->scale,
                      (board->y + board->height) * board->scale,
                      al_map_rgb(1, 33, 51),
                      RECTANGLE_LINE_THICKNESS * board->scale);  // Desenha o retângulo azul de trás da tela
}
