#include "board.h"
#include "list.h"
#include "moves.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum moveType {
    Pawn,
    Normal,
    Castling
};

enum castlingMove {
    Short,
    Long,
    Vertical,
};

#define RULE_GETCHAR() bool isEof = false; c = pullChar(f, &isEof);\
    if(isEof) { return DEFAULT_PIECE; }

static char pullChar(FILE* file, bool *end) {
    char result = fgetc(file);
    *end = (result == EOF);
    return (char)result;
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

static bool isPeice(char c) {
    switch(c) {
    case 'Q':
    case 'N':
    case 'B':
    case 'R':
    case 'K':
        return true;
    default:
        break;
    }
    return false;
}

static bool isNormalPiece(char c) {
    switch(c) {
    case 'Q':
    case 'N':
    case 'B':
    case 'R':
        return true;
    default:
        break;
    }
    return false;
}

static bool isRank(char c) {
    return (c <= '8' && c >= '1');
}

static bool isFile(char c) {
    return (c <= 'h' && c >= 'a');
}

static struct move startSeesTake(FILE *f, struct move move) {
    
}

static struct move startPieceSeesFile(FILE *f, struct move move) {
    
}

static struct move startPieceMove(FILE *f, struct move move) {
    char c;
    RULE_CHECK();
    if(isFile(c)) {
        return ;
    }
    if(c == 'x') {
        move.flags |= TAKES;
    }
}

static struct move startMove(FILE *f) {
    char c;
    struct move m = DEFAULT_MOVE;
    while(true) {
        RULE_CHECK();
        if(isNormalPiece(c)) {
            m.piece = c;
            m.flags = CHESS_NORMAL;
            return startPieceMove(f, m);
        }
        else if(isFile(c)) {
            m.file = c - '0';
            m.flags = CHESS_NORMAL;
            return startPawnMove(f, m);
        }
        else if(c == 'O') {
            m.flags = CASTLE;
            return castleMove(f, m);
        }
    }
    return m;
}

// resetting moves for the time the program interprets moves
// in a list

struct move pullMove(FILE *file) {
    if(!file) {
        fprintf(stderr, "pullmove: file is NULL.\n");
        return DEFAULT_PIECE;
    }
    return startMove();
}
