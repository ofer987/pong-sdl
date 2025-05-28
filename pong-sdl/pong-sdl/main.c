/*
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include "./types.h" */
#include "./constants.h"
#include "./pixel.h"
#include "./player.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* #include "./constants.h" */
/* #include "./coordinates.h" */
/* #include "./game.h" */
/* #include "./snake.h" */

/* #include "./constants.h" */
#include "./ball.h"
#include "./player.h"
#include "./screen.h"
/* #include "./player.h" */

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static Uint32 TIME_DELAY = 10;
static Uint32 TEN_MILLISECONDS = 10;
static Uint64 ONE_HUNDRED_MILLISECONDS = 100;

static const float pixelScale = 1.5f;
static const float textScale = 1.5f;

static uint64_t rerenderedMovementFrames = 0;

static Screen* screen;
static Player* leftPlayer;
static Player* rightPlayer;
static bool movement_changed = false;

#ifdef DEBUG
#define WINDOW_SCREEN_MODE SDL_WINDOW_FULLSCREEN
#else
#define WINDOW_SCREEN_MODE SDL_WINDOW_MAXIMIZED
#endif

static size_t RECORD = 0;
static size_t SCORE = 0;
static char const* const RECORD_PATH = "record.txt";

/* size_t */
/* get_score(Game* game) { */
/*   Snake* snake = get_snake_location(game); */
/*   return get_snake_length(snake) - 1; */
/* } */

bool
write_record(char* record_path, size_t record) {
  FILE* path = fopen(record_path, "wt");
  if (path == NULL) {
    fprintf(stderr, "Failed to write the record (%zu) to %s\n", record, record_path);

    return false;
  }

  fprintf(path, "%zu", record);
  return true;
}

void
render_achievement(size_t new_record) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugTextFormat(renderer, MAZE_WIDTH / 2.0, MAZE_HEIGHT / 2.0, "New Record:  %zu", new_record); */
}

size_t
read_current_record(char* path) {
  FILE* file = fopen(path, "rt");
  if (file == NULL) {
    return 0;
  }

  size_t result = 0;
  fscanf(file, "%zu", &result);

  return result;
}

/* This function runs once at startup. */
SDL_AppResult
SDL_AppInit(void** appstate, int argc, char* argv[]) {
  srandom((unsigned)time(NULL));
  RECORD = read_current_record((char*)RECORD_PATH);

  SDL_HideCursor();

  /* Create the window */
  if (!SDL_CreateWindowAndRenderer("Pong SDL", RIGHT_PLAY_SCREEN - LEFT_PLAY_SCREEN,
                                   BOTTOM_PLAY_SCREEN - TOP_PLAY_SCREEN, WINDOW_SCREEN_MODE | SDL_WINDOW_INPUT_FOCUS,
                                   &window, &renderer)) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  screen = initScreen(TOP_PLAY_SCREEN, BOTTOM_PLAY_SCREEN);
  leftPlayer = getLeftPlayer(screen);
  rightPlayer = getRightPlayer(screen);

  // There are 1_000_000_000 nanoseconds in a second
  /* ns_per_frame = 1000000000 / fps; */

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult
SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }

  /* enum GAME_MODES game_mode = get_game_mode(game); */
  /* enum MOVEMENTS current_movement = get_current_movement(game); */

  if (event->type == SDL_EVENT_KEY_DOWN) {
    SDL_Keycode key = event->key.key;
    switch (key) {
      case SDLK_Q:
        /* SCORE = get_score(game); */
        /* set_game_mode(game, QUIT); */
        /*  */
        /* // Press Q twice to quit the game */
        /* switch (game_mode) { */
        /*   case LOST: */
        /*     #<{(| FALLTHROUGH |)}># */
        /*   case QUIT: */
        /*     // end the program, reporting success to the OS. */
        /*     return SDL_APP_SUCCESS; */
        /*   default: break; */
        /* } */

        /* if (SCORE > RECORD) { */
        /*   write_record((char*)RECORD_PATH, SCORE); */
        /* } */

        return SDL_APP_SUCCESS;
      case SDLK_R:
        restartGame(screen);

        return SDL_APP_CONTINUE;
      case SDLK_P:
        startGame(screen);
        /* set_current_movement(game, NOTHING); */
        /* set_game_mode(game, PAUSE); */

        return SDL_APP_CONTINUE;
      case SDLK_C:
        continueGame(screen);

        return SDL_APP_CONTINUE;
    }

    // Return early
    // If the movement-change has not been rendered
    if (movement_changed) {
      return SDL_APP_CONTINUE;
    }

    EGameMode mode = getGameMode(screen);
    if (mode == GameNotStated) {
      startGame(screen);

      return SDL_APP_CONTINUE;
    } else if (mode == GameRestart) {
      startGame(screen);

      return SDL_APP_CONTINUE;
    } else if (mode == GameInProgress) {
      Player* leftPlayer = getLeftPlayer(screen);
      Player* rightPlayer = getRightPlayer(screen);

      switch (key) {
        case SDLK_S:
          movePlayerDown(leftPlayer);

          break;
        case SDLK_A:
          movePlayerUp(leftPlayer);

          break;
        case SDLK_J:
          movePlayerDown(rightPlayer);

          break;
        case SDLK_K:
          movePlayerUp(rightPlayer);

          break;
        default:
          break;
      }

      return SDL_APP_CONTINUE;
    }
  }

  return SDL_APP_CONTINUE;
}

