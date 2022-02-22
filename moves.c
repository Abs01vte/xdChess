#include "board.h"
#include "list.h"
#include "moves.h"
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

regex_t firstCharacter;
regex_t numberMatcher;
regex_t secondCharacter;
regex_t thirdCharacter;
regex_t fourthCharacter;
regex_t promotion;
regex_t castle;

enum noteState {
  PAWNSTATE,
  PIECESTATE,
  CASTLESTATE,
  TAKESTATE,
  PROMOTIONSTATE,
  ERRORSTATE,
  IDLESTATE
};

bool initMoves(void) {
  int value;

  // Function call to create regex
  value = regcomp(&firstCharacter, "Q|K|N|B|R|a|b|c|d|e|f|g|h", REG_EXTENDED);

  // Else for Compilation error
  if (value != 0) {
    fprintf(stderr, "Compilation error in first character regex\n");
    return false;
  }

  value = regcomp(&numberMatcher, "1|2|3|4|5|6|7|8", REG_EXTENDED);
  if (value != 0) {
    fprintf(stderr, "Compilation error in number matcher regex\n");
    return false;
  }
  value = regcomp(&secondCharacter, "x|1|2|3|4|5|6|7|8|a|b|c|d|e|f|g|h",
                  REG_EXTENDED);
  if (value != 0) {
    fprintf(stderr, "Compilation error in second character regex\n");
    return false;
  }
  value = regcomp(&thirdCharacter, "\\+|=|#|1|2|3|4|5|6|7|8|a|b|c|d|e|f|g|h",
                  REG_EXTENDED);
  if (value != 0) {
    fprintf(stderr, "Compilation error in third character regex\n");
    return false;
  }
  value = regcomp(&fourthCharacter, "\\+|=|#|1|2|3|4|5|6|7|8|Q|R|N|B",
                  REG_EXTENDED);
  if (value != 0) {
    fprintf(stderr, "Compilation error in fourth character regex\n");
    return false;
  }
  value = regcomp(&promotion, "Q|R|N|B", REG_EXTENDED);
  if (value != 0) {
    fprintf(stderr, "Compilation error in promotion character regex\n");
    return false;
  }
  value = regcomp(&castle, "O|-", REG_EXTENDED);
  if (value != 0) {
    fprintf(stderr, "Compilation error in castle character regex\n");
    return false;
  }
  return true;
}
struct move *copyMove(const struct move *move) {
  struct move *newMove = malloc(sizeof(*move));
  memcpy(newMove, move, sizeof(*move));
  if (newMove == NULL) {
    return NULL;
  }
  return newMove;
}

