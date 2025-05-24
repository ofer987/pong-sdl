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

static Uint32 TEN_MILLISECONDS = 10;
static Uint64 ONE_HUNDRED_MILLISECONDS = 100;

static uint64_t rerenderedMovementFrames = 0;

static Screen* screen;
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

  screen = initScreen();

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
      case SDLK_R: restartGame(screen); return SDL_APP_CONTINUE;
      case SDLK_P:
        startGame(screen);
        /* set_current_movement(game, NOTHING); */
        /* set_game_mode(game, PAUSE); */

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
        case SDLK_A: movePlayerDown(leftPlayer); break;
        case SDLK_S: movePlayerUp(leftPlayer); break;
        case SDLK_J: movePlayerDown(rightPlayer); break;
        case SDLK_K: movePlayerUp(rightPlayer); break;
        default: break;
      }

      return SDL_APP_CONTINUE;
    }
  }

  return SDL_APP_CONTINUE;
}

void
render_record(size_t score) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugTextFormat(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT, "Record: %zu", score); */
}

void
render_text(size_t score) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugTextFormat(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 10, "Score:  %zu", score); */
}

void
render_keys(void) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 20, "Press:  (Q)uit | (R)estart | (P)ause"); */
}

void
render_lost_keys(void) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 20, "Press:  (Q)uit | (R)estart"); */
}

void
render_pause(void) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 30, "Game is Paused. Any key to continue"); */
}

void
render_quit(void) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 30, "Press Q again to Quit"); */
}

void
render_lost(void) {
  const float scale = 2.0f;

  SDL_SetRenderScale(renderer, scale, scale);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  /* SDL_RenderDebugText(renderer, 0, MAZE_HEIGHT + TEXT_HEIGHT_SHIFT + 30, "You have lost!"); */
}

/* void */
/* renderPlayer(Player* player) {} */

/* void */
/* renderScreen(Screen* screen) { */
/*   Pixel* pixel = NULL; */
/*   renderPlayer(screen->leftPlayer); */
/*   renderPlayer(screen->rightPlayer); */
/* } */

/* void */
void
render(Screen* screen) {
  // Reset
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Render screen
  Uint8 red = 0, green = 0, blue = 0;
  SDL_FRect myFrects[TOTAL_PIXELS];
  /* SDL_FRect* snake_head_rect = NULL; */

  /* Coordinates* screen_coordinates = screen[index]; */
  // TODO: adjust for 0-based later
  size_t index = 0;
  for (size_t row = TOP_PLAY_SCREEN; row < BOTTOM_PLAY_SCREEN; row += 1) {
    for (size_t column = LEFT_PLAY_SCREEN; column < RIGHT_PLAY_SCREEN; column += 1) {
      myFrects[index].x = column;
      myFrects[index].y = row;
      myFrects[index].w = 1;
      myFrects[index].h = 1;

      red = 127;
      green = 127;
      blue = 127;

      SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
      SDL_RenderFillRect(renderer, &myFrects[index]);

      index += 1;
    }
  }
}

void
renderBall(Ball* ball) {
  Uint64 red = 0, blue = 255, green = 0;

  Pixel* ballPixel = getBallPixel(ball);
  SDL_FRect ballRect = {.x = getPixelX(ballPixel), .y = getPixelY(ballPixel), .w = 1, .h = 1};

  SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
  SDL_RenderFillRect(renderer, &ballRect);
}

void
renderPlayer(Player* player) {
  Pixel* pixel = getPlayerTopPixel(player);
  size_t x = getPixelX(pixel);
  size_t y = getPixelY(pixel);

  size_t length = getPlayerLength(player);
  SDL_FRect myFrects[length];

  /* const float scale = 32.0f; */
  /* SDL_SetRenderScale(renderer, scale, scale); */

  static Uint64 red = 255, blue = 0, green = 0;
  for (size_t index = 0; index < length; index += 1) {
    SDL_FRect frect = myFrects[index];

    frect.x = x;
    frect.y = y + index;
    frect.w = 1;
    frect.h = 1;

    /* const float scale = 1.0f; */
    /* SDL_SetRenderScale(renderer, scale, scale); */
    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
    SDL_RenderFillRect(renderer, &frect);
  }
  /* SDL_RenderPresent(renderer); */
}

void
renderGameMode(EGameMode mode) {
  const float scale = 4.0f;
  SDL_SetRenderScale(renderer, scale, scale);
  size_t x = (LEFT_PLAY_SCREEN);
  size_t y = (BOTTOM_PLAY_SCREEN * 8) + 1;

  switch (mode) {
    case GameRestart:
      /* FALLTHROUGH */
    case GameNotStated: SDL_RenderDebugText(renderer, x, y, "Game Not Started"); break;
    case GameInProgress: SDL_RenderDebugText(renderer, x, y + 1, "In Progress"); break;
    default: break;
  }
}

void
renderLostText(Screen* screen) {
  const float scale = 4.0f;
  SDL_SetRenderScale(renderer, scale, scale);
  size_t x = (LEFT_PLAY_SCREEN);
  size_t y = (BOTTOM_PLAY_SCREEN * 8) + 1;

  SDL_RenderDebugText(renderer, x, y, "Lost!");
  SDL_RenderDebugText(renderer, x, y + 8, "Press R to (R)estart");
  SDL_RenderDebugText(renderer, x, y + 16, "Press Q to (Q)uit");
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult
SDL_AppIterate(void* appstate) {
  /* Snake* snake = get_snake_location(game); */
  Ball* ball = getBall(screen);

  const float scale = 32.0f;
  SDL_SetRenderScale(renderer, scale, scale);

  bool isGameLost = isLost(screen);
  if (isGameLost) {
    setGameToLost(screen);

    render(screen);
    renderPlayer(getLeftPlayer(screen));
    renderPlayer(getRightPlayer(screen));
    renderBall(ball);

    renderLostText(screen);

    SDL_RenderPresent(renderer);
    setNewGame(screen);

    return SDL_APP_CONTINUE;
  }

  // Move the snake
  bool isScreenRerendered = false;
  uint64_t newRerenderedMovementFrames = SDL_GetTicks();
  if (newRerenderedMovementFrames >= rerenderedMovementFrames + ONE_HUNDRED_MILLISECONDS) {
    isScreenRerendered = true;
    rerenderedMovementFrames = newRerenderedMovementFrames;
  }

  /* if (isGameLost) { */
  /*   renderLostText(screen); */
  /*  */
  /*   SDL_RenderPresent(renderer); */
  if (isScreenRerendered) {
    render(screen);
    renderPlayer(getLeftPlayer(screen));
    renderPlayer(getRightPlayer(screen));

    renderBall(ball);

    EGameMode gameMode = getGameMode(screen);
    renderGameMode(gameMode);

    SDL_RenderPresent(renderer);

    if (getGameMode(screen) == GameInProgress) {
      moveScreenBall(screen);
    }
  }

  SDL_Delay(TEN_MILLISECONDS);

  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void
SDL_AppQuit(void* appstate, SDL_AppResult result) {}

