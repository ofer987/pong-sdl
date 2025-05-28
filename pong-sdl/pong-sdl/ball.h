#ifndef __PONG_SDL_BALL__
#define __PONG_SDL_BALL__

#include "./border.h"
#include "./pixel.h"
#include "./player.h"

typedef struct _ball Ball;

void reinitBall(Ball*, enum EPlayerSide);
enum EPlayerSide isBallOutOfBounds(Ball* ball, Player*, Player*);
void moveBall(Ball* ball, Player* leftPlayer, Player* rightPlayer, Border* topBorder, Border* bottomBorder);
Pixel* getBallPixel(Ball*);
void kickBall(Ball*);
void stopBall(Ball*);

Ball* initBall(void);
void destoryBall(Ball*);

#endif
