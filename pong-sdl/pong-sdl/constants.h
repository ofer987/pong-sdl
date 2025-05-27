#ifndef __PONG_SDL_CONSTANTS__
#define __PONG_SDL_CONSTANTS__

#include <stdio.h>

#define BLOCK_WIDTH          10
#define DEFAULT_LENGTH       15 // Middle is seven (7)

#define LEFT_BORDER          0
#define RIGHT_BORDER         600

#define TOP_BORDER           0
#define BOTTOM_BORDER        450

#define BORDER_WIDTH         10
#define BORDER_HEIGHT        10

#define TOP_PLAY_SCREEN      0 + BORDER_HEIGHT
#define BOTTOM_PLAY_SCREEN   BOTTOM_BORDER - BORDER_HEIGHT

#define LEFT_PLAY_SCREEN     0 + BORDER_WIDTH
#define RIGHT_PLAY_SCREEN    RIGHT_BORDER - BORDER_WIDTH

#define TOTAL_PIXELS         (BOTTOM_PLAY_SCREEN - TOP_PLAY_SCREEN) * (RIGHT_PLAY_SCREEN - LEFT_PLAY_SCREEN)

#define LEFT_PLAYER_START_X  LEFT_PLAY_SCREEN + 5
#define RIGHT_PLAYER_START_X RIGHT_PLAY_SCREEN - 5

#define PLAYER_START_TOP_Y   (BOTTOM_PLAY_SCREEN - TOP_PLAY_SCREEN - PLAYER_HEIGHT) / 2 + 5

#define BALL_WIDTH           10
#define BALL_HEIGHT          10

#define PLAYER_WIDTH         10
#define PLAYER_HEIGHT        150

#endif
