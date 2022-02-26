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
// Imports and interprets list
struct linkedList *getList(FILE *file1, FILE *file2);
// prepares regex's for checking the character stream
// for moves
bool initMoves(void);
// determines if the move is allowed to play
bool legalMove(const struct move *move, const struct board *board);
// prints move for debugging purposes
void printMove(const struct move *move);
// puts the move in a string for debugging
char *moveToString(const struct move *move);
// frees all the moves
void quitMoves(void);

#endif
