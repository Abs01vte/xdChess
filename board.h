#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h>

struct move;
enum piece {
  EMPTY = '-',
  PAWN = 'P',
  BISHOP = 'B',
  KNIGHT = 'N',
  ROOK = 'R',
  KING = 'K',
  QUEEN = 'Q'
};
enum player { NONE, WHITE, BLACK };
struct squareTile {
  enum piece piece;
  enum player player;
};
struct board {
  struct squareTile tiles[8][8];
};
void printBoard(const struct board *);
struct board *makeBoard(void);
/*
 * This takes in a board and creates a new one in order to return states
 * in so doing, going back a move. Doing this, you can go forwards
 * and backwards in state, allowing for checking which pieces were taken,
 * openings, other moves, or just general examination.
 * board: is the chessboard, with the pieces in a certain state.
 * move: move that is applied to the board state.
 * return: the updated board state as a new malloc'd board. NULL on error.
 */
struct board *updateBoard(const struct board *board, const struct move *move);
struct board *copyBoard(const struct board *board);
// determines if the move is allowed to play
bool legalMove(const struct move *move, const struct board *board);
const char *getPieceString(enum piece piece);
const char *getPlayerString(enum player player);
void destroyBoard(struct board *);
#endif
