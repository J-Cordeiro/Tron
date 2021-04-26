// THIS DOCUMENT CONTAINS: 
// The player functions used in the code

#include "player.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../board/board.h"
#include "../defs.h"
#include "../utils/utils.h"

// Bitmap variables
static ALLEGRO_BITMAP *player_bitmap_vertical;
static ALLEGRO_BITMAP *player_bitmap_horizontal;
static ALLEGRO_BITMAP *player_bitmap_corner_top_left;
static ALLEGRO_BITMAP *player_bitmap_corner_top_right;
static ALLEGRO_BITMAP *player_bitmap_corner_bottom_left;
static ALLEGRO_BITMAP *player_bitmap_corner_bottom_right;

// Audio variables
static ALLEGRO_SAMPLE *player_audio_turn;

// Function to load player assets
void tron_player_load() {
    player_bitmap_vertical = al_load_bitmap(PLAYER_VERTICAL_BITMAP_PATH);
    if (!player_bitmap_vertical) {
        printf("Could not load player vertical bitmap.\n");
        exit(-1);
    }

    player_bitmap_horizontal = al_load_bitmap(PLAYER_HORIZONTAL_BITMAP_PATH);

    if (!player_bitmap_horizontal) {
        printf("Could not load player horizontal bitmap.\n");
        exit(-1);
    }

    player_bitmap_corner_top_left = al_load_bitmap(PLAYER_CORNER_TOP_LEFT_BITMAP_PATH);
    player_bitmap_corner_top_right = al_load_bitmap(PLAYER_CORNER_TOP_RIGHT_BITMAP_PATH);
    player_bitmap_corner_bottom_left = al_load_bitmap(PLAYER_CORNER_BOTTOM_LEFT_BITMAP_PATH);
    player_bitmap_corner_bottom_right = al_load_bitmap(PLAYER_CORNER_BOTTOM_RIGHT_BITMAP_PATH);

    if (!player_bitmap_corner_top_left ||
        !player_bitmap_corner_top_right ||
        !player_bitmap_corner_bottom_left ||
        !player_bitmap_corner_bottom_right) {
        printf("Could not load player corner bitmap.\n");
        exit(-1);
    }

    player_audio_turn = al_load_sample(AUDIO_PLAYER_TURN_PATH);
    if (!player_audio_turn) {
        printf("Could not load playerTurn audio.\n");
        exit(-1);
    }
}

// Function to unload player assets
void tron_player_unload() {
    al_destroy_bitmap(player_bitmap_vertical);
    al_destroy_bitmap(player_bitmap_horizontal);
    al_destroy_bitmap(player_bitmap_corner_top_left);
    al_destroy_bitmap(player_bitmap_corner_top_right);
    al_destroy_bitmap(player_bitmap_corner_bottom_left);
    al_destroy_bitmap(player_bitmap_corner_bottom_right);
    al_destroy_sample(player_audio_turn);
}

// Function to create a player
TronPlayer *tron_create_player(char *name, TronPlayerType type, TronDirection direction, float x, float y) {
    TronPlayer *player = (TronPlayer *)tron_malloc(sizeof(TronPlayer));
    player->name = tron_calloc(strlen(name) + 1, sizeof(char));
    strcpy(player->name, name);
    player->score = 0;
    player->live = true;
    player->type = type;
    player->color = al_map_rgb(255, 0, 0);
    player->current_direction = direction;
    player->current_position.x = x;
    player->current_position.y = y;
    player->first_position = tron_create_player_vertex_list(x, y);
    return player;
}

// Function to set player color
void tron_player_set_color(TronPlayer *player, ALLEGRO_COLOR color) {
    player->color = color;
}

// Function to set player score
void tron_player_set_score(TronPlayer *player, int score) {
    player->score = score;
}

// Function to transform the player line to a rectangle
static void tron_line_to_retangle(TronPosition *point_result_1, TronPosition *point_result_2, TronPosition *line_point_1, TronPosition *line_point_2, float half_thickness) {
    point_result_1->x = line_point_1->x;
    point_result_1->y = line_point_1->y;
    point_result_2->x = line_point_2->x;
    point_result_2->y = line_point_2->y;

    if (line_point_1->x == line_point_2->x) {
        point_result_1->x -= half_thickness;
        point_result_2->x += half_thickness;
    } else {
        point_result_1->y -= half_thickness;
        point_result_2->y += half_thickness;
    }
}

