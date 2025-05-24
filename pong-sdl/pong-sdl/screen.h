#ifndef __PONG_SDL_SCREEN__
#define __PONG_SDL_SCREEN__

#include "./ball.h"
#include "./player.h"

enum EGameMode { GameNotStated, GameInProgress, GamePaused, GameLost, GameQuit, GameRestart };
typedef enum EGameMode EGameMode;

typedef struct _screen Screen;

bool isLost(Screen*);
void moveScreenBall(Screen*);
Ball* getBall(Screen*);
Player* getLeftPlayer(Screen*);
Player* getRightPlayer(Screen*);

Screen* initScreen(size_t, size_t);
void deleteScreen(Screen*);
void startGame(Screen*);
void setGameToLost(Screen*);
void setNewGame(Screen*);
void restartGame(Screen*);
EGameMode getGameMode(Screen*);

#endif
