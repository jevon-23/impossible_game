#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>

/* 32 bit flag */
enum KEY_FLAGS {
    QUIT_KEY_FLAG,
    SPACE_BAR_KEY_FLAG,
    PAUSE_KEY_FLAG,
    NOTHING_KEY_FLAG,
};

typedef struct int_args {
    /* The thread id of the interrupt */
    pthread_t thread_id;

    /* What key was pressed */
    enum KEY_FLAGS *key_press_flag;
    pthread_mutex_t *key_press_flag_lock;

    bool *game_over;
    pthread_mutex_t *game_over_lock;
}int_args;

int_args *init_keyboard_listener();
void close_listener(int_args *arguments);

void write_game_over_flag(int_args *arguments, bool game_over);
bool read_game_over_flag(int_args *arguments);
enum KEY_FLAGS read_key_press_flag(int_args *arguments);
void write_key_press_flag(int_args *arguments, enum KEY_FLAGS key_press);

#endif
