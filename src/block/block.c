#include "../../include/block.h"
#include "../../include/board.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************/
/* Header functions */
/********************/
char block_type_to_char(enum block_types bt);

/**************************/
/* Sprite implementation */
/**************************/
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

void draw_spaces_above_block(block **block_tiles, int *row_ptr, int block_height, int block_width) {
    int num_spaces_above = NUM_ROWS - block_height;

    for (; *row_ptr < num_spaces_above; (*row_ptr)++) {
        *(block_tiles + *row_ptr) = (block *)malloc(sizeof(block) * SPACE_WIDTH);

        for (int col = 0; col < block_width; col++) {
            *(*(block_tiles + *row_ptr) + col) = new_block(SPACE);
        }
    }
}

sprite generate_sprite(char block_type) {
        
    block **block_tiles = (block **)malloc(sizeof(block *) * NUM_ROWS);
    int row_ptr = 0;

    dimensions dim = get_dimensions(block_type);

    /* Draw spaces above the block */
    draw_spaces_above_block(block_tiles, &row_ptr, dim.height, dim.width);

    /* Draw block tiles */
    for (; row_ptr < NUM_ROWS; row_ptr++) {
        *(block_tiles + row_ptr) = (block *)malloc(sizeof(block) * dim.width);
        for (int col = 0; col < dim.width; col++) {
            *(*(block_tiles + row_ptr) + col) = new_block(block_type);
        }
    }

    return new_sprite(block_type, block_tiles, dim);
}

sprite generate_random_sprite(void) {
    int r;
    do {
        r = rand() % 6;
    } while (r == _P1 || r == _SPACE);
    return generate_sprite(block_type_to_char(r));
}

/*****************************/
/* Dimensions implementation */
/*****************************/
dimensions get_dimensions(char type) {
    dimensions dim;

    switch(type) {
        case SPACE:
            dim.width = SPACE_WIDTH;
            dim.height = SPACE_HEIGHT;
            break;
        case FLOOR:
            dim.width = FLOOR_WIDTH;
            dim.height = FLOOR_HEIGHT;
            break;
        case P1:
            dim.width = P1_WIDTH;
            dim.height = P1_HEIGHT;
            break;
        case HOLE:
            dim.width = HOLE_WIDTH;
            dim.height = HOLE_HEIGHT;
            break;
        case SPIKE:
            dim.width = SPIKE_WIDTH;
            dim.height = SPIKE_HEIGHT;
            break;
        case BLOCK:
            dim.width = BLOCK_WIDTH;
            dim.height = BLOCK_HEIGHT;
            break;
        default:
            printf("Invalid block type passed in for dimensions\n");
            exit(-1);
    }
    return dim;
}

/************************/
/* Block implementation */
/************************/
char block_type_to_char(enum block_types bt) {
    switch(bt) {
        case _SPACE:
            return SPACE;
        case _FLOOR:
            return FLOOR;
        case _P1:
            return P1;
        case _HOLE:
            return HOLE;
        case _SPIKE:
            return SPIKE;
        case _BLOCK:
            return BLOCK;
        default:
            printf("Invalid block type passed in: %d\n", bt);
            exit(-1);
    }
}

block new_block(char type) {
    block b;
    b.type = type;
    b.dim = get_dimensions(type);

    switch (type) {
        case HOLE:
            b.destructive = true;
            b.can_land = false;
            break;
        case SPIKE:
            b.destructive = true;
            b.can_land = false;
            break;
        case SPACE:
            b.destructive = false;
            b.can_land = false;
            break;
        case FLOOR:
            b.destructive = false;
            b.can_land = true;
            break;
        case P1:
            b.destructive = false;
            b.can_land = true;
            break;
        case BLOCK:
            b.destructive = true;
            b.can_land = true;
            break;
        default:
            printf("invalid floor was passed in, type: %c \n", type);
            exit(-1);
    }

    return b;
}
