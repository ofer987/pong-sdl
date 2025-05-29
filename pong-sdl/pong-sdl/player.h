#ifndef __PONG_SDL_PLAYER__
#define __PONG_SDL_PLAYER__

#include <stdint.h>
#include <stdio.h>

#include "./pixel.h"

enum EPlayerSide { LEFT_SIDE = 1, RIGHT_SIDE };

typedef struct _player Player;

void reinitPlayerScore(Player*);
void incrementPlayerScore(Player*);
size_t getPlayerScore(Player*);
enum EPlayerSide getPlayerSide(Player*);
Pixel* getPlayerTopPixel(Player*);
size_t getPlayerLengthMidPointY(Player*);
int32_t getPlayerWidth(Player*);
int32_t getPlayerHeight(Player*);
bool movePlayerUp(Player*);
bool movePlayerDown(Player*);

Player* initPlayer(enum EPlayerSide side, int32_t, int32_t);
void destroyPlayer(Player* player);

#endif
