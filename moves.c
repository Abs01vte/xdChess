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

enum noteState {
  PAWNSTATE,
  PIECESTATE,
  CASTLESTATE,
  TAKESTATE,
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

  value = regcomp(&numberMatcher, "[[:digit:]]", REG_EXTENDED);
  if (value != 0) {
    fprintf(stderr, "Compilation error in number matcher regex\n");
    return false;
  }
  return true;
}
void quitMoves(void) {
  regfree(&firstCharacter);
  regfree(&numberMatcher);
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
  } else if (isupper(c) != 0) {
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

struct linkedList *getList(FILE *file1, FILE *file2) {
  char buff[512];
  enum noteState state = IDLESTATE;
  struct move move =
      (struct move){.player = WHITE, .piece = EMPTY, .rank = 0, .file = A};
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
