#ifndef MOVES_H
#define MOVES_H

#include "board.h"
#include <stdbool.h>
#include <stdio.h>
enum file {
  A = 'A',
  B = 'B',
  C = 'C',
  D = 'D',
  E = 'E',
  F = 'F',
  G = 'G',
  H = 'H',
  FILELESS
};
struct move {
  // Player is the player that makes the move.
  enum player player;
  // Piece is the piece that is being moved.
  enum piece piece;
  // File is the letter, or vertical groupings on the board.
  enum file file;
  // Rank defines the horizontal groupings on the board. Negative one means
  // castling.
  int rank;
};

struct linkedList *getList(FILE *file1, FILE *file2);
bool initMoves(void);

void quitMoves(void);

#endif
