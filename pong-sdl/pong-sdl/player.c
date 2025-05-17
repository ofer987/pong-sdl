#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./constants.h"
#include "./pixel.h"
#include "./player.h"

struct _player {
  Pixel* top;
  /* Pixel* bottom; */
  size_t length;
  size_t score;
};

Pixel*
getPlayerTopPixel(Player* player) {
  return player->top;
}

size_t
getPlayerLength(Player* player) {
  return player->length;
}

Player*
initPlayer(enum EPlayerSide side) {
  Player* result = malloc(sizeof(Player));
  switch (side) {
    case LEFT_SIDE: result->top = initPixel(LEFT_PLAYER_START_X, PLAYER_START_TOP_Y, PLAYER_BLOCK); break;
    case RIGHT_SIDE: result->top = initPixel(RIGHT_PLAYER_START_X, PLAYER_START_TOP_Y, PLAYER_BLOCK); break;
    default: fprintf(stderr, "unknown EPlayerSide %ud", side); exit(EXIT_FAILURE);
  }

  result->length = DEFAULT_LENGTH;
  result->score = 0;

  return result;
}

void
destroyPlayer(Player* player) {
  free(player->top);
  free(player);
}

bool
movePlayerUp(Player* player) {
  return movePixelUp(player->top);
}

bool
movePlayerDown(Player* player) {
  size_t topY = getPixelY(player->top);
  if ((topY + player->length) == (BOTTOM_PLAY_SCREEN - TOP_PLAY_SCREEN)) {
    return false;
  }

  return movePixelDown(player->top);
}
