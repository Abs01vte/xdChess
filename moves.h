#ifndef MOVES_H
#define MOVES_H

#include "board.h"
#include <stdbool.h>
#include <stdio.h>
enum file {
  FILELESS = 0,
  A = 'A',
  B = 'B',
  C = 'C',
  D = 'D',
  E = 'E',
  F = 'F',
  G = 'G',
  H = 'H',
};
#define CHESS_ERROR 0
#define CHESS_NORMAL 1
#define CASTLE 2
#define TAKES 4
#define PROMOTION 8
#define ENPASSANT 16
#define CHECK 32
#define CHECKMATE 64
struct move {
  // Piece is the piece that is being moved.
  enum piece piece;
  // File is the letter, or vertical groupings on the board.
  enum file file;
  // Rank defines the horizontal groupings on the board
  int rank;
  // Disambiguating file.
  enum file disFile;
  // Disambiguating rank.
  int disRank;
  // Special cases for a particular move
  unsigned flags;
};

#define DEFAULT_PIECE (struct move) { .piece = 0, .file = 0, .rank = 0, .disFile = 0,\
            .disRank = 0, .flags = CHESS_ERROR }

struct moveTuple {
    struct move m1;
    struct move m2;
};
struct move pullMove(FILE *file);


struct moveTuple pullMoves(FILE *file1, FILE *file2);

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
