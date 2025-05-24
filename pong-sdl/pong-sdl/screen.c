#include <stdbool.h>
#include <stdlib.h>

#include "./ball.h"
#include "./constants.h"
#include "./pixel.h"
#include "./player.h"
#include "./screen.h"

struct _block {
  Pixel* pixel;
  Block* next;
};

struct _screen {
  Player* leftPlayer;
  Player* rightPlayer;
  Ball* ball;
  enum EGameMode mode;

  Block* blocks;
};

Block*
firstBlock(Screen* screen) {
  return screen->blocks->next;
}

void
initPlayers(Screen* screen) {
  screen->leftPlayer = initPlayer(LEFT_SIDE);
  screen->rightPlayer = initPlayer(RIGHT_SIDE);
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
initScreen(void) {
  Screen* result = malloc(sizeof(Screen));

  initPlayers(result);
  result->ball = initBall();
  result->mode = GameNotStated;
  return result;
}

void
startGame(Screen* screen) {
  switch (screen->mode) {
    case GameRestart:
    case GameNotStated:
      screen->mode = GameInProgress;
      kickBall(screen->ball);

      return;
    default: return;
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
  screen->leftPlayer = initPlayer(LEFT_SIDE);

  free(screen->rightPlayer);
  screen->rightPlayer = NULL;
  screen->rightPlayer = initPlayer(RIGHT_SIDE);
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
    default: return;
  }
}

bool
isLost(Screen* screen) {
  return isBallOutOfBounds(screen->ball, screen->leftPlayer, screen->rightPlayer);
}

void
moveScreenBall(Screen* screen) {
  moveBall(screen->ball, screen->leftPlayer, screen->rightPlayer);
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
  Block* block = screen->blocks;
  Block* nextBlock = NULL;

  if (block != NULL) {
    nextBlock = block->next;

    free(block);
    block = nextBlock;
  }
  screen->blocks = NULL;

  free(screen->leftPlayer);
  screen->leftPlayer = NULL;

  free(screen->rightPlayer);
  screen->rightPlayer = NULL;

  destoryBall(screen->ball);
  screen->ball = NULL;
}
