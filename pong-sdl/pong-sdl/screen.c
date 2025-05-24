#include <stdbool.h>
#include <stdlib.h>

#include "./ball.h"
#include "./border.h"
#include "./constants.h"
#include "./pixel.h"
#include "./player.h"
#include "./screen.h"

struct _screen {
  Border* topBorder;
  Border* bottomBorder;

  Player* leftPlayer;
  Player* rightPlayer;
  Ball* ball;
  enum EGameMode mode;
};

void
initPlayers(Screen* screen) {
  screen->leftPlayer = initPlayer(LEFT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);
  screen->rightPlayer = initPlayer(RIGHT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);
}

Player*
getLeftPlayer(Screen* screen) {
  return screen->leftPlayer;
}

Player*
getRightPlayer(Screen* screen) {
  return screen->rightPlayer;
}

Screen*
initScreen(size_t topBorderY, size_t bottomBorderY) {
  Screen* result = malloc(sizeof(Screen));

  initPlayers(result);
  result->ball = initBall();
  result->mode = GameNotStated;

  result->topBorder = initBorder(topBorderY, TOP_BORDER);
  result->bottomBorder = initBorder(bottomBorderY, BOTTOM_BORDER);
  return result;
}

void
startGame(Screen* screen) {
  switch (screen->mode) {
    case GameInProgress:
      screen->mode = GamePaused;

      return;
    case GamePaused:
      screen->mode = GameInProgress;

      return;
    case GameRestart:
      /* Fallthrough */
    case GameNotStated:
      screen->mode = GameInProgress;
      kickBall(screen->ball);

      return;
    default:
      return;
  }
}

void
restartGame(Screen* screen) {
  screen->mode = GameRestart;

  free(screen->ball);
  screen->ball = NULL;

  screen->ball = initBall();
  free(screen->leftPlayer);
  screen->leftPlayer = NULL;
  screen->leftPlayer = initPlayer(LEFT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);

  free(screen->rightPlayer);
  screen->rightPlayer = NULL;
  screen->rightPlayer = initPlayer(RIGHT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);
}

void
setNewGame(Screen* screen) {
  stopBall(screen->ball);
  screen->mode = GameNotStated;
}

void
setGameToLost(Screen* screen) {
  switch (screen->mode) {
    case GameInProgress:
      stopBall(screen->ball);
      screen->mode = GameLost;

      return;
    default:
      return;
  }
}

bool
isLost(Screen* screen) {
  return isBallOutOfBounds(screen->ball, screen->leftPlayer, screen->rightPlayer);
}

void
moveScreenBall(Screen* screen) {
  moveBall(screen->ball, screen->leftPlayer, screen->rightPlayer, screen->topBorder, screen->bottomBorder);
}

Ball*
getBall(Screen* screen) {
  return screen->ball;
}

EGameMode
getGameMode(Screen* screen) {
  return screen->mode;
}

void
destroyScreen(Screen* screen) {
  free(screen->leftPlayer);
  screen->leftPlayer = NULL;

  free(screen->rightPlayer);
  screen->rightPlayer = NULL;

  destoryBall(screen->ball);
  screen->ball = NULL;

  destroyBorder(screen->topBorder);
  destroyBorder(screen->bottomBorder);
}
