#include "../../include/utils.h"
#include <stdio.h>

void breakpoint() {}

void print_gameover(board *b) {

    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("\t\t You lost!\n");
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("Final game state before collision\n\n");
    print_board(b, false);
    printf("\n\n");
    printf("Final game state after collision\n");
    print_board(b, true);
}
