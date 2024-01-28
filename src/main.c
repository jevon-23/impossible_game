#include "../include/board.h"
#include "../include/window.h"
#include "../include/utils.h"
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
    /* Initialize ncurses window */
    WINDOW *window = init_window();

    // Initialize random seed
    srand(time(NULL));

    printf("initializing gameboard\n");
    board *b = init_board();
    printf("finished initializing gameboard\n\n\n\n");

    /* Game loop */
    while (!b->game_over) {
        update_frame(window, b);
        sleep(1);

        /* For now, generate a random block */
        scroll_next_board(b);
        update_board(b);
    } 

    /* TODO: Update to use ncurses window instead */
    print_gameover(b);

    endwin();
    return 0;
}