bool legalMove(const struct move *move, const struct board *board) {
  struct move *newMove = copyMove(move);
  switch (move->piece) {
  case PAWN:
    if (newMove->player == WHITE) {
      // checking to see if the move is doing the two rank first move for
      // pawns
      if (board->tiles[move->rank - 2][move->file - 'A'].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank - 2][move->file - 'A'].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        if (move->rank - 2 == 1) {
          return true;
        }
      }
      // checking for normal pawn moves
      if (board->tiles[move->rank - 1][move->file - 'A'].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank - 1][move->file - 'A'].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
    }
    if (newMove->player == BLACK) {
      // checking to see if the move is doing the two rank first move for
      // pawns
      if (board->tiles[move->rank + 2][move->file - 'A'].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank + 2][move->file - 'A'].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        if (move->rank + 2 == 7) {
          return true;
        }
      }
      // checking for normal pawn moves
      if (board->tiles[move->rank + 1][move->file - 'A'].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank + 1][move->file - 'A'].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
    }
    break;
  case KNIGHT:
    // checking if the old knight is two ranks down and one file right from
    // the present position of the new move
    if (board->tiles[move->rank - 2][move->file - 'B'].piece == KNIGHT &&
        board->tiles[move->rank - 2][move->file - 'B'].player == move->player) {
      return true;
    }
    // checking if the old knight is two ranks down and one file left from
    // the present position of the new move
    if (board->tiles[move->rank - 2][move->file - '@'].piece == KNIGHT &&
        board->tiles[move->rank - 2][move->file - '@'].player == move->player) {
      return true;
    }
    // checking if the old knight is two ranks up and one file right from
    // the present position of the new move
    if (board->tiles[move->rank + 2][move->file - 'B'].piece == KNIGHT &&
        board->tiles[move->rank + 2][move->file - 'B'].player == move->player) {
      return true;
    }
    // checking if the old knight is one rank up and two files right from
    // the present position of the new move
    if (board->tiles[move->rank + 2][move->file - '@'].piece == KNIGHT &&
        board->tiles[move->rank + 2][move->file - '@'].player == move->player) {
      return true;
    }
    // checking if the old knight is one rank up and two files left from
    // the present position of the new move
    if (board->tiles[move->rank + 1][move->file - 'C'].piece == KNIGHT &&
        board->tiles[move->rank + 1][move->file - 'C'].player == move->player) {
      return true;
    }
    // checking if the old knight is one rank down and two files right from
    // the present position of the new move
    if (board->tiles[move->rank + 1][move->file - '?'].piece == KNIGHT &&
        board->tiles[move->rank + 1][move->file - '?'].player == move->player) {
      return true;
    }
    // checking if the old knight is one rank down and two files left from
    // the present position of the new move
    if (board->tiles[move->rank - 1][move->file - 'C'].piece == KNIGHT &&
        board->tiles[move->rank - 1][move->file - 'C'].player == move->player) {
      return true;
    }
    // checking if the old knight is two ranks down and one file right from
    // the present position of the new move
    if (board->tiles[move->rank - 1][move->file - '?'].piece == KNIGHT &&
        board->tiles[move->rank - 1][move->file - '?'].player == move->player) {
      return true;
    }
    break;
  case BISHOP:
    int file = move->file - 'A';
    for (int i = 0; i < 8; i++) {
      // checking down and right
      if (board->tiles[move->rank - i][file - i].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank - i][file - i].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
      // checking up and right
      if (board->tiles[move->rank + i][file - i].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank + i][file - i].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
      // checking down and left
      if (board->tiles[move->rank - i][file + i].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank - i][file + i].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
      // checking up and left
      if (board->tiles[move->rank + i][file + i].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank + i][file + i].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
    }
    break;
  case ROOK:
    for (int i = 0; i < 8; i++) {
      // checking up on the same file, returning if it finds one
      if (board->tiles[move->rank - i][move->file - 'A'].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank - i][move->file - 'A'].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
      // checking down on the same file, returning if it finds one
      if (board->tiles[move->rank + i][move->file - 'A'].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank + i][move->file - 'A'].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
      // checking left on the same rank, returning if it finds one
      if (board->tiles[move->rank][move->file - 'A' - i].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank][move->file - 'A' - i].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
      // checking right on the same rank, returning if it finds one
      if (board->tiles[move->rank][move->file - 'A' + i].piece ==
              board->tiles[move->rank][move->file - 'A'].piece &&
          board->tiles[move->rank][move->file - 'A' + i].player ==
              board->tiles[move->rank][move->file - 'A'].player) {
        return true;
      }
    }
    break;
  case KING:
    int moveFile = move->file - 'A';
    if (move->player == WHITE) {
      if ((move->rank + 1 <= 7) && (moveFile - 1 >= 0 || moveFile + 1 <= 7)) {
        if (board->tiles[move->rank + 1][moveFile + 1].piece == move->piece &&
            board->tiles[move->rank + 1][moveFile + 1].player == move->player) {
          return true;
        }
        if (board->tiles[move->rank + 1][moveFile].piece == move->piece &&
            board->tiles[move->rank + 1][moveFile].player == move->player) {
          return true;
        }
        if (board->tiles[move->rank + 1][moveFile - 1].piece == move->piece &&
            board->tiles[move->rank + 1][moveFile - 1].player == move->player) {
          return true;
        }
      }
      if (moveFile - 1 >= 0 || moveFile + 1 <= 7) {
        if (board->tiles[move->rank][moveFile - 1].piece == move->piece &&
            board->tiles[move->rank][moveFile - 1].player == move->player) {
          return true;
        }
        if (board->tiles[move->rank][moveFile + 1].piece == move->piece &&
            board->tiles[move->rank][moveFile + 1].player == move->player) {
          return true;
        }
        if ((move->rank + 1 <= 7 || move->rank >= 0) &&
            (moveFile - 1 >= 0 || moveFile + 1 <= 7)) {
          if (board->tiles[move->rank - 1][moveFile - 1].piece == move->piece &&
              board->tiles[move->rank - 1][moveFile - 1].player ==
                  move->player) {
            return true;
          }
          if (board->tiles[move->rank - 1][moveFile].piece == move->piece &&
              board->tiles[move->rank - 1][moveFile].player == move->player) {
            return true;
          }
          if (board->tiles[move->rank - 1][moveFile + 1].piece == move->piece &&
              board->tiles[move->rank - 1][moveFile + 1].player ==
                  move->player) {
            return true;
          }
        }
      } else {
        printf("Move %s not found", moveToString(move));
        return false;
      }
    }
    if (move->player == BLACK) {
      if ((move->rank + 1 <= 7) && (moveFile - 1 >= 0 || moveFile + 1 <= 7)) {
        if (board->tiles[move->rank + 1][moveFile + 1].piece == move->piece &&
            board->tiles[move->rank + 1][moveFile + 1].player == move->player) {
          return true;
        }
        if (board->tiles[move->rank + 1][moveFile].piece == move->piece &&
            board->tiles[move->rank + 1][moveFile].player == move->player) {
          return true;
        }
        if (board->tiles[move->rank + 1][moveFile - 1].piece == move->piece &&
            board->tiles[move->rank + 1][moveFile - 1].player == move->player) {
          return true;
        }
      }
      if (moveFile - 1 >= 0 || moveFile + 1 <= 7) {
        if (board->tiles[move->rank][moveFile - 1].piece == move->piece &&
            board->tiles[move->rank][moveFile - 1].player == move->player) {
          return true;
        }
        if (board->tiles[move->rank][moveFile + 1].piece == move->piece &&
            board->tiles[move->rank][moveFile + 1].player == move->player) {
          return true;
        }
        if ((move->rank + 1 <= 7 || move->rank >= 0) &&
            (moveFile - 1 >= 0 || moveFile + 1 <= 7)) {
          if (board->tiles[move->rank - 1][moveFile - 1].piece == move->piece &&
              board->tiles[move->rank - 1][moveFile - 1].player ==
                  move->player) {
            return true;
          }
          if (board->tiles[move->rank - 1][moveFile].piece == move->piece &&
              board->tiles[move->rank - 1][moveFile].player == move->player) {
            return true;
          }
          if (board->tiles[move->rank - 1][moveFile + 1].piece == move->piece &&
              board->tiles[move->rank - 1][moveFile + 1].player ==
                  move->player) {
            return true;
          }
        } else {
          printf("Move %s not found", moveToString(move));
          return false;
        }
      }
      break;
    default:
      break;
    }
  }
  return false;
}
void printMove(const struct move *move) {
  char *moveString = moveToString(move);
  printf("Board after playing the move: %s\n\n", moveString);
  free(moveString);
}
char *moveToString(const struct move *move) {
  int size =
      snprintf(NULL, 0, "%s %s %c%d", getPlayerString(move->player),
               getPieceString(move->piece), (char)move->file, move->rank + 1);
  char *string = malloc(size + 1);
  if (string == NULL) {
    return NULL;
  }
  snprintf(string, size + 1, "%s %s %c%d", getPlayerString(move->player),
           getPieceString(move->piece), (char)move->file, move->rank + 1);
  return string;
}
void quitMoves(void) {
  regfree(&firstCharacter);
  regfree(&numberMatcher);
  regfree(&secondCharacter);
  regfree(&thirdCharacter);
  regfree(&fourthCharacter);
  regfree(&promotion);
}