/* void */
/* render_record(size_t score) { */
/*   const float scale = 2.0f; */
/*  */
/*   SDL_SetRenderScale(renderer, scale, scale); */
/*   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */
/*   #<{(| SDL_RenderDebugTextFormat(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT, "Record: %zu", score); |)}># */
/* } */

/* void */
/* render_text(size_t score) { */
/*   const float scale = 2.0f; */
/*  */
/*   SDL_SetRenderScale(renderer, scale, scale); */
/*   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */
/*   #<{(| SDL_RenderDebugTextFormat(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 10, "Score:  %zu", score); |)}># */
/* } */
/*  */
/* void */
/* render_keys(void) { */
/*   const float scale = 2.0f; */
/*  */
/*   SDL_SetRenderScale(renderer, scale, scale); */
/*   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */
/*   #<{(| SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 20, "Press:  (Q)uit | (R)estart | (P)ause"); |)}># */
/* } */
/*  */
/* void */
/* render_lost_keys(void) { */
/*   const float scale = 2.0f; */
/*  */
/*   SDL_SetRenderScale(renderer, scale, scale); */
/*   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */
/*   #<{(| SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 20, "Press:  (Q)uit | (R)estart"); |)}># */
/* } */
/*  */
/* void */
/* render_pause(void) { */
/*   const float scale = 2.0f; */
/*  */
/*   SDL_SetRenderScale(renderer, scale, scale); */
/*   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */
/*   #<{(| SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 30, "Game is Paused. Any key to continue"); |)}># */
/* } */
/*  */
/* void */
/* render_quit(void) { */
/*   const float scale = 2.0f; */
/*  */
/*   SDL_SetRenderScale(renderer, scale, scale); */
/*   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */
/*   #<{(| SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 30, "Press Q again to Quit"); |)}># */
/* } */
/*  */
/* void */
/* render_lost(void) { */
/*   const float scale = 2.0f; */
/*  */
/*   SDL_SetRenderScale(renderer, scale, scale); */
/*   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */
/*   #<{(| SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 30, "You have lost!"); |)}># */
/* } */

/* void */
/* renderPlayer(Player* player) {} */

/* void */
/* renderScreen(Screen* screen) { */
/*   Pixel* pixel = NULL; */
/*   renderPlayer(screen->leftPlayer); */
/*   renderPlayer(screen->rightPlayer); */
/* } */

/* void */
/* void */
/* drawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius) { */
/*   const int32_t diameter = (radius * 2); */
/*  */
/*   int32_t x = (radius - 1); */
/*   int32_t y = 0; */
/*   int32_t tx = 1; */
/*   int32_t ty = 1; */
/*   int32_t error = (tx - diameter); */
/*  */
/*   while (x >= y) { */
/*     //  Each of the following renders an octant of the circle */
/*     SDL_RenderPoint(renderer, centreX + x, centreY - y); */
/*     SDL_RenderPoint(renderer, centreX + x, centreY + y); */
/*     SDL_RenderPoint(renderer, centreX - x, centreY - y); */
/*     SDL_RenderPoint(renderer, centreX - x, centreY + y); */
/*     SDL_RenderPoint(renderer, centreX + y, centreY - x); */
/*     SDL_RenderPoint(renderer, centreX + y, centreY + x); */
/*     SDL_RenderPoint(renderer, centreX - y, centreY - x); */
/*     SDL_RenderPoint(renderer, centreX - y, centreY + x); */
/*  */
/*     if (error <= 0) { */
/*       ++y; */
/*       error += ty; */
/*       ty += 2; */
/*     } */
/*  */
/*     if (error > 0) { */
/*       --x; */
/*       tx += 2; */
/*       error += (tx - diameter); */
/*     } */
/*   } */
/* } */

