/*
 *  Contains higher level logic for kernel.
 *  In this example there is a tetris game.
 */

#include "sys.h"
#include "include/multiboot.h"
#include "memory.h"
#include "printf.h"
#include "screen.h"
#include "cursor.h"
#include "time.h"
#include "keyboard.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

/* O,I,J,L,Z,S,T */
#define NUM_BRICKS 3

#define BRICK_CHAR  '#'
#define EMPTY_CHAR  ' '   
#define OTHER_CHAR  '@'
#define BORDER_CHAR '|'

void game_init();
void game();
void game_update();
void video_update();
char check_loose();
void display_gameover();
void game_end();

void show_pause();

void key_work();
void brick_fall();
void brick_new();

void draw_I(int x, int y, char c);
void draw_I_90(int x, int y, char c);
void draw_O(int x, int y, char c);

void delete_row();
void delete_completed_rows();

enum BrickType {
    O,
    I,
    I_90,
    J,
    L,
    Z,
    S,
    T,
    J_90,
    J_180,
    J_270,
    L_90,
    L_180,
    L_270,
    Z_90,
    S_90,
    T_90,
    T_180,
    T_270
};

struct Brick {
    int x, y;
    int new_x, new_y;
    enum BrickType type;
};

/* game field */
char **field;
/* current falling brick */
struct Brick *brick;

char arrow_left_pressed = 0;
char arrow_right_pressed = 0;
char arrow_down_pressed = 0;
char arrow_up_pressed = 0;

int rows_completed = 0;

/*  
 * Entry point accessed from 'loader.s'. 
 */
void main(multiboot_info_t* mbd, unsigned int magic) {   
    mem_init(mbd);
    key_init();
    rtc_seed();
    disable_cursor();
    for (;;) {
        game_init();
        game();
        game_end();
    }
}

/* 
 * Contains game logic.
 */
void game() {   
    char done = 0;
    while (! done) {
        for (int i = 0; i < 5; i++) {
            key_work();
            video_update();
            delay(SECOND / 5);
        }
        brick_fall();
        game_update();
        video_update();
        if (check_loose()) {
            done = 1;
            display_gameover();
        }
    }
}

/*
 * Initializes game.
 */
void game_init() {
    clear_screen();
    field = malloc(FIELD_WIDTH * sizeof(char*));
    for (int i = 0; i < FIELD_WIDTH; i++) {
        field[i] = malloc(FIELD_HEIGHT * sizeof(char));
        for (int j = 0; j < FIELD_HEIGHT; j++) {
            field[i][j] = EMPTY_CHAR;
        }
    }
    /* borders */
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        /* left */
        move_cursor(VGA_WIDTH/2 - FIELD_WIDTH/2 - 1, VGA_HEIGHT/2 - FIELD_HEIGHT/2 + i);
        putchar(BORDER_CHAR);
        /* right */
        move_cursor(VGA_WIDTH/2 + FIELD_WIDTH/2, VGA_HEIGHT/2 - FIELD_HEIGHT/2 + i);
        putchar(BORDER_CHAR);
    }
    /* set current falling brick */
    brick = malloc(sizeof(struct Brick));
    brick_new();
}

void show_pause() {
    clear_screen();
    move_cursor(5, 5);
    puts("paused... press ESC to return to game...");
    
    int k = 0;
    char pressed = 0;
    while (! (k == ESCAPE && pressed)) {
        key_decode(&k, &pressed);
        delay(SECOND / 50);
    }
    
    clear_screen();
}

/*
 * Display "game over!".
 */
void display_gameover() {
    clear_screen();
    move_cursor(5, 5);
    puts("game over! starting new game after 5 seconds...");
    sleeps(5);
}

/*
 * Ends game.
 */
void game_end() {
    arrow_left_pressed = 0;
    arrow_right_pressed = 0;
    arrow_down_pressed = 0;
    arrow_up_pressed = 0;
    rows_completed = 0;
    
    for (int i = 0; i < FIELD_WIDTH; i++) {
        free(field[i], FIELD_HEIGHT * sizeof(char));
    }
    free(field, FIELD_WIDTH * sizeof(char*));
    free(brick, sizeof(struct Brick));
}

void key_work() {
    int k = 0;
    char pressed;
    while (k != UNKNOWN) {
        key_decode(&k, &pressed);
        if (k == ESCAPE) {
            if (pressed) {
                show_pause();
            }
        }
        if (k == ARROW_DOWN) {
            if (pressed) {
                if (! arrow_down_pressed) {
                    brick->new_y++;
                    arrow_down_pressed = 1;
                }
            } else {
                arrow_down_pressed = 0;
            }
        }
        if (k == ARROW_LEFT) {
            if (pressed) {
                if (! arrow_left_pressed) {
                    brick->new_x--;
                    arrow_left_pressed = 1;
                }
            } else {
                arrow_left_pressed = 0;
            }
        }
        if (k == ARROW_RIGHT) {
            if (pressed) {
                if (! arrow_right_pressed) {
                    brick->new_x++;
                    arrow_right_pressed = 1;
                }
            } else {
                arrow_right_pressed = 0;
            }
        }
        game_update();
    }
}

