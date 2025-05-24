#ifndef __PONG_SDL_PLAYER__
#define __PONG_SDL_PLAYER__

#include <stdio.h>

#include "./pixel.h"

enum EPlayerSide { LEFT_SIDE = 0, RIGHT_SIDE };

typedef struct _player Player;

Pixel* getPlayerTopPixel(Player*);
size_t getPlayerLength(Player*);
bool movePlayerUp(Player*);
bool movePlayerDown(Player*);

Player* initPlayer(enum EPlayerSide side);
void destroyPlayer(Player* player);

#endif
