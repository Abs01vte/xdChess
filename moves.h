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
#define CASTLE 1
#define TAKES 2
#define PROMOTION 4
#define ENPASSANT 8
#define CHECK 16
#define CHECKMATE 32
struct move {
  // Player is the player that makes the move.
  enum player player;
  // Piece is the piece that is being moved.
  enum piece piece;
  // File is the letter, or vertical groupings on the board.
  enum file file;
  // Rank defines the horizontal groupings on the board
  int rank;
  // Special cases for a particular move
  unsigned flags;
};

struct linkedList *getList(FILE *file1, FILE *file2);
bool initMoves(void);
bool legalMove(const struct move *move, const struct board *board);
void printMove(const struct move *move);
char *moveToString(const struct move *move);
void quitMoves(void);

#endif
