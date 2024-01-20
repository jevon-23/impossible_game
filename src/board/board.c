#include "../../include/board.h"
#include "../../include/block.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

block **init_next_board(block **board);

void breakpoint() {

}

void place_block(board *gb, bool next_board,  block b, int row, int col) {
    block **board;
    if (next_board)
        board = gb->next_board;
    else
        board = gb->board;
    *(*(board + row) + col) = b;
}

block get_block(block **board, int row, int col) {
    return *(*(board + row) + col);
}

/* Blocks are written in a vertical format, so entire columns are written */
void board_write_sprite(board *gb, bool init, sprite sprite, int col_index) {
    block **blocks = sprite.blocks;
    int start_col = col_index;

    for (int block_ptr = 0; col_index < start_col + sprite.block_dim.width;
         col_index++, block_ptr++) {
      for (int row = 0; row < NUM_ROWS; row++) {

        block src_block = *(*(blocks + row) + block_ptr);
        if (init)
            place_block(gb, false, src_block, row, col_index);
        else
            place_block(gb, true, src_block, row, col_index);
      }
    }

    return;
}

void print_board(board *b, bool next_board) {
    block **game_board;
    if (next_board)
        game_board = b->next_board;
    else
        game_board = b->board;

    for (int i = 0; i < NUM_ROWS; i++) {
      for (int j = 0; j < NUM_COLS; j++) {
        printf("%c", (*(*(game_board + i) + j)).type);
      }
      printf("\n");
    }
}

/* Returns back the row that P1 is in. */
coords find_p1(board *b, bool next_board, bool b4_scroll) {
    coords out;
    block **board;
    if (b4_scroll)
        out.col = 0;
    else
        out.col = 1;

    if (next_board)
        board = b->next_board;
    else
        board = b->board;

    for (int i = 0; i < NUM_ROWS; i++) {
        if (get_block(board, i, out.col).type == P1) {
            out.row = i;
            break;
        }
    }

    return out;
}


bool check_collision(board *b, bool next_board, bool b4_scroll) {
    if (!b->p1_state.is_jumping) {
        /* We are slidinig on the ground, handle collisions that are directly in front of us */
        coords p1_coords = find_p1(b, next_board, b4_scroll);

        /* Check the block directly in front of us to see if we are going to collide w/ a  destructive block*/
        if (get_block(b->next_board, p1_coords.row, p1_coords.col+1).destructive) {
            return true;
        }
    }
    return false;
}



void scroll_next_board(board *b) {
    printf("scroll next gameboard \n");

    block **next_board = b->next_board;
    /* Shift everything over one frame, last col should be untouched */
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS-1; j++) {
            place_block(b, true, get_block(next_board, i, j+1), i, j);
        }
    }

    b->game_over = check_collision(b, true, true);

    /* Place P1 back in pos 0, creating the scroll effect */
    place_block(b, true, get_block(next_board, P1_POS_ROW, P1_POS_COL-1), P1_POS_ROW, P1_POS_COL);
    place_block(b, true, new_block(FLOOR), P1_POS_ROW, P1_POS_COL-1);

    /* Input a random block into the gameboard */
    sprite next_sprite = generate_random_sprite();
    board_write_sprite(b, false, next_sprite, NUM_COLS-1);

    printf("finished scrolling next gameboard\n");
}

void update_board(board *b) {
    if (b->game_over)
        return;

    block **tmp = b->board;

    b->board = b->next_board;
    b->next_board = init_next_board(b->board);

    for (int i = 0; i < NUM_ROWS; i++) {
        free(*(tmp + i));
    }
    free(tmp);
}

block **init_next_board(block **board) {
    /* Copy row ptrs into next board */
    block **next = (block **)malloc(sizeof(block *) * NUM_ROWS);
    memcpy(next, board, sizeof(block *) * NUM_ROWS);

    for (int i = 0; i < NUM_ROWS; i++) {
        /* Copy all column pointers except for the very first */
        *(next + i) = (block *)malloc(sizeof(block) * NUM_COLS);
        memcpy(*(next + i), *(board + i), sizeof(block) * NUM_COLS);
    }

    return next;
}

board *init_board() {
    printf("initializing memory for gameboard\n");

    board *new_board = (board *)malloc(sizeof(board));
    new_board->game_over = false;

    new_board->board = (block **)malloc(sizeof(block *) * NUM_ROWS);
    block **board = new_board->board;
    for (int i = 0; i < NUM_ROWS; i++) {
      *(board + i) = (block *)malloc(sizeof(block) * NUM_COLS);
    }

    printf("memory initialized for gameboard\n");

    printf("writing out initial sprites to gameboard\n");
    sprite sprite_block;
    for (int j = 0; j < NUM_COLS; j++) {
      switch (j) {
          case 1:
              /* Write player 1 to the board */
              sprite_block = generate_sprite(P1);
              break;
          // Test to see the other blocks 
          // case 5:
          //     sprite_block = generate_sprite(BLOCK);
          //     break;
          // case 10:
          //     sprite_block = generate_sprite(HOLE);
          //     break;
          // case 15:
          //     sprite_block = generate_sprite(SPIKE);
          //     break;
          default:
              sprite_block = generate_sprite(FLOOR);
              break;
      }
      board_write_sprite(new_board, true, sprite_block, j);
    }

    printf("finished wirting out initial sprites to gameboard\n");

    printf("copying initial gameboard to next gameboard\n");
    new_board->next_board = init_next_board(board);
    printf("finished copying initial gameboard to next game board\n");

    return new_board;
}