void
renderPlayerScore(Player* player, size_t x, size_t y) {
  size_t score = getPlayerScore(player);
  int32_t total_characters_read = -1;

  char scoreBuffer[30];
  enum EPlayerSide side = getPlayerSide(player);
  switch (side) {
    case LEFT_SIDE:
      total_characters_read = snprintf(scoreBuffer, 30, "Left Player: %zu", score);
      break;
    case RIGHT_SIDE:
      total_characters_read = snprintf(scoreBuffer, 30, "Right Player: %zu", score);
      break;
  }

  if (total_characters_read <= 0) {
    printf("Failed to read score of Left Player\n");
    exit(EXIT_FAILURE);
  }

  SDL_RenderDebugText(renderer, x, y, scoreBuffer);
}

void
render(Screen* screen) {
  // Reset
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Render screen
  Uint8 red = 0, green = 0, blue = 0;
  SDL_FRect myFrects[TOTAL_PIXELS];
  /* SDL_FRect* snake_head_rect = NULL; */

  SDL_FRect screenRect = {.x = 0.0f,
                          .y = 0.0f,
                          .w = (float)(RIGHT_PLAY_SCREEN - LEFT_PLAY_SCREEN),
                          .h = (float)(BOTTOM_PLAY_SCREEN - TOP_PLAY_SCREEN)};
  red = 0;
  green = 0;
  blue = 0;

  SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
  SDL_RenderFillRect(renderer, &screenRect);

  /* Coordinates* screen_coordinates = screen[index]; */
  // TODO: adjust for 0-based later
  /* size_t index = 0; */
  /* for (size_t row = TOP_PLAY_SCREEN; row < BOTTOM_PLAY_SCREEN; row += 1) { */
  /*   for (size_t column = LEFT_PLAY_SCREEN; column < RIGHT_PLAY_SCREEN; column += 1) { */
  /*     myFrects[index].x = column; */
  /*     myFrects[index].y = row; */
  /*     myFrects[index].w = 1; */
  /*     myFrects[index].h = 1; */
  /*  */
  /*     red = 127; */
  /*     green = 127; */
  /*     blue = 127; */
  /*  */
  /*     SDL_SetRenderDrawColor(renderer, red, green, blue, 255); */
  /*     SDL_RenderFillRect(renderer, &myFrects[index]); */
  /*  */
  /*     index += 1; */
  /*   } */
  /* } */

  /* red = 100; */
  /* green = 200; */
  /* blue = 50; */

  /* SDL_RenderPoint(renderer, 3, 5); */
  /* SDL_SetRenderDrawColor(renderer, red, green, blue, 255); */
  /* drawCircle(renderer, 250, 250, 10); */

  /* SDL_FPoint points[1] = {{.x = 2, .y = 0}}; */
  /* SDL_RenderPoints(renderer, points, 500); */
}

void
renderBorders() {
  // clang-format off
  SDL_FRect topBorder = {
    .x = LEFT_BORDER,
    .y = TOP_BORDER,
    .w = RIGHT_BORDER,
    .h = BORDER_HEIGHT
  };
  SDL_FRect bottomBorder = {
    .x = LEFT_BORDER,
    .y = BOTTOM_BORDER,
    .w = RIGHT_BORDER,
    .h = BORDER_HEIGHT
  };
  SDL_FRect leftBorder = {
    .x = LEFT_BORDER,
    .y = TOP_BORDER + BORDER_HEIGHT,
    .w = BORDER_WIDTH,
    .h = BOTTOM_BORDER - BORDER_HEIGHT
  };
  SDL_FRect rightBorder = {
    .x = RIGHT_BORDER,
    .y = TOP_BORDER,
    .w = BORDER_WIDTH,
    .h = BOTTOM_BORDER + BORDER_HEIGHT
  };
  // clang-format on

  Uint8 red = 255, blue = 255, green = 255;

  SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
  SDL_RenderFillRect(renderer, &topBorder);
  SDL_RenderFillRect(renderer, &bottomBorder);
  SDL_RenderFillRect(renderer, &leftBorder);
  SDL_RenderFillRect(renderer, &rightBorder);

  return;
}

