#include <curses.h>
#include "board.h"
#ifndef WINDOW_H
#define WINDOW_H


// char **init_board();

WINDOW *init_window();
void update_frame(WINDOW *window, board *board);

#endif