void video_update() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        move_cursor(VGA_WIDTH/2 - FIELD_WIDTH/2, VGA_HEIGHT/2 - FIELD_HEIGHT/2 + i);
        for (int j = 0; j < FIELD_WIDTH; j++) {
            putchar(field[j][i]);
        }
    }
    /* borders */
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        /* left */
        move_cursor(VGA_WIDTH/2 - FIELD_WIDTH/2 - 1, VGA_HEIGHT/2 - FIELD_HEIGHT/2 + i);
        putchar(BORDER_CHAR);
        /* right */
        move_cursor(VGA_WIDTH/2 + FIELD_WIDTH/2, VGA_HEIGHT/2 - FIELD_HEIGHT/2 + i);
        putchar(BORDER_CHAR);
    }
    move_cursor(1, 1);
    printf("%d rows cleared", rows_completed);
}

void brick_fall() {
    brick->new_y++;
}

void draw_O(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c;
}

void draw_I(int x, int y, char c) {
    field[x][y] = c;
    field[x][y+1] = c;
    field[x][y+2] = c;
    field[x][y+3] = c;
}

void draw_I_90(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x+2][y] = c;
    field[x+3][y] = c;
}

void brick_new() {
    brick->type = (rand()) % NUM_BRICKS;
    brick->x = FIELD_WIDTH / 2;
    brick->y = 0;
    brick->new_x = brick->x;
    brick->new_y = brick->y;
}

void delete_row(int row) {
    for (int k = row - 1; k >= 0; k--) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[j][k+1] != BRICK_CHAR && field[j][k] != BRICK_CHAR) {
                field[j][k+1] = field[j][k];
            }
        }
    }
}

void delete_completed_rows() {
    for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
        int c = 0;
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[j][i] == OTHER_CHAR) {
                c++;
            }
        }
        if (c == FIELD_WIDTH) {
            rows_completed++;
            delete_row(i);
            i--;
            continue;
        }
    }
}

char check_loose() {
    for (int j = 0; j < FIELD_WIDTH; j++) {
        if (field[j][0] == OTHER_CHAR) {
            return 1;
        }
    }
    return 0;
}

void game_update() {
    /* delete brick at prev pos */
    switch (brick->type) {
    case O:
        /* check boundary */
        if (brick->new_x >= FIELD_WIDTH - 1) {
            brick->new_x = FIELD_WIDTH - 2;
        }
        if (brick->new_x < 0) {
            brick->new_x = 0;
        }
        /* collision */
        if (field[brick->new_x][brick->new_y] == OTHER_CHAR || 
                field[brick->new_x+1][brick->new_y] == OTHER_CHAR ||
                    field[brick->new_x][brick->new_y+1] == OTHER_CHAR || 
                        field[brick->new_x+1][brick->new_y+1] == OTHER_CHAR ||
                    brick->new_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->new_x) {
                brick->new_x = brick->x;
                return;
            }
            draw_O(brick->x, brick->y, OTHER_CHAR);
            brick_new();
            return;
        }
        /* delete brick at prev pos */
        draw_O(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_O(brick->new_x, brick->new_y, BRICK_CHAR);
        break;
    case I:
        /* check boundary */
        if (brick->new_x > FIELD_WIDTH - 1) {
            brick->new_x = FIELD_WIDTH - 1;
        }
        if (brick->new_x < 0) {
            brick->new_x = 0;
        }
        /* collision */
        if (field[brick->new_x][brick->new_y] == OTHER_CHAR || 
                field[brick->new_x][brick->new_y+1] == OTHER_CHAR || 
                    field[brick->new_x][brick->new_y+2] == OTHER_CHAR || 
                        field[brick->new_x][brick->new_y+3] == OTHER_CHAR ||
                            brick->new_y >= FIELD_HEIGHT - 3) 
        {
            if (brick->x != brick->new_x) {
                brick->new_x = brick->x;
                return;
            }
            draw_I(brick->x, brick->y, OTHER_CHAR);
            brick_new();
            return;
        }
        /* delete brick at prev pos */
        draw_I(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_I(brick->new_x, brick->new_y, BRICK_CHAR);
        break;
    case I_90:
        /* check boundary */
        if (brick->new_x > FIELD_WIDTH - 4) {
            brick->new_x = FIELD_WIDTH - 4;
        }
        if (brick->new_x < 0) {
            brick->new_x = 0;
        }
        /* collision */
        if (field[brick->new_x][brick->new_y] == OTHER_CHAR || 
                field[brick->new_x+1][brick->new_y] == OTHER_CHAR || 
                    field[brick->new_x+2][brick->new_y] == OTHER_CHAR || 
                        field[brick->new_x+3][brick->new_y] == OTHER_CHAR ||
                            brick->new_y >= FIELD_HEIGHT) 
        {
            if (brick->x != brick->new_x) {
                brick->new_x = brick->x;
                return;
            }
            draw_I_90(brick->x, brick->y, OTHER_CHAR);
            brick_new();
            return;
        }
        /* delete brick at prev pos */
        draw_I_90(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_I_90(brick->new_x, brick->new_y, BRICK_CHAR);
        break;
    }
    brick->x = brick->new_x;
    brick->y = brick->new_y;
    delete_completed_rows();
}
