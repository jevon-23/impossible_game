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

sprite generate_space(block space) {
    /* Do we need this? I don't think so */
    // block **space_tiles = (block **)malloc(sizeof(block *) * NUM_ROWS);

    // return space_tiles;
    printf(
        "Function should not be called: Generate space block not implemented!\n");
    exit(-1);
}

void draw_spaces_above_block(block **block_tiles, int *row_ptr, int block_height, int block_width) {
    int num_spaces_above = NUM_ROWS - block_height;

    for (; *row_ptr < num_spaces_above; (*row_ptr)++) {
        *(block_tiles + *row_ptr) = (block *)malloc(sizeof(block) * SPACE_WIDTH);

        for (int col = 0; col < block_width; col++) {
            *(*(block_tiles + *row_ptr) + col) = new_block(SPACE);
        }
    }
}

sprite generate_floor(block floor) {
    block **floor_tiles = (block **)malloc(sizeof(block *) * NUM_ROWS);
    int row_ptr = 0;

    /* Draw spaces above the floor */
    draw_spaces_above_block(floor_tiles, &row_ptr, floor.height, floor.width);

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

sprite generate_p1(block p1) {
    block **p_tiles = (block **)malloc(sizeof(block *) * NUM_ROWS); 
    int row_ptr = 0;

    /* Draw spaces above the player */
    draw_spaces_above_block(p_tiles, &row_ptr, p1.height, p1.width);

    /* Draw floor tile */
    for (; row_ptr < NUM_ROWS; row_ptr++) {
      *(p_tiles + row_ptr) = (block *)malloc(sizeof(block) * P1_WIDTH);
      for (int col = 0; col < p1.width; col++) {
        *(*(p_tiles + row_ptr) + col) = new_block(P1);
      }
    }

    dimensions dim = new_dimensions(p1.width, p1.height);
    return new_sprite(p1.type, p_tiles, dim);
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

    switch (type) {
        case HOLE:
            b.height = HOLE_HEIGHT;
            b.width = HOLE_WIDTH;
            b.destructive = true;
            b.can_land = false;
            /* TODO: generate block */
            b.generate_block = NULL;
            break;
        case SPIKE:
            b.height = SPIKE_HEIGHT;
            b.width = SPIKE_WIDTH;
            b.destructive = true;
            b.can_land = false;
            /* TODO: generate spike */
            b.generate_block = NULL;
            break;
        case SPACE:
            b.height = SPACE_HEIGHT;
            b.width = SPACE_WIDTH;
            b.destructive = true;
            b.can_land = false;
            b.generate_block = &generate_space;
            break;
        case FLOOR:
            b.height = FLOOR_HEIGHT;
            b.width = FLOOR_WIDTH;
            b.destructive = false;
            b.can_land = true;
            b.generate_block = &generate_floor;
            break;
        case P1:
            b.height = P1_HEIGHT;
            b.width = P1_WIDTH;
            b.destructive = false;
            b.can_land = true;
            b.generate_block = &generate_p1;
            break;
        case BLOCK:
            b.height = BLOCK_HEIGHT;
            b.width = BLOCK_WIDTH;
            b.destructive = true;
            b.can_land = true;
            /* TODO: generate block */
            b.generate_block = NULL;
            break;

        default:
            printf("invalid floor was passed in, type: %c \n", type);
            exit(-1);
    }

    if (b.generate_block == NULL) {
        printf("Generate block has not been implemented\n");
        exit(-1);
    }

    return b;
}
