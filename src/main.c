#include "../include/board.h"
#include "../include/window.h"
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int processCLI(int argc, char **argv, char **filename) {
    if (argc != 2) {
      printf("usage:");
      exit(-1);
    }
    return 0;
}
int main(int arg, char *argv[]) {
    // WINDOW *window = init_window();
    // getmaxyx(window, max_x, max_y);
    // printf("\n%d %d\n", max_x, max_y);
    // sleep(1);

    // Initialize time
    srand(time(NULL));

    generate_random_sprite();

    printf("initializing gameboard\n");
    board *b = init_board();
    printf("finished initializing gameboard\n\n\n\n");
    print_board(b, false);

    /* Game loop */
    while (!b->game_over) {
     /* For now, generate a random block */
        /* TODO: handle input, and determine where p1 should be */
        scroll_next_board(b);
        update_board(b);

        if (b->game_over) {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("\t\t You lost!\n");
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Final game state before collision\n\n");
            print_board(b, false);
            printf("\n\n");
            printf("Final game state after collision\n");
            print_board(b, true);
        } else {
            print_board(b, false);
            sleep(1);
        }
    }

    // printf("Successful build!\n");
    return 0;
}
