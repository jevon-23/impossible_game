#include "../include/board.h"
#include "../include/window.h"
#include "../include/utils.h"
#include "../include/interrupts.h"
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>


int processCLI(int argc, char **argv, char **filename) {
    if (argc != 2) {
      printf("usage:");
      exit(-1);
    }
    return 0;
}
int main(int arg, char *argv[]) {
    printf("initializing gameboard\n");
    board *b = init_board();

    printf("finished initializing gameboard\n\n\n\n");
    /* Initialize ncurses window */
    WINDOW *window = init_window();

    int_args *keyboard_interrupt_vector = init_keyboard_listener();


    // Initialize random seed
    srand(time(NULL));

    /* Game loop */
    while (!read_game_over_flag(keyboard_interrupt_vector)) {

        /* For now, generate a random block */
        scroll_next_board(b);
        update_board(b);
        write_game_over_flag(keyboard_interrupt_vector, b->game_over);
        update_frame(window, b);
        sleep(1);
    } 

    /* TODO: Update to use ncurses window instead */
    print_gameover(b);
    close_listener(keyboard_interrupt_vector);

    endwin();
    return 0;
}
