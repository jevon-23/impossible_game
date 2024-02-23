#include "../../include/interrupts.h"
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


/********************************/
/* Reading and writing to flags */
/********************************/
void write_key_press_flag(int_args *arguments, enum KEY_FLAGS key_press) {
    pthread_mutex_lock(arguments->key_press_flag_lock);
    *(arguments->key_press_flag) = key_press;
    pthread_mutex_unlock(arguments->key_press_flag_lock);
}

enum KEY_FLAGS read_key_press_flag(int_args *arguments) {
    enum KEY_FLAGS out;
    pthread_mutex_lock(arguments->key_press_flag_lock);
    out = *(arguments->key_press_flag);
    pthread_mutex_unlock(arguments->key_press_flag_lock);
    return out;
}

void write_game_over_flag(int_args *arguments, bool game_over) {
    pthread_mutex_lock(arguments->game_over_lock);
    *arguments->game_over = game_over;
    pthread_mutex_unlock(arguments->game_over_lock);
}

bool read_game_over_flag(int_args *arguments) {
    bool out; 
    pthread_mutex_lock(arguments->game_over_lock);
    out = *arguments->game_over;
    pthread_mutex_unlock(arguments->game_over_lock);
    return out;
}

/************************************/
/* Processing / Handling user input */
/************************************/

enum KEY_FLAGS process_user_input(char inp) {
    switch (inp) {
        case 'q': // quit
            return QUIT_KEY_FLAG;
        case ' ': // spaceabar
            printf("return space bar key flag\n");
            return SPACE_BAR_KEY_FLAG;
        case 'p': // pause
            return PAUSE_KEY_FLAG;
        default:
            return NOTHING_KEY_FLAG;
    }
}

void handle_user_input(int_args *arguments, enum KEY_FLAGS key_press) {
    switch (key_press) {
        case QUIT_KEY_FLAG:
            write_game_over_flag(arguments, true);
            break;
        case SPACE_BAR_KEY_FLAG:
            /* Jump (if we aren't jumping already)! */
            printf("We hit the space bar!\n");
            write_key_press_flag(arguments, SPACE_BAR_KEY_FLAG);
            break;
        case PAUSE_KEY_FLAG:
            /* Pause the game */
            break;
        default:
            // Default returns early from function, and doesn't execute write
            return;
    }
    write_key_press_flag(arguments, key_press);
}

/* Kill the thread, free all malloced mem */
void close_listener(int_args *args) {
    printf("begin to close the listener\n");
    /* Kill the thread, don't want to access free'd data */
    pthread_join(args->thread_id, NULL);

    /* Free the flags and their locks */
    free(args->key_press_flag);
    pthread_mutex_destroy(args->key_press_flag_lock);
    free(args->key_press_flag_lock);

    free(args->game_over);
    pthread_mutex_destroy(args->game_over_lock);
    free(args->game_over_lock);

    free(args);


    printf("Gameover, finished closing the listerer \n");
}

// Run the thread's function. Listen for user input
// handle user input by updating the key press &
// game over flags
void *listen(void *interrupt_argumets) {
    printf("Begin keyboard listener\n");
    int_args *arguments = (int_args *) interrupt_argumets;

    char inp;
    enum KEY_FLAGS key_press;
    while (!read_game_over_flag(arguments)) {
        inp = getch();
        key_press = process_user_input(inp);
        handle_user_input(arguments, key_press);
    }
    printf("Keyboard listener is finished listening\n");
    return NULL;
}

// Create a new thread who's purpose is to be a keyboard listener
int_args *init_keyboard_listener(uint32_t *key_press_flag, bool *game_over) {
    printf("initializing keyboard listener and fake interrupt vector \n");
    int_args *out = (int_args *)malloc(sizeof(int_args));

    out->key_press_flag = (enum KEY_FLAGS *)malloc(sizeof(enum KEY_FLAGS));
    *(out->key_press_flag) = NOTHING_KEY_FLAG;
    out->key_press_flag_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (pthread_mutex_init(out->key_press_flag_lock, NULL) != 0) {
        printf("Failed to create mutex for key flag\n");
        exit(-1);
    }

    out->game_over = (bool *)malloc(sizeof(bool));
    out->game_over_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (pthread_mutex_init(out->game_over_lock, NULL) != 0) {
        printf("Failed to create mutex for game over flag\n");
        exit(-1);
    }

    int err = pthread_create(&(out->thread_id), NULL, listen, out);
    if (err != 0) {
        printf("failed to generate the thread for the keyboad listener\n");
        exit(-1);
    }
    printf("finished initializing keyboard listener\n");
    return out;
}