// Function to move the player
void tron_player_move(TronPlayer *player) {
    float player_half_thickness = (float)PLAYER_THICKNESS / 2;
    switch (player->current_direction) {
        case TronTop:
            player->current_position.y -= PLAYER_SPEED + player_half_thickness;
            break;
        case TronRight:
            player->current_position.x += PLAYER_SPEED + player_half_thickness;
            break;
        case TronBottom:
            player->current_position.y += PLAYER_SPEED + player_half_thickness;
            break;
        case TronLeft:
            player->current_position.x -= PLAYER_SPEED + player_half_thickness;
    }
}

// Function to check if a player lost
bool tron_player_check_if_player_lost(TronPlayer *player, TronPlayerList *list, TronBoard *board, float size_check) {
    float player_half_thickness = (float)PLAYER_THICKNESS / 2;
    TronPosition pos_1, pos_2, rect_pos_1, rect_pos_2;
    pos_2.x = pos_1.x = player->current_position.x;
    pos_2.y = pos_1.y = player->current_position.y;

    switch (player->current_direction) {
        case TronTop:
            pos_1.y -= player_half_thickness;
            pos_2.y -= size_check + player_half_thickness;
            break;
        case TronRight:
            pos_1.x += player_half_thickness;
            pos_2.x += size_check + player_half_thickness;
            break;
        case TronBottom:
            pos_1.y += player_half_thickness;
            pos_2.y += size_check + player_half_thickness;
            break;
        case TronLeft:
            pos_1.x -= player_half_thickness;
            pos_2.x -= size_check + player_half_thickness;
    }
    tron_line_to_retangle(&rect_pos_1, &rect_pos_2, &pos_1, &pos_2, player_half_thickness - 1);
    TronPlayerListItem *item = list->first;
    float half_thickness =
        RECTANGLE_LINE_THICKNESS / 2 + player_half_thickness - 1;
    if (pos_2.x <= half_thickness ||
        pos_2.x >= board->width - half_thickness ||
        pos_2.y <= half_thickness ||
        pos_2.y >= board->height - half_thickness) {
        return true;
    }

    while (item != NULL) {
        if (tron_player_check_collision_at_rect(item->player, &rect_pos_1,
                                                &rect_pos_2)) {
            return true;
        }
        item = item->next;
    }
    return false;
}

