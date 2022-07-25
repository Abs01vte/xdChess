#include "board.h"
#include "moves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

float materialEvaluation(const struct board *board) {
  float score = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j ++) {
      if (board->tiles[i][j].player == WHITE) {
        switch (board->tiles[i][j].piece) {
          case QUEEN:
            score+=9;
            break;
          case ROOK:
            score+=5;
            break;
          case KNIGHT:
            score+=3;
            break;
          case BISHOP:
            score+=3;
            break;
          case PAWN:
            score+=1;
          default:
            break;
        }
      }
      if (board->tiles[i][j].player == BLACK) {
        switch (board->tiles[i][j].piece) {
          case QUEEN:
            score-=9;
            break;
          case ROOK:
            score-=5;
            break;
          case KNIGHT:
            score-=3;
            break;
          case BISHOP:
            score-=3;
            break;
          case PAWN:
            score-=1;
          default:
            break;
        }
      }
    }
  }
  return score;
}

float positionalEvaluation(const struct board *board, int turn) {
  float whiteScore = 0;
  float blackScore = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if(board->tiles[i][j].player == WHITE) {
        if (i <= 5) {
          switch (board->tiles[i][j].piece) {
            case QUEEN:
              whiteScore += 9 + ((i + turn) / 2);
              break;
            case ROOK:
              whiteScore += 5 + ((i + turn) / 2);
              break;
            case KNIGHT:
              whiteScore += 3 + ((i + turn) / 2);
              break;
            case BISHOP:
              whiteScore += 3 + ((i + turn) / 2);
            case PAWN:
              whiteScore += 1 + (i * 2);
            default:
              break;
          }
        }
      }
      if(board->tiles[i][j].player == BLACK) {
        if (i >= 4) {
          switch (board->tiles[i][j].piece) {
            case QUEEN:
              blackScore += 9 + ((i + turn) / 2);
              break;
            case ROOK:
              blackScore += 5 + ((i + turn) / 2);
              break;
            case KNIGHT:
              blackScore += 3 + ((i + turn) / 2);
              break;
            case BISHOP:
              blackScore += 3 + ((i + turn) / 2);
            case PAWN:
              blackScore += 1 + (i * 2);
            default:
              break;
          }
        }
      }
    }
  }
  return (whiteScore - blackScore);
}
float evaluateBoard(const struct board *board) {
  return (materialEvaluation(board) + positionalEvaluation(board)) / 2;
}
