#ifndef EVALUATION_H
#define EVALUATION_H
#include "board.h"
#include "moves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct boardState {
  // Numerical value of the balance of power between white and black
  // Positive integers are advantageous for white
  // Negative integers are advantageous for black
  int balance;
  // Whether or not checkmate is possible in 3 moves or less
  bool mating;
  // Collection of continuation moves based on the calculation
  struct linkedList bestMoves;
};




#endif
