#ifndef __PONG_SDL_PIXEL__
#define __PONG_SDL_PIXEL__

#include <stdbool.h>
#include <stdio.h>

#include "./constants.h"

enum EBlockTypes { BACKGROUND_BLOCK = 0b0, NET_BLOCK = 0b1, PLAYER_BLOCK = 0b10, BALL_BLOCK = 0b100 };

typedef struct _pixel Pixel;

void setPixelX(Pixel*, size_t);
void setPixelY(Pixel*, size_t);
size_t getPixelX(Pixel*);
size_t getPixelY(Pixel*);

Pixel* initPixel(size_t x, size_t y, enum EBlockTypes blockType);
void destroyPixel(Pixel* pixel);

bool movePixelUp(Pixel* pixel);
bool movePixelDown(Pixel* pixel);
bool movePixelDownWithConstaint(Pixel* pixel, size_t lengt);
bool movePixelLeft(Pixel* pixel);
bool movePixelRight(Pixel* pixel);

#endif
