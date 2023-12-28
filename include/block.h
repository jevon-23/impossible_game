#ifndef BLOCK_H
#define BLOCK_H
#include <stdbool.h>

typedef struct dimensions {
    int width;
    int height;
}dimensions;

typedef struct block {
    char type;
    bool destructive;
    struct sprite (*generate_block)(struct block);
    /* TODO: Substitute with dimensions */
    int width;
    int height;
} block;

typedef struct sprite {
    char type;
    block **blocks;
    dimensions block_dim;
}sprite;

// Identifiers
#define FLOOR '_' // Floor
#define FLOOR_HEIGHT 1
#define FLOOR_WIDTH  1




#define P1    'P' // Player
#define HOLE  '0' // Empty floor
#define SPIKE '^' // Spike
#define BLOCK '#' // BLOCK

#define SPACE '*' // SPACE
#define SPACE_HEIGHT 1
#define SPACE_WIDTH  1



block new_block(char type);

dimensions new_dimensions(int width, int height);

void print_sprite(sprite s);

#endif
