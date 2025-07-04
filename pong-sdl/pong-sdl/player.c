#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "./constants.h"
#include "./pixel.h"
#include "./player.h"

struct _player {
  Pixel* top;

  enum EPlayerSide side;
  int32_t height;
  int32_t width;

  size_t score;

  enum EPlayerMovement movement;
};

enum EPlayerSide
getPlayerSide(Player* player) {
  return player->side;
}

Pixel*
getPlayerTopPixel(Player* player) {
  return player->top;
}

int32_t
getPlayerLengthMidPointY(Player* player) {
  int32_t y = getPixelY(player->top);

  return y + (player->height / 2) - 1;
}

int32_t
getPlayerWidth(Player* player) {
  return player->width;
}

int32_t
getPlayerHeight(Player* player) {
  return player->height;
}

Player*
initPlayer(enum EPlayerSide side, int32_t width, int32_t height) {
  Player* result = malloc(sizeof(Player));
  switch (side) {
    case LEFT_SIDE:
      result->top = initPixel(LEFT_PLAYER_START_X, PLAYER_START_TOP_Y, PLAYER_BLOCK);

      break;
    case RIGHT_SIDE:
      result->top = initPixel(RIGHT_PLAYER_START_X, PLAYER_START_TOP_Y, PLAYER_BLOCK);

      break;
    default:
      fprintf(stderr, "unknown EPlayerSide %ud", side);
      exit(EXIT_FAILURE);
  }

  result->width = width;
  result->height = height;
  result->side = side;
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
  int32_t topY = getPixelY(player->top);
  if (topY <= (TOP_BORDER + PLAYER_BLOCK_HEIGHT)) {
    return false;
  }

  return movePixelUp(player->top);
}

enum EPlayerMovement
getPlayerMovement(Player* player) {
  return player->movement;
}

void
setPlayerMovement(Player* player, enum EPlayerMovement newMovement) {
  player->movement = newMovement;
}

bool
movePlayerDown(Player* player) {
  int32_t topY = getPixelY(player->top);
  if ((topY + player->height) >= BOTTOM_BORDER) {
    return false;
  }

  return movePixelDown(player->top);
}

void
reinitPlayerScore(Player* player) {
  player->score = 0;
}

void
incrementPlayerScore(Player* player) {
  player->score += 1;
}

size_t
getPlayerScore(Player* player) {
  return player->score;
}