// Function to set player's new direction
void tron_player_set_new_direction(TronPlayer *player, TronDirection new_direction) {
    if (player->live && player->current_direction != new_direction) {
        if ((new_direction == TronTop && player->current_direction != TronBottom) ||
            (new_direction == TronBottom && player->current_direction != TronTop) ||
            (new_direction == TronLeft && player->current_direction != TronRight) ||
            (new_direction == TronRight && player->current_direction != TronLeft)) {
            tron_player_vertex_add(player->first_position, player->current_position.x, player->current_position.y);
            player->current_direction = new_direction;
            //musica Trocou a direção
            if (player->type != TronPlayerBot)
                al_play_sample(player_audio_turn, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
        }
    }
}

// Function to get min coordinates
static void tron_min_position(TronPosition *result, TronPosition *pos_1, TronPosition *pos_2) {
    result->x = pos_1->x < pos_2->x ? pos_1->x : pos_2->x;
    result->y = pos_1->y < pos_2->y ? pos_1->y : pos_2->y;
}

// Function to get max coordinates
static void tron_max_position(TronPosition *result, TronPosition *pos_1, TronPosition *pos_2) {
    result->x = pos_1->x > pos_2->x ? pos_1->x : pos_2->x;
    result->y = pos_1->y > pos_2->y ? pos_1->y : pos_2->y;
}

// Function to check if rectangles overlap
static bool tron_check_if_rectangles_overlap(
    TronPosition *rectangle_1_point_1, TronPosition *rectangle_1_point_2,
    TronPosition *rectangle_2_point_1, TronPosition *rectangle_2_point_2) {
    TronPosition top_left_rect_1, top_left_rect_2, bottom_right_rect_1,
        bottom_right_rect_2;
    tron_min_position(&top_left_rect_1, rectangle_1_point_1, rectangle_1_point_2);
    tron_max_position(&bottom_right_rect_1, rectangle_1_point_1, rectangle_1_point_2);

    tron_min_position(&top_left_rect_2, rectangle_2_point_1, rectangle_2_point_2);
    tron_max_position(&bottom_right_rect_2, rectangle_2_point_1, rectangle_2_point_2);

    if (top_left_rect_1.x < bottom_right_rect_2.x &&
        bottom_right_rect_1.x > top_left_rect_2.x &&
        top_left_rect_1.y < bottom_right_rect_2.y &&
        bottom_right_rect_1.y > top_left_rect_2.y) {
        return true;
    }

    return false;
}

// Function to check the rectangles collision
bool tron_player_check_collision_at_rect(TronPlayer *player, TronPosition *point_1, TronPosition *point_2) {
    TronPlayerVertexList *vertex = player->first_position;
    TronPosition rect_point_1, rect_point_2;
    float half_thickness = (PLAYER_THICKNESS / 2);
    while (vertex != NULL && vertex->next != NULL) {
        tron_line_to_retangle(&rect_point_1, &rect_point_2, &vertex->position, &vertex->next->position, half_thickness);
        if (tron_check_if_rectangles_overlap(&rect_point_1, &rect_point_2, point_1, point_2)) {
            return true;
        }
        vertex = vertex->next;
    }
    tron_line_to_retangle(&rect_point_1, &rect_point_2, &vertex->position, &player->current_position, half_thickness);
    if (tron_check_if_rectangles_overlap(&rect_point_1, &rect_point_2, point_1, point_2)) {
        return true;
    } else {
        return false;
    }
}

// Function to get min coordinates
static int get_direction_score(TronPlayer *player, TronPlayerList *list, TronBoard *board) {
    int score;
    for (score = PLAYER_SPEED; score < MAX_BOT_LOOK; score++) {
        if (tron_player_check_if_player_lost(player, list, board, score)) {
            break;
        }
    }
    return score;
}

// Function to change bot direction
void tron_player_bot_switch_direction(TronPlayer *player, TronPlayerList *list, TronBoard *board) {
    TronDirection first_direction = TronTop, second_direction = TronTop, old_direction = player->current_direction;
    int first_direction_score, second_direction_score;
    old_direction = player->current_direction;
    switch (player->current_direction) {
        case TronTop:
        case TronBottom:
            if (rand() % 2) {
                first_direction = TronRight;
                second_direction = TronLeft;
            } else {
                first_direction = TronLeft;
                second_direction = TronRight;
            }
            break;
        default:
            if (rand() % 2) {
                first_direction = TronTop;
                second_direction = TronBottom;
            } else {
                first_direction = TronBottom;
                second_direction = TronTop;
            }
    }

    player->current_direction = first_direction;
    first_direction_score = get_direction_score(player, list, board);

    player->current_direction = second_direction;
    second_direction_score = get_direction_score(player, list, board);
    player->current_direction = old_direction;

    if (first_direction_score >= second_direction_score) {
        tron_player_set_new_direction(player, first_direction);
    } else {
        tron_player_set_new_direction(player, second_direction);
    }
}

// Function that verifies if the bot has to turn
void tron_player_update_bot(TronPlayer *player, TronPlayerList *list, TronBoard *board) {
    if (rand() % 100 == 0 || tron_player_check_if_player_lost(player, list, board, PLAYER_SPEED)) {
        tron_player_bot_switch_direction(player, list, board);
    }
}

// Function to destroy a player
void tron_destroy_player(TronPlayer *player) {
    tron_player_vertex_list_destroy(player->first_position);
    free(player->name);
    free(player);
}

// Function to draw a player
void tron_player_draw(TronPlayer *player, TronBoard *board) {
    TronPosition start, end, corner_delta;
    TronPosition *pos_1, *pos_2;
    TronDirection old_direction = TronTop;
    bool first_vertex = true;
    TronDirection direction = TronTop;

    ALLEGRO_BITMAP *player_bitmap, *corner_bitmap;

    float player_thickness = (float)PLAYER_THICKNESS * board->scale;
    TronPlayerVertexList *vertex = player->first_position;

    while (vertex != NULL) {
        pos_1 = &vertex->position;
        if (vertex->next != NULL) {
            pos_2 = &vertex->next->position;
        } else {
            pos_2 = &player->current_position;
        }
        tron_min_position(&start, pos_1, pos_2);
        tron_max_position(&end, pos_1, pos_2);

        if (start.x == end.x) {
            if (start.y == pos_1->y) {
                direction = TronBottom;
            } else {
                direction = TronTop;
            }
            player_bitmap = player_bitmap_vertical;
            start.x -= PLAYER_THICKNESS;
            end.x += PLAYER_THICKNESS;
        } else {
            if (start.x == pos_1->x) {
                direction = TronRight;
            } else {
                direction = TronLeft;
            }
            player_bitmap = player_bitmap_horizontal;
            start.y -= PLAYER_THICKNESS;
            end.y += PLAYER_THICKNESS;
        }
        if (!first_vertex) {
            switch (old_direction) {
                case TronTop:
                    switch (direction) {
                        case TronRight:
                            corner_delta.x = -1;
                            corner_delta.y = -1;
                            corner_bitmap = player_bitmap_corner_top_left;
                            break;
                        default:
                            corner_delta.x = 0;
                            corner_delta.y = -1;
                            corner_bitmap = player_bitmap_corner_top_right;
                    }
                    break;
                case TronRight:
                    switch (direction) {
                        case TronTop:
                            corner_delta.x = 0;
                            corner_delta.y = 0;
                            corner_bitmap = player_bitmap_corner_bottom_right;
                            break;
                        default:
                            corner_delta.x = 0;
                            corner_delta.y = -1;
                            corner_bitmap = player_bitmap_corner_top_right;
                    }
                    break;
                case TronBottom:
                    switch (direction) {
                        case TronRight:
                            corner_delta.x = -1;
                            corner_delta.y = 0;
                            corner_bitmap = player_bitmap_corner_bottom_left;
                            break;
                        default:
                            corner_delta.x = 0;
                            corner_delta.y = 0;
                            corner_bitmap = player_bitmap_corner_bottom_right;
                    }
                    break;
                case TronLeft:
                    switch (direction) {
                        case TronTop:
                            corner_delta.x = -1;
                            corner_delta.y = 0;
                            corner_bitmap = player_bitmap_corner_bottom_left;
                            break;
                        default:
                            corner_delta.x = -1;
                            corner_delta.y = -1;
                            corner_bitmap = player_bitmap_corner_top_left;
                    }
                    break;
            }
            al_draw_tinted_scaled_bitmap(corner_bitmap, player->color,
                                         0, 0, PLAYER_CORNER_WIDTH, PLAYER_CORNER_HEIGHT,
                                         (board->x + pos_1->x + corner_delta.x * PLAYER_THICKNESS) * board->scale,
                                         (board->y + pos_1->y + corner_delta.y * PLAYER_THICKNESS) * board->scale,
                                         player_thickness,
                                         player_thickness,
                                         0);
        }
        al_draw_tinted_scaled_bitmap(player_bitmap, player->color,
                                     0, 0, PLAYER_WIDTH, PLAYER_WIDTH,
                                     (board->x + start.x) * board->scale,
                                     (board->y + start.y) * board->scale,
                                     (end.x - start.x) * board->scale,
                                     (end.y - start.y) * board->scale,
                                     0);

        vertex = vertex->next;
        first_vertex = false;
        old_direction = direction;
    }
}

// Function to create a player list
TronPlayerList *tron_create_player_list() {
    TronPlayerList *list =
        (TronPlayerList *)tron_malloc(sizeof(TronPlayerList));
    list->size = 0;
    list->first = NULL;
    return list;
}

// Function to clear player list
void tron_player_list_flush(TronPlayerList *list) {
    TronPlayerListItem *item = list->first;
    TronPlayerListItem *item_next;
    while (item != NULL) {
        item_next = item->next;
        tron_destroy_player(item->player);
        free(item);
        item = item_next;
    }
    list->first = NULL;
    list->size = 0;
}

// Function to destroy player list
void tron_destroy_player_list(TronPlayerList *list) {
    tron_player_list_flush(list);
    free(list);
}

// Function to add a player
void tron_player_list_add_player(TronPlayerList *list, TronPlayer *player) {
    list->size++;
    TronPlayerListItem *new_item = (TronPlayerListItem *)tron_malloc(sizeof(TronPlayerListItem));
    new_item->next = NULL;
    new_item->player = player;

    if (list->first == NULL) {
        list->first = new_item;
    } else {
        TronPlayerListItem *item = list->first;
        while (item->next != NULL) {
            item = item->next;
        }
        item->next = new_item;
    }
}
