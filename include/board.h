#include "block.h"
#ifndef BOARD_H
#define BOARD_H

// Dimensions
#define NUM_ROWS 4
#define NUM_COLS 100

typedef struct board {
    block **board;
}board;


board *init_board();
void print_board(board *b);

#endif