void
renderBall(Ball* ball) {
  Uint64 red = 0, blue = 255, green = 0;

  Pixel* ballPixel = getBallPixel(ball);
  SDL_FRect ballRect = {.x = getPixelX(ballPixel), .y = getPixelY(ballPixel), .w = BALL_WIDTH, .h = BALL_HEIGHT};

  SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
  SDL_RenderFillRect(renderer, &ballRect);
}

void
renderPlayer(Player* player) {
  Pixel* pixel = getPlayerTopPixel(player);
  size_t x = getPixelX(pixel);
  size_t y = getPixelY(pixel);

  /* size_t length = getPlayerLength(player); */
  SDL_FRect playerFrect;

  static Uint64 red = 255, blue = 0, green = 0;
  /* for (size_t index = 0; index < length; index += 1) { */
  /* SDL_FRect frect = myFrects[index]; */

  playerFrect.x = x;
  playerFrect.y = y;
  playerFrect.w = PLAYER_WIDTH;
  playerFrect.h = PLAYER_HEIGHT;

  SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
  SDL_RenderFillRect(renderer, &playerFrect);
}

void
renderGameMode(Screen* screen) {
  const float scale = textScale;
  SDL_SetRenderScale(renderer, scale, scale);
  size_t x = LEFT_TEXT_AREA;
  size_t y = TOP_TEXT_AREA;

  EGameMode mode = getGameMode(screen);
  enum EPlayerSide winningSide;
  switch (mode) {
    case GameRestart:
      /* FALLTHROUGH */
    case GameNotStated:
      SDL_RenderDebugText(renderer, x, y, "Game Not Started");

      break;
    case GameLost:
      winningSide = getLastPlayerToWin(screen);
      switch (winningSide) {
        case LEFT_SIDE:
          SDL_RenderDebugText(renderer, x, y, "Right Player wins the round!");

          break;
        case RIGHT_SIDE:
          SDL_RenderDebugText(renderer, x, y, "Left Player wins the round!");

          break;
      }

      renderPlayerScore(leftPlayer, x, y + TEXT_AREA_HEIGHT * 2);
      renderPlayerScore(rightPlayer, x, y + TEXT_AREA_HEIGHT * 3);

      SDL_RenderDebugText(renderer, x, y + TEXT_AREA_HEIGHT * 5, "Press R to (R)estart");
      SDL_RenderDebugText(renderer, x, y + TEXT_AREA_HEIGHT * 6, "Press C to (C)ontinue");
      SDL_RenderDebugText(renderer, x, y + TEXT_AREA_HEIGHT * 7, "Press Q to (Q)uit");
      break;
    case GameInProgress:
      SDL_RenderDebugText(renderer, x, y, "Playing");
      renderPlayerScore(leftPlayer, x, y + TEXT_AREA_HEIGHT * 2);
      renderPlayerScore(rightPlayer, x, y + TEXT_AREA_HEIGHT * 3);

      break;
    default:
      break;
  }
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult
SDL_AppIterate(void* appstate) {
  /* Snake* snake = get_snake_location(game); */
  Ball* ball = getBall(screen);

  const float scale = pixelScale;
  SDL_SetRenderScale(renderer, scale, scale);
  EGameMode gameMode = getGameMode(screen);

  bool isGameLost = (gameMode == GameLost) || isLost(screen);
  if (isGameLost) {
    setGameToLost(screen);
  }

  // Move the snake
  // TODO: change to false
  bool isScreenRerendered = false;
  uint64_t newRerenderedMovementFrames = SDL_GetTicks();
  if (newRerenderedMovementFrames >= rerenderedMovementFrames + 1.5 * TEN_MILLISECONDS) {
    isScreenRerendered = true;
    rerenderedMovementFrames = newRerenderedMovementFrames;
  }

  /* if (isGameLost) { */
  /*   renderLostText(screen); */
  /*  */
  /*   SDL_RenderPresent(renderer); */
  /* if (isScreenRerendered) { */
  render(screen);
  renderBorders();
  renderPlayer(getLeftPlayer(screen));
  renderPlayer(getRightPlayer(screen));

  renderBall(ball);

  renderGameMode(screen);

  SDL_RenderPresent(renderer);

  if (getGameMode(screen) == GameInProgress && isScreenRerendered) {
    moveScreenBall(screen);
  }
  /* } */

  SDL_Delay(TIME_DELAY);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void
SDL_AppQuit(void* appstate, SDL_AppResult result) {
  return;
}
