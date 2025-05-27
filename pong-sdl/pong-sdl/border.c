#include <stdint.h>
#include <stdlib.h>

#include "./border.h"

struct _border {
  enum EBorderSide side;
  int32_t y;
};

enum EBorderSide
getBorderSide(Border* border) {
  return border->side;
}

size_t
getBorderY(Border* border) {
  return border->y;
}

Border*
initBorder(size_t y, enum EBorderSide side) {
  Border* result = malloc(sizeof(Border));

  result->y = y;
  result->side = side;

  return result;
}

void
destroyBorder(Border* border) {
  free(border);
}
