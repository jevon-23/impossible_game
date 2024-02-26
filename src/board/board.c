#include "../../include/board.h"
#include "../../include/block.h"
#include "../../include/interrupts.h"
#include "../../include/utils.h"
#include <float.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

block **init_next_board(block **board);

void place_block(board *gb, bool next_board, block b, int row, int col) {
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

char *str_board(board *b, bool next_board) {
    int block_index = 0;
    char *board_str = (char *)malloc(sizeof(char) * BOARD_STR_LEN);

    block **game_board;
    if (next_board)
        game_board = b->next_board;
    else
        game_board = b->board;

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            board_str[block_index++] = get_block(game_board, i, j).type;
        }
        board_str[block_index++] = '\n';
    }

    board_str[block_index] = '\0';
    return board_str;
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

    out.row = P1_POS_ROW;
    for (int i = 0; i < NUM_ROWS; i++) {
        if (get_block(board, i, out.col).type == P1) {
            out.row = i;
            break;
        }
    }
    return out;
}

bool check_jump_collision(board *b, bool next_board, bool b4_scroll) {
    block **game_board;
    if (next_board)
        game_board = b->next_board;
    else
        game_board = b->board;

    /********************************************************************************
     *   Note: This function is called after we update P1_POS_COL & P1_POS_ROW,
     *         but before we actually update the position of the player sprite
     *         on the board. So there should be a block other than the P1 block
     *         in the position of (P1_POS_COL, P1_POS_ROW).
     *
     *   Collision rules are based on jump state
     *   CASE NO JUMP:
     *      We shouldn't be in here, hit panic
     *   CASE UPn:
     *      If there is anything that is in the path of the jump, we are colliding
     *   CASE DOWNn:
     *      If there is anything below us that we cannot land on, we are colliding
     *      If there is anything below us that is destructive, we are colliding
     *   CASE LANDED:
     *      We should hit panic. As of right now, as soon as we hit the landed
     *      state, we automatically update to either UP1 or NO_JUMP
     ********************************************************************************/

    block next_block = get_block(game_board, P1_POS_ROW, P1_POS_COL);
    block replace_block = get_block(game_board, P1_POS_ROW, P1_POS_COL-1);
    switch (b->p1_state.jump_state) {
    case UP1:
    case UP2:
        if (next_block.destructive) {
            return true;
        }
        break;
    case DOWN1:
    case DOWN2:
        if (replace_block.can_land && replace_block.type != FLOOR) {
          P1_POS_ROW--;
          break;
        }
        if (next_block.destructive || (!next_block.can_land && next_block.type != SPACE)) {
          return true;
        }
        break;

    case LANDED:
    default:
        printf("This jump state is not being handled for jump state: %d\n",
               b->p1_state.jump_state);
        addstr("hit da default\n");
    }
    return false;
}

bool check_collision(board *b, bool next_board, bool b4_scroll) {
    block **game_board;
    if (next_board)
        game_board = b->next_board;
    else
        game_board = b->board;

    if (!b->p1_state.is_jumping) {
        /* We are slidinig on the ground, handle collisions that are directly in
         * front of us */
        coords p1_coords = find_p1(b, next_board, b4_scroll);

        /* Check the block directly in front of us to see if we are going to collide
         * w/ a  destructive block*/
        if (get_block(game_board, p1_coords.row, p1_coords.col + 1).destructive) {
            return true;
        }
        return false;
    }
    return check_jump_collision(b, next_board, b4_scroll);
}

void update_p1_pos_row_while_jumping(board *b) {
    if (!b->p1_state.is_jumping)
        return;

    switch (b->p1_state.jump_state) {
        case UP1:
        case UP2:
            P1_POS_ROW -= 1;
            break;
        case DOWN1:
        case DOWN2:
            P1_POS_ROW += 1;
            break;
        case NO_JUMP:
        case LANDED:
            break;
        default:
            printf("Jumping state has not been handled yet\n");
    }
}

