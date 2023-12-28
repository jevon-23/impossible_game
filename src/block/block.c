#include "../../include/block.h"
#include "../../include/board.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_sprite(sprite s) {
  printf("sprite type: %c\n", s.type);

  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < s.block_dim.width; j++) {
      printf("%c", (*(*(s.blocks + i) + j)).type);
    }
    printf("\n");
  }
}

sprite new_sprite(char type, block **blocks, dimensions dim) {
  sprite s;
  s.type = type;
  s.blocks = blocks;
  s.block_dim = dim;
  return s;
}

block **generate_space(block space) {
  /* Do we need this? I don't think so */
  // block **space_tiles = (block **)malloc(sizeof(block *) * NUM_ROWS);

  // return space_tiles;
  printf(
      "Function should not be called: Generate space block not implemented!\n");
  exit(-1);
}

sprite generate_floor(block floor) {
  block **floor_tiles = (block **)malloc(sizeof(block *) * NUM_ROWS);
  int num_spaces_above = NUM_ROWS - floor.height;
  int row_ptr;

  /* Draw spaces above the floor */
  for (row_ptr = 0; row_ptr < num_spaces_above; row_ptr++) {
    *(floor_tiles + row_ptr) = (block *)malloc(sizeof(block) * FLOOR_WIDTH);
    for (int col = 0; col < floor.width; col++) {
      *(*(floor_tiles + row_ptr) + col) = new_block(SPACE);
    }
  }

  /* Draw floor tile */
  for (; row_ptr < NUM_ROWS; row_ptr++) {
    *(floor_tiles + row_ptr) = (block *)malloc(sizeof(block) * FLOOR_WIDTH);
    for (int col = 0; col < floor.width; col++) {
      *(*(floor_tiles + row_ptr) + col) = new_block(FLOOR);
    }
  }

  dimensions dim = new_dimensions(floor.width, floor.height);
  return new_sprite(floor.type, floor_tiles, dim);
}

dimensions new_dimensions(int width, int height) {
  dimensions dim;
  dim.width = width;
  dim.height = height;
  return dim;
}

/* Block implementation */
block new_block(char type) {
  block b;
  b.type = type;

  /* update destructive types */
  switch (type) {
  case HOLE:
  case SPIKE:
  case BLOCK:
    b.destructive = true;
    break;
  case FLOOR:
  case P1:
  case SPACE:
    b.destructive = false;
    break;

  default:
    printf("invalid floor was passed in, type: %c \n", type);
    exit(-1);
  }

  /* Update dimensions, and set fn pointer */
  switch (type) {
  case HOLE:
    break;
  case SPIKE:
    break;
  case BLOCK:
    break;
  case FLOOR:
    b.height = FLOOR_HEIGHT;
    b.width = FLOOR_WIDTH;
    b.generate_block = &generate_floor;
    break;
  case P1:
    break;
  case SPACE:
    b.height = SPACE_HEIGHT;
    b.width = SPACE_WIDTH;
    break;
  default:
    printf("invalid floor was passed in, type: %c \n", type);
    exit(-1);
  }

  return b;
}
