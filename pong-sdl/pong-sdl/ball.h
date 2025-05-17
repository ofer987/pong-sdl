#ifndef __PONG_SDL_BALL__
#define __PONG_SDL_BALL__

#include "./pixel.h"
#include "./player.h"

typedef struct _ball Ball;

bool isBallOutOfBounds(Ball* ball, Player*, Player*);
void moveBall(Ball* ball, Player*, Player*);
Pixel* getBallPixel(Ball*);
void kickBall(Ball*);
void stopBall(Ball*);

Ball* initBall(void);
void destoryBall(Ball*);

#endif
