#include "../../include/board.h"
#include "../../include/block.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board(board *b) {
  block **game_board = b->board;

  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      printf("%c", (*(*(game_board + i) + j)).type);
    }
    printf("\n");
  }
}

void place_block(board *gb, block b, int row, int col) {
  block **board = gb->board;
  *(*(board + row) + col) = b;
}

void board_write_block(board *gb, sprite sprite, int col_index) {
  block **blocks = sprite.blocks;
  int start_col = col_index;

  for (int block_ptr = 0; col_index < start_col + sprite.block_dim.width;
       col_index++, block_ptr++) {
    for (int row = 0; row < NUM_ROWS; row++) {

      block src_block = *(*(blocks + row) + block_ptr);
      place_block(gb, src_block, row, col_index);
    }
  }

  return;
}

board *init_board() {
  board *new_board = (board *)malloc(sizeof(board));
  new_board->board = (block **)malloc(sizeof(block *) * NUM_ROWS);

  block **board = new_board->board;

  for (int i = 0; i < NUM_ROWS; i++) {
    *(board + i) = (block *)malloc(sizeof(block) * NUM_COLS);
  }

  for (int j = 0; j < NUM_COLS; j++) {
    block floor = new_block(FLOOR);
    sprite floor_sprite = floor.generate_block(floor);
    board_write_block(new_board, floor_sprite, j);
  }

  return new_board;
}
