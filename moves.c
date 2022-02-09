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
  value = regcomp(&firstCharacter, "Q|K|N|B|R|a|b|c|d|e|f|g|h|O", REG_EXTENDED);

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
  return true;
}
void printMove(const struct move *move) {
  printf("Playing move: %s %s %c %d\n", getPlayerString(move->player),
         getPieceString(move->piece), (char)move->file, move->rank + 1);
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
  } else if (c == 'O') {
    move->rank = -1;
    return CASTLESTATE;
  } else {
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
    switch (c) {
    case '1':
      move->rank = 1;
      break;
    case '2':
      move->rank = 2;
      break;
    case '3':
      move->rank = 3;
      break;
    case '4':
      move->rank = 4;
      break;
    case '5':
      move->rank = 5;
      break;
    case '6':
      move->rank = 6;
      break;
    case '7':
      move->rank = 7;
      break;
    case '8':
      move->rank = 8;
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
}

enum noteState getFourthPart(struct move *move, char c) {
  if (isupper(c) != 0) {
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
    return PIECESTATE;
  } else {
    switch (c) {
    case '1':
      move->rank = 1;
      break;
    case '2':
      move->rank = 2;
      break;
    case '3':
      move->rank = 3;
      break;
    case '4':
      move->rank = 4;
      break;
    case '5':
      move->rank = 5;
      break;
    case '6':
      move->rank = 6;
      break;
    case '7':
      move->rank = 7;
      break;
    case '8':
      move->rank = 8;
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
struct linkedList *getList(FILE *file1, FILE *file2) {
  char buff[512];
  enum noteState state = IDLESTATE;
  struct move move = (struct move){
      .player = WHITE, .piece = EMPTY, .rank = 0, .file = FILELESS};
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
        break;
      case PAWNSTATE:
        if (regexec(&numberMatcher, curStr, 0, NULL, 0) == 0) {
          move.rank = buff[i] - '1';
          addList(&move, list);
          state = IDLESTATE;
          move.player = move.player == WHITE ? BLACK : WHITE;
          move = (struct move){.player = move.player,
                               .piece = EMPTY,
                               .rank = 0,
                               .file = FILELESS};
        }
        break;
      case PIECESTATE:
        if (regexec(&secondCharacter, curStr, 0, NULL, 0) == 0) {
          state = getSecondPart(&move, buff[i]);
        } else if (move.file != FILELESS &&
                   regexec(&thirdCharacter, curStr, 0, NULL, 0) == 0) {
          state = getThirdPart(&move, buff[i]);
          addList(&move, list);
          state = IDLESTATE;
          move.player = move.player == WHITE ? BLACK : WHITE;
          move = (struct move){.player = move.player,
                               .piece = EMPTY,
                               .rank = 0,
                               .file = FILELESS};

        } else if (checkState && move.rank != 0 && move.file != FILELESS) {
          // TODO: check
          addList(&move, list);
          state = IDLESTATE;
          move.player = move.player == WHITE ? BLACK : WHITE;
          move = (struct move){.player = move.player,
                               .piece = EMPTY,
                               .rank = 0,
                               .file = FILELESS};
        }
        // TODO: Takes, disambiguation
        break;
      case PROMOTIONSTATE:
        if (regexec(&promotion, curStr, 0, NULL, 0) == 0) {
          move.piece = buff[i];
        }
        break;

      default:
        fprintf(stderr, "UNKNOWN STATE: %d\n", state);
      case ERRORSTATE:
        state = IDLESTATE;
        break;
      }
    }
  }
  return list;
}
