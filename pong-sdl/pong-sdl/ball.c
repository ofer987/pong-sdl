#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "./ball.h"
#include "./border.h"
#include "./constants.h"
#include "./pixel.h"
#include "./player.h"

static const float initMovementX = 10.0f;
static const float initMovementY = 0.0f;

struct _ballMovement {
  float_t x;
  float_t y;
};

typedef struct _ballMovement BallMovement;

// clang-format off
enum EBallMovements { DO_NOT_MOVE_BALL_MOVEMENT = 7, TOTAL_BALL_MOVEMENTS = 8 };
const static BallMovement ballMovements[TOTAL_BALL_MOVEMENTS] = {
  [DO_NOT_MOVE_BALL_MOVEMENT] = {.x = 0.0f,  .y = 0.0f},
  [0] = {.x = 3.2f / 1, .y = 0.0f / 1},
  [1] = {.x = 3.2f / 1, .y = 1.2f / 1},
  [2] = {.x = 3.2f / 1, .y = 1.6f / 1},
  [3] = {.x = 3.2f / 1, .y = 2.0f / 1},
  [4] = {.x = 3.2f / 1, .y = 2.3f / 1},
  [5] = {.x = 3.2f / 1, .y = 2.5f / 1},
  [6] = {.x = 3.2f / 1, .y = 2.8f / 1},
};
// clang-format on

/* balls[0] = {.x = 10, .y = 13 }; */
/* ] */
/* static BallMovement ballMovements[10] = [ */
/*  { .x = 5, .y = 10 } */
/* ] */

struct _ball {
  BallMovement ballMovement;

  Pixel* pixel;
  int32_t width;
  int32_t height;
};

Ball*
initBall(void) {
  Ball* result = malloc(sizeof(Ball));

  result->ballMovement = ballMovements[8];

  size_t middleX = (RIGHT_PLAY_SCREEN - LEFT_PLAY_SCREEN) / 2;
  size_t middleY = (BOTTOM_PLAY_SCREEN - TOP_PLAY_SCREEN) / 2;
  Pixel* ball = initPixel(middleX, middleY, BALL_BLOCK);
  result->pixel = ball;

  result->width = BALL_WIDTH;
  result->height = BALL_HEIGHT;

  return result;
}

bool
isCollidedWithPlayerX(int32_t ballX, Player* player) {
  Pixel* playerPixel = getPlayerTopPixel(player);
  int32_t playerX = getPixelX(playerPixel);

  int result = abs(ballX - playerX);

  return result == 0;
}

int32_t
getBallHeight(Ball* ball) {
  return ball->height;
}

int32_t
getBallWidth(Ball* ball) {
  return ball->width;
}

bool
isCollidedWithPlayer(Ball* ball, Player* player) {
  bool result = false;
  int32_t newBallPixelX = 0;
  int32_t newBallPixelY = 0;

  int32_t ballPixelX = (int32_t)getPixelX(ball->pixel);
  int32_t ballPixelY = (int32_t)getPixelY(ball->pixel);

  Pixel* playerPixel = getPlayerTopPixel(player);
  int32_t playerPixelX = getPixelX(playerPixel);
  int32_t playerPixelY = getPixelY(playerPixel);

  int32_t nextBallPixelX = ballPixelX + ball->ballMovement.x;
  int32_t nextBallPixelY = ballPixelY + ball->ballMovement.y;

  enum EPlayerSide side = getPlayerSide(player);

  bool isCollision = false;
  /* int32_t collisionX = -1; */
  switch (side) {
    case LEFT_SIDE:
      isCollision = nextBallPixelX - (getBallWidth(ball) / 2) <= playerPixelX + (getPlayerWidth(player) / 2);

      break;
    case RIGHT_SIDE:
      isCollision = nextBallPixelX + (getBallWidth(ball) / 2) >= playerPixelX - (getPlayerWidth(player) / 2);

      break;
  }

  newBallPixelX = nextBallPixelX;
  newBallPixelY = nextBallPixelY;
  /* if (!isCollision) { */
  /*   goto END_IS_COLLISION_PLAYER; */
  /* } */

  int32_t diffBallX = playerPixelX - ballPixelX;
  /* int32_t diffNextBallX = playerPixelX - nextBallPixelX; */
  /* int32_t diffNextBallY = playerPixelY - nextBallPixelY; */
  int32_t diffBallY = (int32_t)llround((double)ball->ballMovement.y * (double)diffBallX / (double)ball->ballMovement.x);
  int32_t ballAtPlayerY = ballPixelY + diffBallY;
  int32_t playerLengthY = (int32_t)getPlayerHeight(player);

  // Collision with Player
  if (!isCollision) {
    return false;
  }

  if (ballAtPlayerY >= playerPixelY && ballAtPlayerY < (playerPixelY + playerLengthY)) {
    return true;
  }

  return false;
}

