#include "../include/board.h"
#include "../include/window.h"
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

  board *b = init_board();
  print_board(b);

  // printf("Successful build!\n");
  return 0;
}
