#ifndef __PONG_SDL_BORDER__
#define __PONG_SDL_BORDER__

#include <stdio.h>

enum EBorderSide { TOP_BORDER_SIDE, BOTTOM_BORDER_SIDE };

typedef struct _border Border;

enum EBorderSide getBorderSide(Border*);
size_t getBorderY(Border*);

Border* initBorder(size_t, enum EBorderSide);
void destroyBorder(Border*);

#endif
