#include <stdlib.h>
#include "board.h"
#include <string.h>
#include <stdio.h>
#include "list.h"
#include <regex.h>
#include <stdbool.h>
#include "moves.h"
#include <ctype.h>

regex_t firstCharacter;
regex_t numberMatcher;

enum noteState {
  PAWNSTATE, PIECESTATE, CASTLESTATE, TAKESTATE, ERRORSTATE, IDLESTATE
};

bool initMoves(void){
  int value;

  // Function call to create regex
  value = regcomp( &firstCharacter, "Q|K|N|B|R|a|b|c|d|e|f|g|h|O", 0);



  // Else for Compilation error
  if(value != 0) {
      printf("Compilation error in first character regex\n");
      return false;
  }

  value = regcomp(&numberMatcher, "[[:digit:]]", 0);
  if(value != 0) {
      printf("Compilation error in number matcher regex\n");
      return false;
  }
  return true;
}
void quitMoves(void){
  regfree(&firstCharacter);
  regfree(&numberMatcher);
}
enum noteState getFirstPart(struct move* move, char c){
  if(islower(c) != 0){
    move->piece = PAWN;
    switch(c){
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
        printf("Error finding pawn move %c\n", c);
        return ERRORSTATE;
      break;
    }
    return PAWNSTATE;
  }
  else if(c == 'O'){
    move->rank = -1;
    return CASTLESTATE;
  }
  else{
    switch(c){
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
        printf("Error could not find piece %c\n", c);
        return ERRORSTATE;
      break;
    }
    return PIECESTATE;
  }

  return ERRORSTATE;
}

struct linkedList* getList(FILE* file1, FILE* file2){
  char buff[512];
  enum noteState state = IDLESTATE;
  struct move move = (struct move){.player = WHITE, .piece = EMPTY, .rank = 0, .file = 'A'};
  struct linkedList* list = makeList(sizeof(struct move));
  if(list == NULL){
    return NULL;
  }
  for(char* retVal = fgets(buff, sizeof(buff), file1); retVal != NULL; retVal = fgets(buff, sizeof(buff), file1)){
    size_t buffLen = sizeof(buff);
    for(size_t i = 0; i < buffLen; i++){
      if(state == IDLESTATE){
        if(regexec(&firstCharacter, buff+i, 0, NULL, 0) == 0){
          state = getFirstPart(&move, buff[i]);
        }
      }
      else if (state == PAWNSTATE){
        if(regexec(&numberMatcher, buff+i, 0, NULL, 0) == 0){
          move.rank = buff[i]-48;
          addList(&move, list);
          state = IDLESTATE;
        }
      }
      if(state == ERRORSTATE){
        state = IDLESTATE;
        continue;
      }
    }
  }
  return list;
}