enum noteState getFirstPart(struct move *move, char c) {
  if (islower(c) != 0) {
    move->piece = PAWN;
    // if the first character is a file designation
    // then it is a pawn
    switch (c) {
    case 'a':
      move->file = A;
      break;
    case 'b':
      move->file = B;
      break;
    case 'c':
      move->file = C;
      break;
    case 'd':
      move->file = D;
      break;
    case 'e':
      move->file = E;
      break;
    case 'f':
      move->file = F;
      break;
    case 'g':
      move->file = G;
      break;
    case 'h':
      move->file = H;
      break;
    default:
      fprintf(stderr, "Error finding pawn move %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
    return PAWNSTATE;
    // but if it is an "O", it designates castling
  } else if (c == 'O') {
    return CASTLESTATE;
  } else {
    // or it is a piece designation
    switch (c) {
    case 'Q':
      move->piece = QUEEN;
      break;
    case 'K':
      move->piece = KING;
      break;
    case 'N':
      move->piece = KNIGHT;
      break;
    case 'B':
      move->piece = BISHOP;
      break;
    case 'R':
      move->piece = ROOK;
      break;
    default:
      fprintf(stderr, "Error could not find piece %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
    return PIECESTATE;
  }

  return ERRORSTATE;
}
enum noteState getSecondPart(struct move *move, char c) {
  if (islower(c) != 0) {
    // if the second character is a file designation, it is a piece
    switch (c) {
    case 'a':
      move->file = A;
      break;
    case 'b':
      move->file = B;
      break;
    case 'c':
      move->file = C;
      break;
    case 'd':
      move->file = D;
      break;
    case 'e':
      move->file = E;
      break;
    case 'f':
      move->file = F;
      break;
    case 'g':
      move->file = G;
      break;
    case 'h':
      move->file = H;
      break;
    case 'x':
      return TAKESTATE;
      break;
    default:
      fprintf(stderr, "Error finding file for move %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
    return PIECESTATE;
  } else {
    // or if it is a rank designation, it is a pawn
    switch (c) {
    case '1':
      move->rank = 0;
      break;
    case '2':
      move->rank = 1;
      break;
    case '3':
      move->rank = 2;
      break;
    case '4':
      move->rank = 3;
      break;
    case '5':
      move->rank = 4;
      break;
    case '6':
      move->rank = 5;
      break;
    case '7':
      move->rank = 6;
      break;
    case '8':
      move->rank = 7;
      break;
    default:
      fprintf(stderr, "Error finding rank for move %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
    return PAWNSTATE;
  }
  return PIECESTATE;
}
enum noteState getThirdPart(struct move *move, char c) {
  if (islower(c) != 0) {
    // if the third character is a file designation
    // then it is a disambiguation for a piece or pawn move
    switch (c) {
    case 'a':
      move->file = A;
      break;
    case 'b':
      move->file = B;
      break;
    case 'c':
      move->file = C;
      break;
    case 'd':
      move->file = D;
      break;
    case 'e':
      move->file = E;
      break;
    case 'f':
      move->file = F;
      break;
    case 'g':
      move->file = G;
      break;
    case 'h':
      move->file = H;
      break;
    default:
      fprintf(stderr, "Error finding file for move %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
  } else {
    // or if it is a rank designation, it is a piece that has a take state
    switch (c) {

    case '1':
      move->rank = 0;
      break;
    case '2':
      move->rank = 1;
      break;
    case '3':
      move->rank = 2;
      break;
    case '4':
      move->rank = 3;
      break;
    case '5':
      move->rank = 4;
      break;
    case '6':
      move->rank = 5;
      break;
    case '7':
      move->rank = 6;
      break;
    case '8':
      move->rank = 7;
      break;
    case '+':
      printf("Check!\n");
      break;
    case '#':
      printf("Checkmate! The king is dead!\n");
      break;
    case '=':
      return PROMOTIONSTATE;
      break;
    default:
      fprintf(stderr, "Error finding rank for move %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
  }
  return ERRORSTATE;
}

enum noteState getFourthPart(struct move *move, char c) {
  if (isupper(c) != 0) {
    switch (c) {
      // this is only for promotion
    case 'Q':
      move->piece = QUEEN;
      break;
    case 'N':
      move->piece = KNIGHT;
      break;
    case 'B':
      move->piece = BISHOP;
      break;
    case 'R':
      move->piece = ROOK;
      break;
    default:
      fprintf(stderr, "Error could not find piece %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
    return PIECESTATE;
  } else {
    switch (c) {
      // or takes into check, promotion  or disambiguation ranks
    case '1':
      move->rank = 0;
      break;
    case '2':
      move->rank = 1;
      break;
    case '3':
      move->rank = 2;
      break;
    case '4':
      move->rank = 3;
      break;
    case '5':
      move->rank = 4;
      break;
    case '6':
      move->rank = 5;
      break;
    case '7':
      move->rank = 6;
      break;
    case '8':
      move->rank = 7;
      break;
    case '+':
      printf("Check!\n");
      break;
    case '#':
      printf("Checkmate! The king is dead!\n");
      break;
    case '=':
      return PROMOTIONSTATE;
      break;
    default:
      fprintf(stderr, "Error finding rank for move %c (%d)\n", c, (int)c);
      return ERRORSTATE;
      break;
    }
  }
  return ERRORSTATE;
}
enum noteState getPromotion(struct move *move, char c) {
  switch (c) {
  case 'Q':
    move->piece = QUEEN;
    break;
  case 'N':
    move->piece = KNIGHT;
    break;
  case 'B':
    move->piece = BISHOP;
    break;
  case 'R':
    move->piece = ROOK;
    break;
  default:
    fprintf(stderr, "Error could not find piece %c (%d)\n", c, (int)c);
    return ERRORSTATE;
    break;
  }
  return PROMOTIONSTATE;
}
// resetting moves for the time the program interprets moves
// in a list
#define RESET_MOVE                                                             \
  state = IDLESTATE;                                                           \
  move.player = move.player == WHITE ? BLACK : WHITE;                          \
  move = (struct move){                                                        \
      .player = move.player, .piece = EMPTY, .rank = 0, .file = FILELESS};
struct linkedList *getList(FILE *file1, FILE *file2) {
  char buff[512];
  enum noteState state = IDLESTATE;
  struct move move = (struct move){
      .player = WHITE, .piece = EMPTY, .rank = 0, .file = FILELESS, .flags = 0};
  struct linkedList *list = makeList(sizeof(struct move));
  if (list == NULL) {
    return NULL;
  }
  // Extremely simple algebraic chess notation parser.
  for (char *retVal = fgets(buff, sizeof(buff), file1); retVal != NULL;
       retVal = fgets(buff, sizeof(buff), file1)) {
    size_t buffLen = strnlen(buff, sizeof(buff));
    if (buff[buffLen - 1] == '\n')
      buffLen--;
    for (size_t i = 0; i < buffLen; i++) {
      // Match the next character to a regular expression.
      // This is very simple due to all chess information being only one
      // character in length.

      // Constant string that represents the current character.
      const char curStr[] = {buff[i], '\0'};
      bool checkState = buff[i] == '+' || buff[i] == '#';
      switch (state) {
      case IDLESTATE:
        if (regexec(&firstCharacter, curStr, 0, NULL, 0) == 0) {
          state = getFirstPart(&move, buff[i]);
        }
        if (regexec(&castle, curStr, 0, NULL, 0) == 0) {
          state = CASTLESTATE;
        }
        break;
      case PAWNSTATE:
        if (regexec(&numberMatcher, curStr, 0, NULL, 0) == 0) {
          move.rank = buff[i] - '1';
          addList(&move, list);
        }
        if (regexec(&secondCharacter, curStr, 0, NULL, 0) == 0) {
          if (buff[i + 1] == 'x') {
            state = TAKESTATE;
            i++;
            break;
          }
        }
        RESET_MOVE;
        break;
      case PIECESTATE:
        if (regexec(&secondCharacter, curStr, 0, NULL, 0) == 0) {
          state = getSecondPart(&move, buff[i]);
          state = IDLESTATE;
        }
        if (regexec(&thirdCharacter, curStr, 0, NULL, 0) == 0) {
          state = getThirdPart(&move, buff[i + 1]);
          addList(&move, list);
          RESET_MOVE;
          i++;
        }
        if (checkState && move.rank != 0 && move.file != FILELESS) {
          // TODO: check
          addList(&move, list);
          RESET_MOVE;
        }
        // TODO: Takes, disambiguation
        break;
      case PROMOTIONSTATE:
        if (regexec(&promotion, curStr, 0, NULL, 0) == 0) {
          move.piece = buff[i];
        }
        break;
      case CASTLESTATE:
        move.flags = CASTLE;
        addList(&move, list);
        RESET_MOVE;
        printf("we outtie");
        break;
      case TAKESTATE:
        if (move.piece == PAWN) {
          move.rank = (char)buff[i + 2];
          move.file = (char)buff[i + 1];
          move.flags = TAKES;
          RESET_MOVE;
          i += 2;
          break;
        }
        if (move.piece != PAWN && move.piece != EMPTY) {
          move.rank = (char)buff[i + 2];
          move.file = (char)buff[i + 1];
          move.flags = TAKES;
          RESET_MOVE;
          i += 2;
          break;
        }
        break;
      default:
        fprintf(stderr, "UNKNOWN STATE: %d\n", state);
        // fall through
      case ERRORSTATE:
        state = IDLESTATE;
        break;
      }
    }
  }
  return list;
}
