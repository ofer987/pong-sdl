#ifndef __PONG_SDL_PIXEL__
#define __PONG_SDL_PIXEL__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "./constants.h"

enum EBlockTypes { BACKGROUND_BLOCK = 0b0, NET_BLOCK = 0b1, PLAYER_BLOCK = 0b10, BALL_BLOCK = 0b100 };

typedef struct _pixel Pixel;

void setPixelX(Pixel*, int32_t);
void setPixelY(Pixel*, int32_t);
int32_t getPixelX(Pixel*);
int32_t getPixelY(Pixel*);

Pixel* initPixel(size_t x, size_t y, enum EBlockTypes blockType);
void destroyPixel(Pixel* pixel);

bool movePixelUp(Pixel* pixel);
bool movePixelDown(Pixel* pixel);
bool movePixelLeft(Pixel* pixel);
bool movePixelRight(Pixel* pixel);

#endif
