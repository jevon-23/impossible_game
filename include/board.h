#include "block.h"
#ifndef BOARD_H
#define BOARD_H

// Dimensions
#define NUM_ROWS 4
#define NUM_COLS 10

#define P1_POS_ROW NUM_ROWS-1
#define P1_POS_COL 1

typedef struct player_state {
    bool is_jumping;
} player_state;

typedef struct board {
    /* Board that is currently being displayed */
    block **board;

    /* Board to be displayed on the next frame */
    block **next_board;

    player_state p1_state;

    bool is_paused;
    bool game_over;
}board;

typedef struct coords {
    int row;
    int col;
}coords;

board *init_board();
void update_board(board *b);
void print_board(board *b, bool next_board);
void scroll_next_board(board *b);

#endif
