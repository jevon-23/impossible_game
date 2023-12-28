#include "../../include/window.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WINDOW *init_window() {

  WINDOW *w = initscr();
  timeout(-1);
  char *txt = (char *)malloc(sizeof(char) * 160);
  char txt2[160] =
      "fdsjaklfjadkslfjskalfjskjfakljdkljasfklajdfksdkjfdsioejkfjodkoifjoejkofe"
      "iokjdsifjoehgijfoejkofjighoekajsaiogdojkgioehaagkdphiwphvkjagp";

  strcpy(txt, txt2);
  // addch(ACS_BLOCK | A_UNDERLINE);
  // addchstr(txt);
  // refresh();
  // for (int i = 0; i < 157; i++) {
  //   addch(' ' | A_UNDERLINE);
  //   refresh();
  // }
  // noecho();
  // start_color();
  // init_pair(RED_ID, COLOR_RED, COLOR_BLACK);
  // init_pair(GREEN_ID, COLOR_GREEN, COLOR_BLACK);
  // init_pair(WHITE_ID, COLOR_WHITE, COLOR_BLACK);
  return w;
}
