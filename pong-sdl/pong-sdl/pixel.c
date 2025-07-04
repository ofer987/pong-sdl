#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "./pixel.h"

struct _pixel {
  // TODO: change to uint32_t
  int32_t x;
  int32_t y;

  enum EBlockTypes type;
};

void
setPixelX(Pixel* pixel, int32_t x) {
  pixel->x = x;
}

void
setPixelY(Pixel* pixel, int32_t y) {
  pixel->y = y;
}

int32_t
getPixelX(Pixel* pixel) {
  return pixel->x;
}

int32_t
getPixelY(Pixel* pixel) {
  return pixel->y;
}

Pixel*
initPixel(size_t x, size_t y, enum EBlockTypes blockType) {
  Pixel* result = malloc(sizeof(Pixel));

  result->x = x;
  result->y = y;
  result->type = blockType;

  return result;
}

void
destroyPixel(Pixel* pixel) {
  free(pixel);
}

bool
movePixelUp(Pixel* pixel) {
  if (pixel->y <= TOP_PLAY_SCREEN) {
    return false;
  }

  pixel->y -= BLOCK_WIDTH / 2;

  return true;
}

bool
movePixelDown(Pixel* pixel) {
  if (pixel->y >= BOTTOM_PLAY_SCREEN - 1) {
    return false;
  }

  pixel->y += BLOCK_WIDTH / 2;

  return true;
}

bool
movePixelLeft(struct _pixel* pixel) {
  if (pixel->x <= LEFT_PLAY_SCREEN) {
    return false;
  }

  pixel->x -= BLOCK_WIDTH;

  return true;
}

bool
movePixelRight(struct _pixel* pixel) {
  if (pixel->x >= RIGHT_PLAY_SCREEN - BLOCK_WIDTH) {
    return false;
  }

  pixel->x -= BLOCK_WIDTH;

  return true;
}