void
isCollidedWithPlayerChangeCourse(Ball* ball, Player* player, int32_t nextBallPixelX, int32_t nextBallPixelY) {
  bool result = false;
  int32_t newBallPixelX = 0;
  int32_t newBallPixelY = 0;

  int32_t ballPixelX = (int32_t)getPixelX(ball->pixel);
  int32_t ballPixelY = (int32_t)getPixelY(ball->pixel);

  Pixel* playerPixel = getPlayerTopPixel(player);
  int32_t playerPixelX = getPixelX(playerPixel);
  int32_t playerPixelY = getPixelY(playerPixel);

  /* int32_t nextBallPixelX = ballPixelX + ball->ballMovement.x; */
  /* int32_t nextBallPixelY = ballPixelY + ball->ballMovement.y; */

  enum EPlayerSide side = getPlayerSide(player);

  /* bool isCollision = false; */
  /* #<{(| int32_t collisionX = -1; |)}># */
  /* switch (side) { */
  /*   case LEFT_SIDE: */
  /*     isCollision = nextBallPixelX <= playerPixelX; */
  /*  */
  /*     break; */
  /*   case RIGHT_SIDE: */
  /*     isCollision = nextBallPixelX >= playerPixelX; */
  /*  */
  /*     break; */
  /* } */

  newBallPixelX = nextBallPixelX;
  newBallPixelY = nextBallPixelY;
  /* if (!isCollision) { */
  /*   goto END_IS_COLLISION_PLAYER; */
  /* } */

  int32_t diffBallX = playerPixelX - ballPixelX;
  /* int32_t diffNextBallX = playerPixelX - nextBallPixelX; */
  /* int32_t diffNextBallY = playerPixelY - nextBallPixelY; */
  int32_t diffBallY = (int32_t)llround((double)ball->ballMovement.y * (double)diffBallX / (double)ball->ballMovement.x);
  int32_t ballAtPlayerY = ballPixelY + diffBallY;
  int32_t playerLengthY = (int32_t)getPlayerHeight(player);

  // Collision with Player
  int32_t diffY = ((int32_t)ballAtPlayerY - (int32_t)getPlayerLengthMidPointY(player)) / getBallHeight(ball);
  int32_t newBallMovementIndex = abs(diffY);
  ball->ballMovement = ballMovements[newBallMovementIndex];

  if (diffY < 0) {
    ball->ballMovement.y = 0 - ball->ballMovement.y;
  }

  if (side == RIGHT_SIDE) {
    ball->ballMovement.x = 0 - ball->ballMovement.x;
  }

  newBallPixelX = ballPixelX + ball->ballMovement.x;
  newBallPixelY = ballPixelY + ball->ballMovement.y;
  // No collision
  setPixelX(ball->pixel, newBallPixelX);
  setPixelY(ball->pixel, newBallPixelY);
}

bool
isCollidedWithPlayerY(size_t ballY, Player* player) {
  Pixel* playerPixel = getPlayerTopPixel(player);
  size_t startY = getPixelY(playerPixel);
  size_t endY = startY + getPlayerHeight(player);

  if (ballY < startY) {
    return false;
  }

  if (ballY >= endY) {
    return false;
  }

  return true;
}

/* getDegrees(size */

void
changeBallMovement(Ball* ball, Player* player, size_t x, size_t y) {
  double_t halfPi = M_PI_2;
  /* (40 / 45) * (M_PI / 2); */
  /* static double_t pi = acos(-1); */

  // Get degrees
  size_t playerLengthY = getPlayerHeight(player);
  Pixel* playerTopPixel = getPlayerTopPixel(player);
  size_t playerTopPixelY = getPixelY(playerTopPixel);

  size_t midPointY = playerTopPixelY + (playerLengthY / 2);

  double_t angle = ((int32_t)(y - midPointY) / (playerLengthY / 2.0)) * ((M_PI_2 * 40 / 45.0));
  /* size_t midPointY = (3 / 2) * playerTopPixelY - (playerLengthY / 2); */

  /* double_t degrees = (y - midPointY) / 40.0; */
  /* dou */
  /* if (y == midPointY) { */
  /*   degrees = 0.0; */
  /* } else if (y < midPointY) { */
  /*   degrees = (y - midPointY) / 40.0; */
  /* } else { */
  /*   degrees = (midPointY - y) / 40.0; */
  /* } */

  /* fabs(degrees) */
  double_t newMovementX = cos(angle);
  double_t newMovementY = sin(angle);

  if (y < midPointY) {
    /* ball->yMovement -= newMovementY; */
  } else {
    /* ball->yMovement -= newMovementY; */
  }

  /* newMovementX = fabs(newMovementX); */

  enum EPlayerSide side = getPlayerSide(player);
  switch (side) {
    case LEFT_SIDE:
      /* ball->xMovement += newMovementX; */
      break;
    case RIGHT_SIDE:
      /* ball->xMovement += newMovementX; */
      break;
  }
}

