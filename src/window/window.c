#include "../../include/window.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WINDOW *init_window() {
  printf("initializing ncurses window\n");
  WINDOW *w = initscr();
  timeout(-1);
  noecho();
  printf("finished initializing ncurses window\n");
  return w;
}

void update_frame(WINDOW *window, board *board) {
    printf("updating the next frame\n");
    char *board_str = str_board(board, false);
    clear();
    addstr(board_str);
    refresh();
    timeout(3);
    return;
}
