#ifndef MOVES_H
#define MOVES_H

#include "board.h"
enum rank {
  A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7
};
struct movesList {
  enum piece piece;
  enum rank rank;
  int file;
};

#endif
