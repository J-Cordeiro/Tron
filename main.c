//	IFSC - Campus Florianópolis
//
// Curso Técnico Integrado em Eletrônica 7º Fase
//
//	Projeto de Programação em C
//
//	Alunos: Geovane Martins Schmitz
//			João Pedro Schmidt Cordeiro
//
//  TRON O LEGADO
//
//////////////////////////////////////////////////////

#include <stdio.h>
#include <time.h>

// Include allegro libraries
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

// Include Tron libraries
#include "libs/board/board.h"
#include "libs/defs.h"
#include "libs/menu/menu.h"
#include "libs/player/player.h"

// Display size variable
int current_width = DEFAULT_WIDTH;
int current_height = DEFAULT_WIDTH;

// Font variable
ALLEGRO_FONT *font_gameover = NULL;

// Audio variables
static ALLEGRO_SAMPLE *board_game_audio = NULL;
static ALLEGRO_SAMPLE_ID board_game_audio_id;
static ALLEGRO_SAMPLE *menu_audio = NULL;
static ALLEGRO_SAMPLE_ID menu_audio_id;

// Bitmap variable
static ALLEGRO_BITMAP *
    player_bitmap_controls;

// Function to render the intro gameplay
void gameplay_render(TronBoard *board, TronPlayerList *player_list) {
    al_clear_to_color(al_map_rgb(20, 69, 98));
    bool is_win = false;
    tron_board_draw(board);
    TronPlayerListItem *item = player_list->first;
    while (item != NULL) {
        tron_player_draw(item->player, board);
        if (!item->player->live) {
            if (item->player->type == TronPlayerBot) {
                is_win = true;
            } else {
                is_win = false;
            }
        }

        if (!board->gameover && item->player->type == TronPlayerKeyboard1) {
            al_draw_tinted_scaled_bitmap(player_bitmap_controls, al_map_rgba(128, 128, 128, 1),
                                         0, 0, PLAYER_CONTROLS_WIDTH / 2, PLAYER_CONTROLS_HEIGHT,
                                         (RECTANGLE_LINE_THICKNESS + board->x) * board->scale, (RECTANGLE_LINE_THICKNESS + board->y) * board->scale,
                                         150 * board->scale,
                                         (150 * board->scale) / (PLAYER_CONTROLS_WIDTH / 2) * PLAYER_CONTROLS_HEIGHT, 0);
        }
        if (!board->gameover && item->player->type == TronPlayerKeyboard2) {
            al_draw_tinted_scaled_bitmap(player_bitmap_controls, al_map_rgba(128, 128, 128, 1),
                                         PLAYER_CONTROLS_WIDTH / 2, 0, PLAYER_CONTROLS_WIDTH / 2, PLAYER_CONTROLS_HEIGHT,
                                         (board->width + board->x - 150 - RECTANGLE_LINE_THICKNESS) * board->scale, (RECTANGLE_LINE_THICKNESS + board->y) * board->scale,
                                         150 * board->scale,
                                         (150 * board->scale) / (PLAYER_CONTROLS_WIDTH / 2) * PLAYER_CONTROLS_HEIGHT, 0);
        }
        item = item->next;
    }
    if (board->gameover) {
        al_draw_filled_rectangle(0, 0, current_width, current_height, al_map_rgba(0, 0, 0, 128));
        if (is_win) {
            al_draw_text(font_gameover, al_map_rgba(255, 255, 255, 192),
                         current_width / 2, current_height / 2,
                         ALLEGRO_ALIGN_CENTRE, "WIN");
        } else {
            al_draw_text(font_gameover, al_map_rgba(255, 255, 255, 192),
                         current_width / 2, current_height / 2,
                         ALLEGRO_ALIGN_CENTRE, "GAME OVER");
        }
    }
}

// Function to risize the board
void gameplay_resize(TronBoard *board, int width, int height) {
    if (width / board->width < height / board->height) {
        board->scale = width / (board->width + 2 * RECTANGLE_LINE_THICKNESS);
        board->y = (height - board->height * board->scale) / 2 - RECTANGLE_LINE_THICKNESS;
        board->x = RECTANGLE_LINE_THICKNESS;
    } else {
        board->scale = height / (board->height + 2 * RECTANGLE_LINE_THICKNESS);
        board->x = (width - board->width * board->scale) / 2;
        board->y = RECTANGLE_LINE_THICKNESS;
    }
}