void update_p1_pos_row_for_falling(board *b, int_args *key_board_listener_args) {
    if (P1_POS_ROW == P1_DEFAULT_POS_ROW) {
        /* Start jump */
        enum KEY_FLAGS key_press = read_key_press_flag(key_board_listener_args);
        if (key_press == JUMP_KEY) {
            b->p1_state.is_jumping = true;
            b->p1_state.jump_state = UP1;
            update_p1_pos_row_while_jumping(b);
            write_key_press_flag(key_board_listener_args, NOTHING_KEY_FLAG);
            return;
        }
        return;
    }
    block prev_block =
        get_block(b->next_board, P1_POS_ROW,  P1_POS_COL-1);
    block block_underneath_p1 =
        get_block(b->next_board, P1_POS_ROW + 1, P1_POS_COL);

    if ((block_underneath_p1.type == SPACE || block_underneath_p1.type == FLOOR)
            ) {

        P1_POS_ROW++;
    }
}

void scroll_next_board(board *b, int_args *key_board_listener_args) {
    printf("scroll next gameboard \n");

    block **next_board = b->next_board;

    /* Place P1 back in pos 0, creating the scroll effect */
    block replace_block = get_block(b->board, P1_POS_ROW, P1_POS_COL+1);
    /* Shift everything over one frame, last col should be untouched */
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS - 1; j++) {
            place_block(b, true, get_block(next_board, i, j + 1), i, j);
        }
    }

    enum KEY_FLAGS key_press = read_key_press_flag(key_board_listener_args);
    if (!b->p1_state.is_jumping) {
        b->p1_state.jump_state = NO_JUMP;

        /* Start jump */
        if (key_press == JUMP_KEY) {
            b->p1_state.is_jumping = true;
            b->p1_state.jump_state = UP1;
            write_key_press_flag(key_board_listener_args, NOTHING_KEY_FLAG);
        }

    } else {
        b->p1_state.jump_state += 1;

        if (b->p1_state.jump_state == LANDED) {

            if (key_press == JUMP_KEY) {
                b->p1_state.is_jumping = true;
                b->p1_state.jump_state = UP1;
            } else {
                b->p1_state.is_jumping = false;
                b->p1_state.jump_state = NO_JUMP;
            }
        }

        /* Reset the key to be nothing if we're already jumping, makes jump more
         * timing based */
        write_key_press_flag(key_board_listener_args, NOTHING_KEY_FLAG);
    }

    coords p1_coords = find_p1(b, b->next_board, false);
    update_p1_pos_row_while_jumping(b);

    b->game_over = check_collision(b, true, true);
    if (b->p1_state.jump_state == NO_JUMP || b->p1_state.jump_state == DOWN1 || b->p1_state.jump_state == DOWN2 || b->p1_state.jump_state == LANDED)
        update_p1_pos_row_for_falling(b, key_board_listener_args);

    if (!b->p1_state.is_jumping) {
        place_block(b, true,
                    get_block(next_board, p1_coords.row, p1_coords.col - 1),
                    P1_POS_ROW, P1_POS_COL);
        place_block(b, true, replace_block, P1_POS_ROW, P1_POS_COL - 1);
    } else {
        place_block(b, true,
                    get_block(next_board, p1_coords.row, p1_coords.col - 1),
                    P1_POS_ROW, P1_POS_COL);
        place_block(b, true, replace_block, p1_coords.row, p1_coords.col - 1);
    }

    /* Input a random block into the gameboard */
    // sprite next_sprite = generate_random_sprite();

    if (counter % 5 == 0)
        flip = ~flip;

    sprite next_sprite;
    if (counter % 5 == 0)
        next_sprite = generate_sprite(BLOCK);
        // next_sprite = generate_sprite(SPIKE);
    else
        next_sprite = generate_sprite(FLOOR);
    counter++;
    board_write_sprite(b, false, next_sprite, NUM_COLS - 1);

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
