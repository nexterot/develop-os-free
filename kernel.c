/*
 *  Contains higher level logic for kernel.
 *  In this example there is a tetris game.
 */

#include "include/multiboot.h"
#include "memory.h"
#include "printf.h"
#include "screen.h"
#include "time.h"
#include "keyboard.h"

void program();
void game_init();
void tick();

/*  
 * Entry point accessed from 'loader.s'. 
 */
void main(multiboot_info_t* mbd, unsigned int magic) {   
    clear_screen();
    mem_init(mbd);
    program();
}

/* 
 * Contains user-defined program for the kernel.
 */
void program() {   
    game_init();
    char done = 0;
    while (! done) {
        key_work();
        tick();
    }
}

void game_init() {
    key_init();
}

void key_work() {
    int k;
    char pressed;
    key_decode(&k, &pressed);
    
    if (k == ARROW_UP) {
        if (pressed) {
            move_cursor_delta(0, -1);
        }
    }
    if (k == ARROW_DOWN) {
        if (pressed) {
            move_cursor_delta(0, 1);
        }
    }
    if (k == ARROW_LEFT) {
        if (pressed) {
            move_cursor_delta(-1, 0);
        }
    }
    if (k == ARROW_RIGHT) {
        if (pressed) {
            move_cursor_delta(1, 0);
        }
    }
}

void tick() {
    delay(SECOND / 1000);
}