// Function to create a game
void create_player_game(TronBoard *board, TronPlayerList *player_list, TronPlayerType second_player_type) {
    // musica game audio
    al_stop_sample(&menu_audio_id);
    al_stop_sample(&board_game_audio_id);

    al_play_sample(board_game_audio, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &board_game_audio_id);

    tron_player_list_flush(player_list);

    TronPlayer *player1 = tron_create_player("", second_player_type, TronTop, board->width - board->width / 10, board->height - 10);
    tron_player_set_color(player1, COLOR_1);
    tron_player_list_add_player(player_list, player1);

    TronPlayer *player2 = tron_create_player("", TronPlayerKeyboard1, TronBottom, board->width / 10, 10);
    tron_player_set_color(player2, COLOR_2);
    tron_player_list_add_player(player_list, player2);

    board->gameover = false;
    al_flush_event_queue(board->event_queue);
}

// MAIN FUNCTION
int main(void) {
    // Variables
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    TronState state = MainMenu;
    TronState new_state = MainMenu;
    bool fullscreen_window = false;
    bool first_paint = true;

    srand(time(NULL));

    // Error messages
    if (!al_init()) {
        printf("Couldn't initialize allegro.\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        printf("Couldn't initialize primitive addon.\n");
        return -1;
    }

    if (!al_init_font_addon()) {
        printf("Couldn't initialize allegro font library.\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        printf("Couldn't initialize allegro acodec library.\n");
        return -1;
    }

    if (!al_init_ttf_addon()) {
        printf("Couldn't initialize allegro ttf library.\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        printf("Couldn't initialize allegro image library.\n");
        return -1;
    }

    if (!al_install_mouse()) {
        printf("Couldn't initialize mouse driver.\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        printf("Couldn't initialize keyboard driver.\n");
        return -1;
    }

    if (!al_install_audio()) {
        printf("Couldn't initialize keyboard driver.\n");
        return -1;
    }

    font_gameover = al_load_font(FONT_PATH, GAMEOVER_FONT_SIZE, 0);
    if (font_gameover == NULL) {
        printf("Couldn't load font %s\n", FONT_PATH);
        return -1;
    }

    player_bitmap_controls = al_load_bitmap(PLAYER_CONTROLS_BITMAP_PATH);
    if (!player_bitmap_controls) {
        printf("Could not load player controls bitmap.\n");
        exit(-1);
    }

    // Load functions
    tron_player_load();
    tron_board_load();
    tron_menu_load();

    al_reserve_samples(10);

    // Set functions
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_display_flags(ALLEGRO_RESIZABLE);

    display = al_create_display(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    if (!display) {
        printf("Couldn't initialize display.\n");
        return -1;
    }

    event_queue = al_create_event_queue();

    TronBoard *board = tron_create_board(0, 0, 1200, 600);
    TronPlayerList *player_list = tron_create_player_list();

    timer = al_create_timer(1.0 / FPS);
    al_register_event_source(event_queue, al_get_keyboard_event_source());        // Registrou o get_keyboard na fila de
                                                                                  // eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));  // Registrou o get_display na fila de eventos
    al_register_event_source(event_queue, al_get_timer_event_source(timer));      // Registrar o get_timer na fila de eventos
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);  // Começar o timer
    menu_audio = al_load_sample(AUDIO_MENU_SOUNDTRACK_PATH);
    if (!menu_audio) {
        printf("Could not load menu audio.\n");
        exit(-1);
    }

    al_play_sample(menu_audio, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &menu_audio_id);

    board_game_audio = al_load_sample(AUDIO_GAME_SOUNDTRACK_PATH);
    if (!board_game_audio) {
        printf("Could not load game audio.\n");
        exit(-1);
    }

    // Allegro loop
    while (true) {
        ALLEGRO_EVENT event;

        if (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE || first_paint) {
                first_paint = false;
                al_acknowledge_resize(display);
                current_width = al_get_display_width(display);
                current_height = al_get_display_height(display);
                tron_menu_resize(current_width, current_height);
                gameplay_resize(board, current_width, current_height);
            }

            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button & 1) {
                if (state != MainMenu && board->gameover) {
                    state = MainMenu;
                    new_state = MainMenu;
                }
            }

            if (event.type == ALLEGRO_EVENT_TIMER) {
                if (al_is_event_queue_empty(event_queue)) {
                    if (state == MainMenu) {
                        new_state = tron_menu_update();
                        if (new_state == SinglePlayer) {
                            create_player_game(board, player_list, TronPlayerBot);
                        } else if (new_state == MultiPlayer) {
                            create_player_game(board, player_list, TronPlayerKeyboard2);
                        }
                        state = new_state;
                    }
                    if (state != MainMenu) {
                        tron_board_update(board, player_list);
                        gameplay_render(board, player_list);

                    } else {
                        tron_menu_render();
                    }
                    al_flip_display();
                }
            }

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }

            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_F11) {
                fullscreen_window = !fullscreen_window;
                al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen_window);
            }
        }
    }

    // Unload and destroy functions
    tron_board_unload();
    tron_player_unload();
    tron_menu_unload();
    al_destroy_sample(board_game_audio);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}