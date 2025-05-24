#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "./ball.h"
#include "./constants.h"
#include "./pixel.h"
#include "./player.h"

struct _ball {
  double xMovement;
  double yMovement;

  Pixel* pixel;
};

Ball*
initBall() {
  Ball* result = malloc(sizeof(Ball));

  result->xMovement = 0.0f;
  result->yMovement = 0.0f;

  size_t middleX = (RIGHT_PLAY_SCREEN - LEFT_PLAY_SCREEN) / 2;
  size_t middleY = (BOTTOM_PLAY_SCREEN - TOP_PLAY_SCREEN) / 2;
  Pixel* ball = initPixel(middleX, middleY, BALL_BLOCK);
  result->pixel = ball;

  return result;
}

int32_t
isCollidedWithPlayerX(size_t ballX, Player* player) {
  Pixel* playerPixel = getPlayerTopPixel(player);
  size_t playerX = getPixelX(playerPixel);

  size_t result = ballX - playerX;

  return (uint32_t)result;
}

bool
isCollidedWithPlayerY(size_t ballY, Player* player) {
  Pixel* playerPixel = getPlayerTopPixel(player);
  size_t startY = getPixelY(playerPixel);
  size_t endY = startY + getPlayerLength(player);

  if (ballY < startY) {
    return false;
  }

  if (ballY >= endY) {
    return false;
  }

  return true;
}

void
moveBall(Ball* ball, Player* leftPlayer, Player* rightPlayer) {
  size_t x = getPixelX(ball->pixel);
  size_t y = getPixelY(ball->pixel);

  bool collidedY = isCollidedWithPlayerY(y, leftPlayer);

  if (collidedY && isCollidedWithPlayerX(x, leftPlayer) == 1) {
    ball->xMovement = 0 - ball->xMovement;
  }

  collidedY = isCollidedWithPlayerY(y, rightPlayer);
  if (collidedY && isCollidedWithPlayerX(x, rightPlayer) == -1) {
    ball->xMovement = 0 - ball->xMovement;
  }

  x += ball->xMovement;
  y += ball->yMovement;

  setPixelX(ball->pixel, x);
  setPixelY(ball->pixel, y);
}

void
stopBall(Ball* ball) {
  ball->xMovement = 0.0f;
  ball->yMovement = 0.0f;
}

bool
isBallOutOfBounds(Ball* ball, Player* leftPlayer, Player* rightPlayer) {
  size_t ballX = getPixelX(ball->pixel);

  Pixel* leftPlayerPixel = getPlayerTopPixel(leftPlayer);
  size_t leftPlayerX = getPixelX(leftPlayerPixel);

  if (ballX < leftPlayerX) {
    return true;
  }

  Pixel* rightPlayerPixel = getPlayerTopPixel(rightPlayer);
  size_t rightPlayerX = getPixelX(rightPlayerPixel);
  if (ballX > rightPlayerX) {
    return true;
  }

  return false;
}

Pixel*
getBallPixel(Ball* ball) {
  return ball->pixel;
}

void
kickBall(Ball* ball) {
  ball->xMovement = 1.0f;
  ball->yMovement = 0.0f;
}

void
destoryBall(Ball* ball) {
  destroyPixel(ball->pixel);
  ball->pixel = NULL;

  free(ball);
}
