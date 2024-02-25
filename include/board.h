#include "block.h"
#include "interrupts.h"
#ifndef BOARD_H
#define BOARD_H

// Dimensions
#define NUM_ROWS 4
#define NUM_COLS 10

// Board str_len = (# of \n) + ((# of rows) * (# of cols) + '\0')
#define BOARD_STR_LEN (NUM_ROWS + (NUM_ROWS * NUM_COLS) + 1)

#define P1_POS_COL 1
#define P1_DEFAULT_POS_ROW NUM_ROWS-1
static int P1_POS_ROW = NUM_ROWS-1;

static uint32_t counter = 0;
static uint32_t flip = 0;

/* interrupts.h -> if user his space bar we jump */
#define JUMP_KEY SPACE_BAR_KEY_FLAG

enum jump_state_enum {
    NO_JUMP,
    UP1,
    UP2,
    DOWN1,
    DOWN2,
    LANDED,
};

typedef struct player_state {
    bool is_jumping;
    enum jump_state_enum jump_state; 
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
void scroll_next_board(board *b, int_args *key_board_listener_args);
char *str_board(board *b, bool next_board);
#endif
