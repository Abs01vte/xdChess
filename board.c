#include "board.h"
#include "moves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initilizing the board
struct board *makeBoard(void) {
  struct board *newBoard = malloc(sizeof(struct board));
  if (newBoard == NULL) {
    return NULL;
  }
  memset(newBoard, '-', sizeof(*newBoard));
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // this if is for the backranks of both sides
      if (i == 0 || i == 7) {
        if (j == 0 || j == 7) {
          newBoard->tiles[i][j].piece = ROOK;
        } else if (j == 1 || j == 6) {
          newBoard->tiles[i][j].piece = KNIGHT;
        } else if (j == 2 || j == 5) {
          newBoard->tiles[i][j].piece = BISHOP;
        } else if (j == 3) {
          newBoard->tiles[i][j].piece = QUEEN;
        } else {
          newBoard->tiles[i][j].piece = KING;
        }
      }
      // this if is for the pawn ranks of both sides
      else if (i == 1 || i == 6) {
        newBoard->tiles[i][j].piece = PAWN;
      } else {
        newBoard->tiles[i][j].piece = EMPTY;
      }
      if (i > 5) {
        newBoard->tiles[i][j].player = BLACK;
      } else if (i < 2) {
        newBoard->tiles[i][j].player = WHITE;
      } else {
        newBoard->tiles[i][j].player = NONE;
      }
    }
  }
}
// Displays the board
void printBoard(struct board *board) {
  printf("    A B C D E F G H\n");
  for (int i = 0; i < 8; i++) {
    printf("%d [ ", i + 1);
    for (int j = 0; j < 8; j++) {
      if (board->tiles[i][j].player == BLACK) {
        printf("\033[30;1;1m%c \033[0m", board->tiles[i][j].piece);
      } else if (board->tiles[i][j].player == WHITE) {
        printf("\033[97;1;1m%c \033[0m", board->tiles[i][j].piece);
      } else {
        printf("\033[0m%c ", board->tiles[i][j].piece);
      }
    }
    printf("]\n");
  }
}
struct board *updateBoard(const struct board *board, const struct move *move) {
  struct board *newBoard = makeBoard();
  if (newBoard == NULL || board == NULL) {
    return NULL;
  }
  *newBoard = *board;
  newBoard->tiles[move->file - 'A'][move->rank].player = move->player;
  newBoard->tiles[move->file - 'A'][move->rank].piece = move->piece;
  switch (move->piece) {
  case PAWN: {
    for (int i = 0; i < 8; i++) {
      if (newBoard->tiles[move->file - 'A'][i].piece ==
          newBoard->tiles[move->file - 'A'][move->rank - 1].piece) {
        if (newBoard->tiles[move->file - 'A'][i].player ==
            newBoard->tiles[move->file - 'A'][move->rank - 1].player) {
          newBoard->tiles[move->file - 'A'][i].piece = EMPTY;
          newBoard->tiles[move->file - 'A'][i].player = NONE;
        }

      } else {
        continue;
      }
    }
  } break;
  }
  return newBoard;
}
void destroyBoard(struct board *board) { free(board); }
