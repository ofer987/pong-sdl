#ifndef __PONG_SDL_PLAYER__
#define __PONG_SDL_PLAYER__

#include <stdint.h>
#include <stdio.h>

#include "./pixel.h"

enum EPlayerMovement { NO_MOVEMENT, DOWN_MOVEMENT, UP_MOVEMENT };

enum EPlayerSide { NO_WINNING_SIDE = 0, LEFT_SIDE, RIGHT_SIDE };

typedef struct _player Player;

enum EPlayerMovement getPlayerMovement(Player*);
void setPlayerMovement(Player*, enum EPlayerMovement);
void reinitPlayerScore(Player*);
void incrementPlayerScore(Player*);
size_t getPlayerScore(Player*);
enum EPlayerSide getPlayerSide(Player*);
Pixel* getPlayerTopPixel(Player*);
int32_t getPlayerLengthMidPointY(Player*);
int32_t getPlayerWidth(Player*);
int32_t getPlayerHeight(Player*);
bool movePlayerUp(Player*);
bool movePlayerDown(Player*);

Player* initPlayer(enum EPlayerSide side, int32_t, int32_t);
void destroyPlayer(Player* player);

#endif
