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
  if (board == NULL) {
    return;
  }
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
  printf("\n");
}
struct board *updateBoard(const struct board *board, const struct move *move) {
  struct board *newBoard = makeBoard();
  if (newBoard == NULL || board == NULL) {
    return NULL;
  }
  *newBoard = *board;
  // appending the piece that moved to an empty space where it came from
  switch (move->piece) {
  case PAWN: {
    // the new board has the new move added to the board, piece and side
    newBoard->tiles[move->rank][move->file - 'A'].player = move->player;
    newBoard->tiles[move->rank][move->file - 'A'].piece = move->piece;
    if (newBoard->tiles[move->rank][move->file - 'A'].player == WHITE) {
      if (newBoard->tiles[move->rank - 2][move->file - 'A'].piece ==
          newBoard->tiles[move->rank][move->file - 'A'].piece) {
        newBoard->tiles[move->rank - 2][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank - 2][move->file - 'A'].player = NONE;
      }
      if (newBoard->tiles[move->rank - 1][move->file - 'A'].piece ==
          newBoard->tiles[move->rank][move->file - 'A'].piece) {
        newBoard->tiles[move->rank - 1][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank - 1][move->file - 'A'].player = NONE;
      }
    }
    if (newBoard->tiles[move->rank][move->file - 'A'].player == BLACK) {
      if (newBoard->tiles[move->rank + 2][move->file - 'A'].piece ==
          newBoard->tiles[move->rank][move->file - 'A'].piece) {
        newBoard->tiles[move->rank + 2][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank + 2][move->file - 'A'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 1][move->file - 'A'].piece ==
          newBoard->tiles[move->rank][move->file - 'A'].piece) {
        newBoard->tiles[move->rank + 1][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank + 1][move->file - 'A'].player = NONE;
      }
    }
  } break;
  case KNIGHT:
    if (move->piece == KNIGHT) {
      // the new board has the new move added to the board, piece and side
      newBoard->tiles[move->rank][move->file - 'A'].player = move->player;
      newBoard->tiles[move->rank][move->file - 'A'].piece = move->piece;
    }
    // making sure that we remove a white knight where it was moved from
    if (newBoard->tiles[move->rank][move->file - 'A'].player == WHITE) {
      if (newBoard->tiles[move->rank - 2][move->file - 'B'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 2][move->file - 'B'].player ==
              move->player) {
        // checking if the old knight is two ranks down and one file right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank - 2][move->file - 'B'].piece = EMPTY;
        newBoard->tiles[move->rank - 2][move->file - 'B'].player = NONE;
      }
      if (newBoard->tiles[move->rank - 2][move->file - '@'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 2][move->file - '@'].player ==
              move->player) {
        // checking if the old knight is two ranks down and one file left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank - 2][move->file - '@'].piece = EMPTY;
        newBoard->tiles[move->rank - 2][move->file - '@'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 2][move->file - 'B'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 2][move->file - 'B'].player ==
              move->player) {
        // checking if the old knight is two ranks up and one file right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank + 2][move->file - 'B'].piece = EMPTY;
        newBoard->tiles[move->rank + 2][move->file - 'B'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 2][move->file - '@'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 2][move->file - '@'].player ==
              move->player) {
        // checking if the old knight is two ranks up and one file left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank + 2][move->file - '@'].piece = EMPTY;
        newBoard->tiles[move->rank + 2][move->file - '@'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 1][move->file - 'C'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 1][move->file - 'C'].player ==
              move->player) {
        // checking if the old knight is one rank up and two files right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank + 1][move->file - 'C'].piece = EMPTY;
        newBoard->tiles[move->rank + 1][move->file - 'C'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 1][move->file - '?'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 1][move->file - '?'].player ==
              move->player) {
        // checking if the old knight is one rank up and two files left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank + 1][move->file - '?'].piece = EMPTY;
        newBoard->tiles[move->rank + 1][move->file - '?'].player = NONE;
      }
      if (newBoard->tiles[move->rank - 1][move->file - 'C'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 1][move->file - 'C'].player ==
              move->player) {
        // checking if the old knight is one rank down and two files right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank - 1][move->file - 'C'].piece = EMPTY;
        newBoard->tiles[move->rank - 1][move->file - 'C'].player = NONE;
      }
      if (newBoard->tiles[move->rank - 1][move->file - '?'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 1][move->file - '?'].player ==
              move->player) {
        // checking if the old knight is one rank down and two files left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none

        newBoard->tiles[move->rank - 1][move->file - '?'].piece = EMPTY;
        newBoard->tiles[move->rank - 1][move->file - '?'].player = NONE;
      }
    }
    if (newBoard->tiles[move->rank][move->file - 'A'].player == BLACK) {
      if (newBoard->tiles[move->rank - 2][move->file - 'B'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 2][move->file - 'B'].player ==
              move->player) {
        // checking if the old knight is two ranks down and one file right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank - 2][move->file - 'B'].piece = EMPTY;
        newBoard->tiles[move->rank - 2][move->file - 'B'].player = NONE;
      }
      if (newBoard->tiles[move->rank - 2][move->file - '@'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 2][move->file - '@'].player ==
              move->player) {
        // checking if the old knight is two ranks down and one file left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank - 2][move->file - '@'].piece = EMPTY;
        newBoard->tiles[move->rank - 2][move->file - '@'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 2][move->file - 'B'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 2][move->file - 'B'].player ==
              move->player) {
        // checking if the old knight is two ranks up and one file right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank + 2][move->file - 'B'].piece = EMPTY;
        newBoard->tiles[move->rank + 2][move->file - 'B'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 2][move->file - '@'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 2][move->file - '@'].player ==
              move->player) {
        // checking if the old knight is two ranks up and one file left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank + 2][move->file - '@'].piece = EMPTY;
        newBoard->tiles[move->rank + 2][move->file - '@'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 1][move->file - 'C'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 1][move->file - 'C'].player ==
              move->player) {
        // checking if the old knight is one rank up and two files right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank + 1][move->file - 'C'].piece = EMPTY;
        newBoard->tiles[move->rank + 1][move->file - 'C'].player = NONE;
      }
      if (newBoard->tiles[move->rank + 1][move->file - '?'].piece == KNIGHT &&
          newBoard->tiles[move->rank + 1][move->file - '?'].player ==
              move->player) {
        // checking if the old knight is one rank up and two files left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank + 1][move->file - '?'].piece = EMPTY;
        newBoard->tiles[move->rank + 1][move->file - '?'].player = NONE;
      }
      if (newBoard->tiles[move->rank - 1][move->file - 'C'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 1][move->file - 'C'].player ==
              move->player) {
        // checking if the old knight is one rank down and two files right from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank - 1][move->file - 'C'].piece = EMPTY;
        newBoard->tiles[move->rank - 1][move->file - 'C'].player = NONE;
      }
      if (newBoard->tiles[move->rank - 1][move->file - '?'].piece == KNIGHT &&
          newBoard->tiles[move->rank - 1][move->file - '?'].player ==
              move->player) {
        // checking if the old knight is one rank down and two files left from
        // the present position of the new move
        // on success: the spot matching the placement of the old knight is
        // returned to '-' on the board and the player to none
        newBoard->tiles[move->rank - 1][move->file - '?'].piece = EMPTY;
        newBoard->tiles[move->rank - 1][move->file - '?'].player = NONE;
      }
    }
    break;
  default:
    return newBoard;
    break;
  }
  return newBoard;
}
const char *getPieceString(enum piece piece) {
  switch (piece) {
  case QUEEN:
    return "Queen";
    break;
  case KING:
    return "King";
    break;
  case ROOK:
    return "Rook";
    break;
  case KNIGHT:
    return "Knight";
    break;
  case BISHOP:
    return "Bishop";
    break;
  case PAWN:
    return "Pawn";
    break;
  case EMPTY:
    return "No piece";
    break;
  }
  return "Invalid piece";
}
const char *getPlayerString(enum player player) {
  switch (player) {
  case WHITE:
    return "White";
    break;
  case BLACK:
    return "Black";
    break;
  case NONE:
    return "No player";
    break;
  }
  return "Invalid player";
}
void destroyBoard(struct board *board) { free(board); }
