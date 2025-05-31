#include <stdbool.h>
#include <stdlib.h>

#include "./ball.h"
#include "./border.h"
#include "./constants.h"
#include "./player.h"
#include "./screen.h"

struct _screen {
  Border* topBorder;
  Border* bottomBorder;

  Player* leftPlayer;
  Player* rightPlayer;
  Ball* ball;

  enum EPlayerSide lastPlayerToWin;
  enum EGameMode mode;
};

enum EPlayerSide
getLastPlayerToWin(Screen* screen) {
  if (screen->mode == GameInProgressNoWin || screen->lastPlayerToWin == 0) {
    printf("Incorrect value for (screen->lastPlayerToWin)\n");

    exit(EXIT_FAILURE);
  }

  return screen->lastPlayerToWin;
}

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

  result->topBorder = initBorder(topBorderY, TOP_BORDER_SIDE);
  result->bottomBorder = initBorder(bottomBorderY, BOTTOM_BORDER_SIDE);
  return result;
}

void
continueGame(Screen* screen) {
  switch (screen->mode) {
    case GameLost:
      if (screen->lastPlayerToWin == 0) {
        screen->mode = GameInProgressNoWin;
      } else {
        screen->mode = GameInProgress;
      }

      reinitBall(screen->ball, screen->lastPlayerToWin);

      return;
    default:
      return;
  }
}

void
startGame(Screen* screen) {
  switch (screen->mode) {
    case GameInProgressNoWin:
      /* FALLTHROUGH */
    case GameInProgress:
      screen->mode = GamePaused;

      return;
    case GamePaused:
      if (screen->lastPlayerToWin == 0) {
        screen->mode = GameInProgressNoWin;
      } else {
        screen->mode = GameInProgress;
      }

      return;
    case GameRestart:
      reinitPlayerScore(screen->leftPlayer);
      reinitPlayerScore(screen->rightPlayer);
      /* FALLTHROUGH */
    case GameNotStated:
      screen->mode = GameInProgressNoWin;
      kickBall(screen->ball);

      return;
    default:
      return;
  }
}

void
restartGame(Screen* screen) {
  free(screen->ball);
  screen->ball = NULL;
  screen->ball = initBall();

  destroyPlayer(screen->leftPlayer);
  screen->leftPlayer = NULL;
  screen->leftPlayer = initPlayer(LEFT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);

  destroyPlayer(screen->rightPlayer);
  screen->rightPlayer = NULL;
  screen->rightPlayer = initPlayer(RIGHT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);

  screen->lastPlayerToWin = 0;

  screen->mode = GameInProgressNoWin;
  kickBall(screen->ball);
}

void
setGameToLost(Screen* screen) {
  switch (screen->mode) {
    case GameInProgressNoWin:
      /* FALLTHROUGH */
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
  enum EPlayerSide winningSide = isBallOutOfBounds(screen->ball, screen->leftPlayer, screen->rightPlayer);

  switch (winningSide) {
    case LEFT_SIDE:
      screen->lastPlayerToWin = LEFT_SIDE;

      return true;
    case RIGHT_SIDE:
      screen->lastPlayerToWin = RIGHT_SIDE;

      return true;
    default:
      return false;
  }
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
