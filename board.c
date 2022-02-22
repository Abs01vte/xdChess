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
  return newBoard;
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
struct board *copyBoard(const struct board *board) {
  if (board == NULL) {
    return NULL;
  }
  struct board *newBoard = malloc(sizeof(*board));
  if (newBoard == NULL) {
    return NULL;
  }
  memcpy(newBoard, board, sizeof(*board));
  return newBoard;
}
bool rookCollision(const struct board *board, const struct move *move) {
  bool rank = false;
  bool up = false;
  bool left = false;
  struct board *newBoard = copyBoard(board);
  if (newBoard == NULL) {
    return true;
  }
  newBoard->tiles[move->rank][move->file - 'A'].piece = move->piece;
  newBoard->tiles[move->rank][move->file - 'A'].player = move->player;
  int file = move->file - 'A';

  for (int i = 0; i < 8; i++) {

    if (move->rank - i >= 0) {
      if (newBoard->tiles[move->rank - i][file].piece ==
              newBoard->tiles[move->rank][file].piece &&
          newBoard->tiles[move->rank - i][file].player ==
              newBoard->tiles[move->rank][file].player) {
        rank = false;
      }
    }
    if (move->rank + i < 8) {
      if (newBoard->tiles[move->rank + i][file].piece ==
              newBoard->tiles[move->rank][file].piece &&
          newBoard->tiles[move->rank + i][file].player ==
              newBoard->tiles[move->rank][file].player) {
        up = true;
        rank = false;
      }
    }
    if ((file)-i >= 0) {
      if (newBoard->tiles[move->rank][(file)-i].piece ==
              newBoard->tiles[move->rank][file].piece &&
          newBoard->tiles[move->rank][(file)-i].player ==
              newBoard->tiles[move->rank][file].player) {
        left = true;
        rank = true;
      }
    }
    if ((file) + i < 8) {
      if (newBoard->tiles[move->rank][file + i].piece ==
              newBoard->tiles[move->rank][file].piece &&
          newBoard->tiles[move->rank][file + i].player ==
              newBoard->tiles[move->rank][file].player) {
        rank = true;
      }
    }
  }
  if (rank) {
    for (int i = 0; i < 8; i++) {
      if (left) {
        if (newBoard->tiles[move->rank][file - i].piece != EMPTY ||
            newBoard->tiles[move->rank][file - i].piece != EMPTY ||
            (newBoard->tiles[move->rank][file - i].piece != ROOK &&
             newBoard->tiles[move->rank][file - i].player != move->player)) {
          printf("Wrong hole!\n");
          free(newBoard);
          return true;
        }
      } else {
        if (newBoard->tiles[move->rank][file + i].piece != EMPTY ||
            newBoard->tiles[move->rank][file + i].piece != EMPTY ||
            (newBoard->tiles[move->rank][file + i].piece != ROOK &&
             newBoard->tiles[move->rank][file + i].player != move->player)) {
          printf("its at least looking in the right direction\n");
          free(newBoard);
          return true;
        }
      }
    }
  }
  if (!rank) {
    for (int i = 0; i < 8; i++) {
      if (up) {
        if (newBoard->tiles[move->rank + i][file].piece != EMPTY ||
            newBoard->tiles[move->rank + i][file].piece != EMPTY ||
            (newBoard->tiles[move->rank + i][file].piece != ROOK &&
             newBoard->tiles[move->rank + i][file].player != move->player)) {
          free(newBoard);
          return true;
        }
      } else {
        if (newBoard->tiles[move->rank - i][file].piece != EMPTY ||
            newBoard->tiles[move->rank - i][file].piece != EMPTY ||
            (newBoard->tiles[move->rank - i][file].piece != ROOK &&
             newBoard->tiles[move->rank - i][file].player != move->player)) {
          free(newBoard);
          return true;
        }
      }
    }
  }
  free(newBoard);
  return false;
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
    // checking for first move double space move
    if (move->player == WHITE) {
      if (newBoard->tiles[move->rank - 2][move->file - 'A'].piece ==
              newBoard->tiles[move->rank][move->file - 'A'].piece &&
          newBoard->tiles[move->rank - 2][move->file - 'A'].player ==
              newBoard->tiles[move->rank][move->file - 'A'].player) {
        newBoard->tiles[move->rank - 2][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank - 2][move->file - 'A'].player = NONE;
        break;
      } else if (newBoard->tiles[move->rank - 1][move->file - 'A'].piece ==
                     newBoard->tiles[move->rank][move->file - 'A'].piece &&
                 newBoard->tiles[move->rank - 1][move->file - 'A'].player ==
                     newBoard->tiles[move->rank][move->file - 'A'].player) {
        printf("pawn removed\n");
        newBoard->tiles[move->rank - 1][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank - 1][move->file - 'A'].player = NONE;
        break;
      }
    }
    if (move->player == BLACK) {
      if (newBoard->tiles[move->rank + 2][move->file - 'A'].piece ==
              newBoard->tiles[move->rank][move->file - 'A'].piece &&
          newBoard->tiles[move->rank + 2][move->file - 'A'].player ==
              newBoard->tiles[move->rank][move->file - 'A'].player) {
        newBoard->tiles[move->rank + 2][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank + 2][move->file - 'A'].player = NONE;
        break;
      }
      if (newBoard->tiles[move->rank + 1][move->file - 'A'].piece ==
              newBoard->tiles[move->rank][move->file - 'A'].piece &&
          newBoard->tiles[move->rank + 1][move->file - 'A'].player ==
              newBoard->tiles[move->rank][move->file - 'A'].player) {
        printf("pawn removed\n");
        newBoard->tiles[move->rank + 1][move->file - 'A'].piece = EMPTY;
        newBoard->tiles[move->rank + 1][move->file - 'A'].player = NONE;
        break;
      }
    }
  } break;
  case KNIGHT:
    if (legalMove(move, board)) {
      // the new board has the new move added to the board, piece and side
      newBoard->tiles[move->rank][move->file - 'A'].player = move->player;
      newBoard->tiles[move->rank][move->file - 'A'].piece = move->piece;
      int moveFile = (int)move->file;
      // making sure that we remove a white knight where it was moved from
      if (newBoard->tiles[move->rank][moveFile - 'A'].player == WHITE) {
        if ((move->rank - 2 >= 0 || move->rank - 2 <= 7) &&
            (moveFile - '@' >= 0 || moveFile - '@' <= 7 ||
             moveFile - 'B' >= 0 || moveFile - 'B' <= 7)) {
          if (newBoard->tiles[move->rank - 2][moveFile - 'B'].piece == KNIGHT &&
              newBoard->tiles[move->rank - 2][moveFile - 'B'].player ==
                  move->player) {
            // checking if the old knight is two ranks down and one file right
            // from the present position of the new move on success: the spot
            // matching the placement of the old knight is returned to '-' on
            // the board and the player to none

            newBoard->tiles[move->rank - 2][moveFile - 'B'].piece = EMPTY;
            newBoard->tiles[move->rank - 2][moveFile - 'B'].player = NONE;
          }
          if (newBoard->tiles[move->rank - 2][moveFile - '@'].piece == KNIGHT &&
              newBoard->tiles[move->rank - 2][moveFile - '@'].player ==
                  move->player) {
            // checking if the old knight is two ranks down and one file left
            // from the present position of the new move on success: the spot
            // matching the placement of the old knight is returned to '-' on
            // the board and the player to none

            newBoard->tiles[move->rank - 2][moveFile - '@'].piece = EMPTY;
            newBoard->tiles[move->rank - 2][moveFile - '@'].player = NONE;
          }
          if (newBoard->tiles[move->rank - 2][moveFile - '@'].piece == KNIGHT &&
              newBoard->tiles[move->rank - 2][moveFile - '@'].player ==
                  move->player) {
            // checking if the old knight is two ranks down and one file left
            // from the present position of the new move on success: the spot
            // matching the placement of the old knight is returned to '-' on
            // the board and the player to none

            newBoard->tiles[move->rank - 2][moveFile - '@'].piece = EMPTY;
            newBoard->tiles[move->rank - 2][moveFile - '@'].player = NONE;
          }
        }
        if ((move->rank + 2 >= 0 || move->rank + 2 <= 7) &&
            (moveFile - '@' - 1 >= 0 || moveFile - '@' - 1 <= 7 ||
             moveFile - 'B' >= 0 || moveFile - 'B' + 1 <= 7)) {
          if (newBoard->tiles[move->rank + 2][moveFile - 'B'].piece == KNIGHT &&
              newBoard->tiles[move->rank + 2][moveFile - 'B'].player ==
                  move->player) {
            // checking if the old knight is two ranks up and one file right
            // from the present position of the new move on success: the spot
            // matching the placement of the old knight is returned to '-' on
            // the board and the player to none

            newBoard->tiles[move->rank + 2][moveFile - 'B'].piece = EMPTY;
            newBoard->tiles[move->rank + 2][moveFile - 'B'].player = NONE;
          }
          if (newBoard->tiles[move->rank + 2][moveFile - '@'].piece == KNIGHT &&
              newBoard->tiles[move->rank + 2][moveFile - '@'].player ==
                  move->player) {
            // checking if the old knight is two ranks up and one file left from
            // the present position of the new move
            // on success: the spot matching the placement of the old knight is
            // returned to '-' on the board and the player to none

            newBoard->tiles[move->rank + 2][moveFile - '@'].piece = EMPTY;
            newBoard->tiles[move->rank + 2][moveFile - '@'].player = NONE;
          }
        }
        if ((move->rank + 1 >= 0 || move->rank + 1 <= 7) &&
            (moveFile - '?' - 1 >= 0 || moveFile - '?' <= 7 ||
             moveFile - 'C' >= 0 || moveFile - 'C' <= 7)) {
          if (newBoard->tiles[move->rank + 1][moveFile - 'C'].piece == KNIGHT &&
              newBoard->tiles[move->rank + 1][moveFile - 'C'].player ==
                  move->player) {
            // checking if the old knight is one rank up and two files right
            // from the present position of the new move on success: the spot
            // matching the placement of the old knight is returned to '-' on
            // the board and the player to none

            newBoard->tiles[move->rank + 1][moveFile - 'C'].piece = EMPTY;
            newBoard->tiles[move->rank + 1][moveFile - 'C'].player = NONE;
          }
          if (newBoard->tiles[move->rank + 1][moveFile - '?'].piece == KNIGHT &&
              newBoard->tiles[move->rank + 1][moveFile - '?'].player ==
                  move->player) {
            // checking if the old knight is one rank up and two files left from
            // the present position of the new move
            // on success: the spot matching the placement of the old knight is
            // returned to '-' on the board and the player to none

            newBoard->tiles[move->rank + 1][moveFile - '?'].piece = EMPTY;
            newBoard->tiles[move->rank + 1][moveFile - '?'].player = NONE;
          }
          if (newBoard->tiles[move->rank - 1][moveFile - 'C'].piece == KNIGHT &&
              newBoard->tiles[move->rank - 1][moveFile - 'C'].player ==
                  move->player) {
            // checking if the old knight is one rank down and two files right
            // from the present position of the new move on success: the spot
            // matching the placement of the old knight is returned to '-' on
            // the board and the player to none
            newBoard->tiles[move->rank - 1][moveFile - 'C'].piece = EMPTY;
            newBoard->tiles[move->rank - 1][moveFile - 'C'].player = NONE;
          }
          if (newBoard->tiles[move->rank - 1][moveFile - '?'].piece == KNIGHT &&
              newBoard->tiles[move->rank - 1][moveFile - '?'].player ==
                  move->player) {
            // checking if the old knight is one rank down and two files left
            // from the present position of the new move on success: the spot
            // matching the placement of the old knight is returned to '-' on
            // the board and the player to none
            newBoard->tiles[move->rank - 1][moveFile - '?'].piece = EMPTY;
            newBoard->tiles[move->rank - 1][moveFile - '?'].player = NONE;
          }
        }
      }
      if (newBoard->tiles[move->rank][moveFile - 'A'].player == BLACK) {
        if (newBoard->tiles[move->rank - 2][moveFile - 'B'].piece == KNIGHT &&
            newBoard->tiles[move->rank - 2][moveFile - 'B'].player ==
                move->player) {
          // checking if the old knight is two ranks down and one file right
          // from the present position of the new move on success: the spot
          // matching the placement of the old knight is returned to '-' on the
          // board and the player to none
          newBoard->tiles[move->rank - 2][moveFile - 'B'].piece = EMPTY;
          newBoard->tiles[move->rank - 2][moveFile - 'B'].player = NONE;
        }
        if (newBoard->tiles[move->rank - 2][moveFile - '@'].piece == KNIGHT &&
            newBoard->tiles[move->rank - 2][moveFile - '@'].player ==
                move->player) {
          // checking if the old knight is two ranks down and one file left from
          // the present position of the new move
          // on success: the spot matching the placement of the old knight is
          // returned to '-' on the board and the player to none
          newBoard->tiles[move->rank - 2][moveFile - '@'].piece = EMPTY;
          newBoard->tiles[move->rank - 2][moveFile - '@'].player = NONE;
        }
        if (newBoard->tiles[move->rank + 2][moveFile - 'B'].piece == KNIGHT &&
            newBoard->tiles[move->rank + 2][moveFile - 'B'].player ==
                move->player) {
          // checking if the old knight is two ranks up and one file right from
          // the present position of the new move
          // on success: the spot matching the placement of the old knight is
          // returned to '-' on the board and the player to none
          newBoard->tiles[move->rank + 2][moveFile - 'B'].piece = EMPTY;
          newBoard->tiles[move->rank + 2][moveFile - 'B'].player = NONE;
        }
        if (newBoard->tiles[move->rank + 2][moveFile - '@'].piece == KNIGHT &&
            newBoard->tiles[move->rank + 2][moveFile - '@'].player ==
                move->player) {
          // checking if the old knight is two ranks up and one file left from
          // the present position of the new move
          // on success: the spot matching the placement of the old knight is
          // returned to '-' on the board and the player to none
          newBoard->tiles[move->rank + 2][moveFile - '@'].piece = EMPTY;
          newBoard->tiles[move->rank + 2][moveFile - '@'].player = NONE;
        }
        if (newBoard->tiles[move->rank + 1][moveFile - 'C'].piece == KNIGHT &&
            newBoard->tiles[move->rank + 1][moveFile - 'C'].player ==
                move->player) {
          // checking if the old knight is one rank up and two files right from
          // the present position of the new move
          // on success: the spot matching the placement of the old knight is
          // returned to '-' on the board and the player to none
          newBoard->tiles[move->rank + 1][moveFile - 'C'].piece = EMPTY;
          newBoard->tiles[move->rank + 1][moveFile - 'C'].player = NONE;
        }
        if (newBoard->tiles[move->rank + 1][moveFile - '?'].piece == KNIGHT &&
            newBoard->tiles[move->rank + 1][moveFile - '?'].player ==
                move->player) {
          // checking if the old knight is one rank up and two files left from
          // the present position of the new move
          // on success: the spot matching the placement of the old knight is
          // returned to '-' on the board and the player to none
          newBoard->tiles[move->rank + 1][moveFile - '?'].piece = EMPTY;
          newBoard->tiles[move->rank + 1][moveFile - '?'].player = NONE;
        }
        if (newBoard->tiles[move->rank - 1][moveFile - 'C'].piece == KNIGHT &&
            newBoard->tiles[move->rank - 1][moveFile - 'C'].player ==
                move->player) {
          // checking if the old knight is one rank down and two files right
          // from the present position of the new move on success: the spot
          // matching the placement of the old knight is returned to '-' on the
          // board and the player to none
          newBoard->tiles[move->rank - 1][moveFile - 'C'].piece = EMPTY;
          newBoard->tiles[move->rank - 1][moveFile - 'C'].player = NONE;
        }
        if (newBoard->tiles[move->rank - 1][moveFile - '?'].piece == KNIGHT &&
            newBoard->tiles[move->rank - 1][moveFile - '?'].player ==
                move->player) {
          // checking if the old knight is one rank down and two files left from
          // the present position of the new move
          // on success: the spot matching the placement of the old knight is
          // returned to '-' on the board and the player to none
          newBoard->tiles[move->rank - 1][moveFile - '?'].piece = EMPTY;
          newBoard->tiles[move->rank - 1][moveFile - '?'].player = NONE;
        }
      }

      break;
    case BISHOP:
      // the new board has the new move added to the board, piece and side
      newBoard->tiles[move->rank][move->file - 'A'].player = move->player;
      newBoard->tiles[move->rank][move->file - 'A'].piece = move->piece;
      // first, checking for the legality of the move
      if (legalMove(move, board)) {
        int file = move->file - 'A';
        for (int i = 1; i < 8; i++) {
          if (newBoard->tiles[move->rank - i][file - i].piece ==
                  newBoard->tiles[move->rank][move->file - 'A'].piece &&
              newBoard->tiles[move->rank - i][file - i].player ==
                  newBoard->tiles[move->rank][move->file - 'A'].player) {
            newBoard->tiles[move->rank - i][file - i].piece = EMPTY;
            newBoard->tiles[move->rank - i][file - i].player = NONE;
          }
          if (newBoard->tiles[move->rank + i][file - i].piece ==
                  newBoard->tiles[move->rank][move->file - 'A'].piece &&
              newBoard->tiles[move->rank + i][file - i].player ==
                  newBoard->tiles[move->rank][move->file - 'A'].player) {
            newBoard->tiles[move->rank + i][file - i].piece = EMPTY;
            newBoard->tiles[move->rank + i][file - i].player = NONE;
          }
          if (newBoard->tiles[move->rank - i][file + i].piece ==
                  newBoard->tiles[move->rank][move->file - 'A'].piece &&
              newBoard->tiles[move->rank - i][file + i].player ==
                  newBoard->tiles[move->rank][move->file - 'A'].player) {
            newBoard->tiles[move->rank - i][file + i].piece = EMPTY;
            newBoard->tiles[move->rank - i][file + i].player = NONE;
          }
          if (newBoard->tiles[move->rank + i][file + i].piece ==
                  newBoard->tiles[move->rank][move->file - 'A'].piece &&
              newBoard->tiles[move->rank + i][file + i].player ==
                  newBoard->tiles[move->rank][move->file - 'A'].player) {
            newBoard->tiles[move->rank + i][file + i].piece = EMPTY;
            newBoard->tiles[move->rank + i][file + i].player = NONE;
          }
        }
      }
      break;
    case ROOK:
      // the new board has the new move added to the board, piece and side
      newBoard->tiles[move->rank][move->file - 'A'].player = move->player;
      newBoard->tiles[move->rank][move->file - 'A'].piece = move->piece;
      // first, checking for the legality of the move
      if (legalMove(move, board)) {
        // ensuring that a collison on the file or rank that the rook
        // moves on does not happen
        if (!rookCollision(newBoard, move)) {
          for (int i = 0; i < 8; i++) {
            if (newBoard->tiles[move->rank - i][move->file - 'A'].piece ==
                    newBoard->tiles[move->rank][move->file - 'A'].piece &&
                newBoard->tiles[move->rank - i][move->file - 'A'].player ==
                    newBoard->tiles[move->rank][move->file - 'A'].player) {
              newBoard->tiles[move->rank - i][move->file - 'A'].player = NONE;
              newBoard->tiles[move->rank - i][move->file - 'A'].piece = EMPTY;
              break;
            }
            if (newBoard->tiles[move->rank + i][move->file - 'A'].piece ==
                    newBoard->tiles[move->rank][move->file - 'A'].piece &&
                newBoard->tiles[move->rank + i][move->file - 'A'].player ==
                    newBoard->tiles[move->rank][move->file - 'A'].player) {
              newBoard->tiles[move->rank + i][move->file - 'A'].player = NONE;
              newBoard->tiles[move->rank + i][move->file - 'A'].piece = EMPTY;
              break;
            }
            if (newBoard->tiles[move->rank][move->file - 'A' - i].piece ==
                    newBoard->tiles[move->rank][move->file - 'A'].piece &&
                newBoard->tiles[move->rank][move->file - 'A' - i].player ==
                    newBoard->tiles[move->rank][move->file - 'A'].player) {
              newBoard->tiles[move->rank][move->file - 'A' - i].player = NONE;
              newBoard->tiles[move->rank][move->file - 'A' - i].piece = EMPTY;
              break;
            }
            if (newBoard->tiles[move->rank][move->file - 'A' + i].piece ==
                    newBoard->tiles[move->rank][move->file - 'A'].piece &&
                newBoard->tiles[move->rank][move->file - 'A' + i].player ==
                    newBoard->tiles[move->rank][move->file - 'A'].player) {
              newBoard->tiles[move->rank][move->file - 'A' + i].player = NONE;
              newBoard->tiles[move->rank][move->file - 'A' + i].piece = EMPTY;
              break;
            }
          }
        } else {
          printf("Failure to detect rook in move: %s %s %c %d\n\n",
                 getPlayerString(move->player), getPieceString(move->piece),
                 (char)move->file, move->rank + 1);
          break;
        }
      }
      break;
    default:
      return newBoard;
      break;
    }
    if (move->flags & CASTLE) {
      if (move->player == WHITE) {
        newBoard->tiles[0][5].piece = ROOK;
        newBoard->tiles[0][5].player = WHITE;
        newBoard->tiles[0][6].piece = KING;
        newBoard->tiles[0][6].player = WHITE;
        newBoard->tiles[0][7].piece = EMPTY;
        newBoard->tiles[0][7].player = NONE;
        newBoard->tiles[0][4].piece = EMPTY;
        newBoard->tiles[0][4].player = NONE;
      }
      if (move->player == BLACK) {
        newBoard->tiles[7][5].piece = ROOK;
        newBoard->tiles[7][5].player = WHITE;
        newBoard->tiles[7][6].piece = KING;
        newBoard->tiles[7][6].player = WHITE;
        newBoard->tiles[7][7].piece = EMPTY;
        newBoard->tiles[7][7].player = NONE;
        newBoard->tiles[7][4].piece = EMPTY;
        newBoard->tiles[7][4].player = NONE;
      }
    }
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
