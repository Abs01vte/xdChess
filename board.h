#ifndef BOARD_H
#define BOARD_H
enum piece {
  EMPTY='-', PAWN='P', BISHOP='B', KNIGHT='N', ROOK='R', KING='K', QUEEN='Q'
};
enum player {
  NONE, WHITE, BLACK
};
struct squareTile {
  enum piece piece;
  enum player player;
};
struct board {
  struct squareTile tiles[8][8];
};
void printBoard(struct board*);
struct board* makeBoard(void);
#endif
