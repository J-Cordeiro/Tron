// THIS DOCUMENT CONTAINS:
// All the definitions used on the rest of the code

#ifndef __TRON_DEFS_H__
#define __TRON_DEFS_H__

#define FPS 60

#define TPS 60

// Font definitions
#define FONT_PATH "assets/font/zenDots.ttf"
#define FONT_NUMBER_OF_SCALES 3

#define GAMEOVER_FONT_SIZE 36
#define BUTTON_FONT_SIZE 9

// Player definitions
#define PLAYER_VERTICAL_BITMAP_PATH "assets/images/playerVertical.png"
#define PLAYER_HORIZONTAL_BITMAP_PATH "assets/images/playerHorizontal.png"
#define PLAYER_WIDTH 500
#define PLAYER_HEIGHT 500

#define PLAYER_CORNER_TOP_LEFT_BITMAP_PATH "assets/images/playerCornerTopLeft.png"
#define PLAYER_CORNER_TOP_RIGHT_BITMAP_PATH "assets/images/playerCornerTopRight.png"
#define PLAYER_CORNER_BOTTOM_LEFT_BITMAP_PATH "assets/images/playerCornerBottomLeft.png"
#define PLAYER_CORNER_BOTTOM_RIGHT_BITMAP_PATH "assets/images/playerCornerBottomRight.png"
#define PLAYER_CORNER_WIDTH 250
#define PLAYER_CORNER_HEIGHT 250

//audio definitions
#define AUDIO_GAMEOVER_PATH "assets/audio/gameOver.wav"
#define AUDIO_PLAYER_TURN_PATH "assets/audio/playerTurn.wav"
#define AUDIO_MENU_SOUNDTRACK_PATH "assets/audio/introMusic.wav"
#define AUDIO_GAME_SOUNDTRACK_PATH "assets/audio/gameAudio.ogg"

//controls bitmap definitions
#define PLAYER_CONTROLS_BITMAP_PATH "assets/images/controls.png"
#define PLAYER_CONTROLS_WIDTH 1808
#define PLAYER_CONTROLS_HEIGHT 512

#define PLAYER_THICKNESS 6
#define PLAYER_SPEED 3

#define MAX_BOT_LOOK 10

// Logo definitions
#define LOGO_BITMAP_PATH "assets/images/logo.png"
#define LOGO_WIDTH 1547
#define LOGO_HEIGHT 599

// Button definitions
#define BUTTON_BITMAP_PATH "assets/images/tronBigButton.png"
#define BUTTON_WIDTH 1080
#define BUTTON_HEIGHT 1080
#define BUTTON_BOX_DELTA_X 27
#define BUTTON_BOX_DELTA_Y 55
#define BUTTON_BOX_WIDTH 90
#define BUTTON_BOX_HEIGHT 35

// Board definitions
#define HORIZONTAL_LINES 5
#define VERTICAL_LINES 7
#define RECTANGLE_LINE_THICKNESS 10
#define BOARD_SCALE 2

#define DEFAULT_WIDTH 1200
#define DEFAULT_HEIGHT 600

#define COLOR_1 al_map_rgb(231, 19, 90)
#define COLOR_2 al_map_rgb(31, 35, 230)
#define COLOR_3 al_map_rgb(233, 196, 21)
#define COLOR_4 al_map_rgb(95, 207, 21)

#endif