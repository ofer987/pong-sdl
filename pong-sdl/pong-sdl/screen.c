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

  enum EPlayerSide winnerOfPreviousRound;
  enum EGameMode mode;
};

enum EPlayerSide
getWinner(Screen* screen) {
  size_t leftPlayerScore = getPlayerScore(screen->leftPlayer);
  size_t rightPlayerScore = getPlayerScore(screen->rightPlayer);

  if (leftPlayerScore == rightPlayerScore) {
    return NO_WINNING_SIDE;
  }

  if (leftPlayerScore > rightPlayerScore) {
    return LEFT_SIDE;
  }

  return RIGHT_SIDE;
}

enum EPlayerSide
getWinnerOfPreviousRound(Screen* screen) {
  return screen->winnerOfPreviousRound;
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
      screen->mode = GameInProgress;

      enum EPlayerSide startingSide = screen->winnerOfPreviousRound;
      if (startingSide == 0) {
        // Default side
        startingSide = LEFT_SIDE;
      }

      reinitBall(screen->ball, startingSide);

      return;
    default:
      return;
  }
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
      reinitPlayerScore(screen->leftPlayer);
      reinitPlayerScore(screen->rightPlayer);
      /* FALLTHROUGH */
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
  free(screen->ball);
  screen->ball = NULL;
  screen->ball = initBall();

  destroyPlayer(screen->leftPlayer);
  screen->leftPlayer = NULL;
  screen->leftPlayer = initPlayer(LEFT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);

  destroyPlayer(screen->rightPlayer);
  screen->rightPlayer = NULL;
  screen->rightPlayer = initPlayer(RIGHT_SIDE, PLAYER_WIDTH, PLAYER_HEIGHT);

  screen->winnerOfPreviousRound = NO_WINNING_SIDE;

  screen->mode = GameInProgress;
  kickBall(screen->ball);
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
  enum EPlayerSide winningSide = isBallOutOfBounds(screen->ball, screen->leftPlayer, screen->rightPlayer);

  switch (winningSide) {
    case LEFT_SIDE:
      screen->winnerOfPreviousRound = LEFT_SIDE;

      return true;
    case RIGHT_SIDE:
      screen->winnerOfPreviousRound = RIGHT_SIDE;

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