bool
isCollidedWithBorder(Ball* ball, Border* border) {
  /* return abs(y - border) <= 2; */
  bool result = false;

  int32_t ballPixelX = (int32_t)getPixelX(ball->pixel);
  int32_t ballPixelY = (int32_t)getPixelY(ball->pixel);

  int32_t nextBallPixelX = ballPixelX + ball->ballMovement.x;
  int32_t nextBallPixelY = ballPixelY + ball->ballMovement.y;

  /* Pixel* playerPixel = getPlayerTopPixel(player); */
  /* int32_t playerPixelX = getPixelX(playerPixel); */
  /* int32_t playerPixelY = getPixelY(playerPixel); */

  int32_t borderY = (int32_t)getBorderY(border);
  switch (getBorderSide(border)) {
    case TOP_BORDER_SIDE:
      if (nextBallPixelY <= borderY) {
        return true;
      }

      break;
    case BOTTOM_BORDER_SIDE:
      if (nextBallPixelY >= borderY) {
        return true;
      }

      break;
  }

  return false;
}

void
isCollidedWithBorderChangeCourse(Ball* ball, Border* border, int32_t nextBallPixelX, int32_t nextBallPixelY) {
  int32_t ballPixelX = (int32_t)getPixelX(ball->pixel);
  int32_t ballPixelY = (int32_t)getPixelY(ball->pixel);

  int32_t newBallX = nextBallPixelX;
  int32_t newBallY = nextBallPixelY;
  int32_t diffNextBallToBallY = ball->ballMovement.y;
  int32_t diffNextBallToBallX = ball->ballMovement.x;
  int32_t diffBorderToBallY = getBorderY(border) - ballPixelY;

  newBallY = ballPixelY;
  double newBallXInDouble = 2.0 * ((double)diffNextBallToBallX / (double)diffNextBallToBallY)
                            * (double)diffBorderToBallY;
  newBallX = ballPixelX + (int32_t)llround(newBallXInDouble);

  ball->ballMovement.y = 0 - ball->ballMovement.y;
  setPixelX(ball->pixel, newBallX);
  setPixelY(ball->pixel, newBallY);
}

void
moveBall(Ball* ball, Player* leftPlayer, Player* rightPlayer, Border* topBorder, Border* bottomBorder) {
  int32_t ballPixelX = (int32_t)getPixelX(ball->pixel);
  int32_t ballPixelY = (int32_t)getPixelY(ball->pixel);

  int32_t nextBallPixelX = ballPixelX + ball->ballMovement.x;
  int32_t nextBallPixelY = ballPixelY + ball->ballMovement.y;

  /* int32_t nextBallPixelX = ballPixelX + ball->ballMovement.x; */
  /* int32_t nextBallPixelY = ballPixelY + ball->ballMovement.y; */
  /* size_t x = getPixelX(ball->pixel); */
  /* size_t y = getPixelY(ball->pixel); */

  Player* players[2] = {leftPlayer, rightPlayer};

  for (size_t index = 0; index < 2; index += 1) {
    Player* player = players[index];
    bool isCollision = isCollidedWithPlayer(ball, player);

    if (isCollision) {
      isCollidedWithPlayerChangeCourse(ball, player, nextBallPixelX, nextBallPixelY);

      return;
    }
  }

  Border* borders[2] = {topBorder, bottomBorder};

  for (size_t index = 0; index < 2; index += 1) {
    Border* border = borders[index];

    bool isCollision = isCollidedWithBorder(ball, border);
    if (isCollision) {
      isCollidedWithBorderChangeCourse(ball, border, nextBallPixelX, nextBallPixelY);
      return;
    }
  }

  setPixelX(ball->pixel, nextBallPixelX);
  setPixelY(ball->pixel, nextBallPixelY);

  return;
}

void
stopBall(Ball* ball) {
  ball->ballMovement = ballMovements[DO_NOT_MOVE_BALL_MOVEMENT];
  /* ball->xMovement = 0.0f; */
  /* ball->yMovement = 0.0f; */
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
  ball->ballMovement = ballMovements[0];
  /* ball->xMovement = initMovementX; */
  /* ball->yMovement = initMovementY; */
}

void
destoryBall(Ball* ball) {
  destroyPixel(ball->pixel);
  ball->pixel = NULL;

  free(ball);
}
