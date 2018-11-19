/*
 *  Contains higher level logic for kernel.
 *  In this example there is the Tetris game.
 * 
 *  Rules:
 *  Falling brick is made of '#' chars. After it falls down, 
 *  each char of it becomes '@'.
 *  You loose if top row contains any '@' char.
 *  Otherwise game may last endlessly.
 *  If you are bored, hit Esc, then continue after some time :).
 *  Completed rows (consist only of '@' chars) are immediately deleted.
 * 
 *  Controls:
 *  ESC         - pause/resume
 *  Arrow DOWN  - fall faster
 *  Arrow UP    - fall immediately;
 *  Arrow LEFT  - move left
 *  Arrow RIGHT - move right
 */

#include "include/multiboot.h"
#include "sys.h"
#include "memory.h"
#include "printf.h"
#include "screen.h"
#include "cursor.h"
#include "time.h"
#include "keyboard.h"

/* game field size */
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

/* ASCII chars used in game */
#define BRICK_CHAR  '#'
#define EMPTY_CHAR  ' '   
#define OTHER_CHAR  '@'
#define BORDER_CHAR '|'

/*
 * All possible bricks and their positions.
 * Suffix "_n" means rotated clockwise by n degrees.
 */
#define NUM_POS 19
#define NUM_BRICKS 7
 
enum BrickPos {
    I,
    I_90,
    J,
    J_90,
    J_180,
    J_270, // 5
    L,
    L_90,
    L_180,
    L_270,
    O, // 10
    S,
    S_90,
    T,
    T_90,
    T_180, // 15
    T_270,
    Z,
    Z_90,
};

/*
 * Structure for current falling brick.
 * x, y: coordinates of most left and top char of the brick.
 * next_x, next_y: coordinates where brick is moving next. They may be
 *               applied in game_update function.
 * type: type and rotation of the current block.
 */
struct Brick {
    int x, y;
    int next_x, next_y;
    enum BrickPos type;
};

/* game field */
char **field;
/* current falling brick */
struct Brick *brick;
/* buttons pressed and not released */
char arrow_left_pressed = 0;
char arrow_right_pressed = 0;
char arrow_down_pressed = 0;
char arrow_up_pressed = 0;
/* number of completed and deleted rows */
int rows_completed = 0;


void game_init();
void game_run();
char game_update();
void game_end();

void video_update();

char you_loose_check();

void gameover_display();
void pause_display();

void key_work();

void brick_gravity_fall();
void brick_spawn();

void draw_I(int x, int y, char c);
void draw_I_90(int x, int y, char c);
void draw_O(int x, int y, char c);

void row_delete(int row);
void rows_delete_completed();


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
        game_run();
        game_end();
    }
}

/*
 * Initializes game.
 */
void game_init() {
    clear_screen();
    key_buffer_clear();
    field = malloc(FIELD_WIDTH * sizeof(char*));
    for (int i = 0; i < FIELD_WIDTH; i++) {
        field[i] = malloc(FIELD_HEIGHT * sizeof(char));
        for (int j = 0; j < FIELD_HEIGHT; j++) {
            field[i][j] = EMPTY_CHAR;
        }
    }
    /* draw borders */
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
    brick_spawn();
}

/* 
 * Contains one game logic.
 */
void game_run() {   
    char done = 0;
    while (! done) {
        for (int i = 0; i < 5; i++) {
            key_work();
            video_update();
            delay(SECOND / 5);
        }
        brick_gravity_fall();
        game_update();
        video_update();
        if (you_loose_check()) {
            done = 1;
            gameover_display();
        }
    }
}

/*
 * Updates brick position to (next_x, next_y) if were no collisions.
 * Otherwise creates new brick.
 * Calls check for completed rows.
 * Returns 1 if brick stuck else 0.
 */
char game_update() {
    /* delete brick at prev pos */
    switch (brick->type) {
    case O:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR ||
                    field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR ||
                    brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_O(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_O(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_O(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case I:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 1) {
            brick->next_x = FIELD_WIDTH - 1;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x][brick->next_y+2] == OTHER_CHAR || 
                        field[brick->next_x][brick->next_y+3] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 3) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_I(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_I(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_I(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case I_90:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 4) {
            brick->next_x = FIELD_WIDTH - 4;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x+2][brick->next_y] == OTHER_CHAR || 
                        field[brick->next_x+3][brick->next_y] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_I_90(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_I_90(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_I_90(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case J:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+2] == OTHER_CHAR || 
                        field[brick->next_x][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_J(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_J(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_J(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case J_90:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+2][brick->next_y+1] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_J_90(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_J_90(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_J_90(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case J_180:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_J_180(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_J_180(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_J_180(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
     case J_270:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x+2][brick->next_y] == OTHER_CHAR || 
                        field[brick->next_x+2][brick->next_y+1] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_J_270(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_J_270(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_J_270(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case L:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x][brick->next_y+2] == OTHER_CHAR || 
                        field[brick->next_x+1][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_L(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_L(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_L(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case L_90:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x+2][brick->next_y] == OTHER_CHAR || 
                        field[brick->next_x+2][brick->next_y+1] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_L_90(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_L_90(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_L_90(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case L_180:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+1][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_L_180(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_L_180(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_L_180(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
     case L_270:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+2][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+2][brick->next_y] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_L_270(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_L_270(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_L_270(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case S:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+2][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_S(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_S(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_S(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case S_90:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+1][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_S_90(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_S_90(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_S_90(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case Z:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+2][brick->next_y+1] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_Z(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_Z(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_Z(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case Z_90:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_Z_90(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_Z_90(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_Z_90(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case T:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                    field[brick->next_x+2][brick->next_y] == OTHER_CHAR || 
                        field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_T(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_T(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_T(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case T_90:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+1][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_T_90(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_T_90(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_T_90(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case T_180:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 3) {
            brick->next_x = FIELD_WIDTH - 3;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x+1][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x+2][brick->next_y+1] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 1) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_T_180(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_T_180(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_T_180(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    case T_270:
        /* check boundary */
        if (brick->next_x > FIELD_WIDTH - 2) {
            brick->next_x = FIELD_WIDTH - 2;
        }
        if (brick->next_x < 0) {
            brick->next_x = 0;
        }
        /* collision */
        if (field[brick->next_x][brick->next_y] == OTHER_CHAR || 
                field[brick->next_x][brick->next_y+1] == OTHER_CHAR || 
                    field[brick->next_x+1][brick->next_y+1] == OTHER_CHAR || 
                        field[brick->next_x][brick->next_y+2] == OTHER_CHAR ||
                            brick->next_y >= FIELD_HEIGHT - 2) 
        {
            if (brick->x != brick->next_x) {
                brick->next_x = brick->x;
                return 0;
            }
            draw_T_270(brick->x, brick->y, OTHER_CHAR);
            brick_spawn();
            return 1;
        }
        /* delete brick at prev pos */
        draw_T_270(brick->x, brick->y, EMPTY_CHAR);
        /* draw brick at next pos */
        draw_T_270(brick->next_x, brick->next_y, BRICK_CHAR);
        break;
    }
    brick->x = brick->next_x;
    brick->y = brick->next_y;
    rows_delete_completed();
    return 0;
}

/*
 * Ends game and utilizes resources created by game_init.
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

/*
 * Writes borders and game field to screen.
 * Updates counter of cleared rows.
 */
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
    move_cursor(70, 1);
    printf(" %d ", brick->type);
}

/*
 * Checks if gamer looses.
 */
char you_loose_check() {
    for (int j = 0; j < FIELD_WIDTH; j++) {
        if (field[j][0] == OTHER_CHAR) {
            return 1;
        }
    }
    return 0;
}

/*
 * Displays game over and restarts game after 5 seconds.
 */
void gameover_display() {
    clear_screen();
    move_cursor(5, 5);
    puts("game over! starting new game after 5 seconds...");
    sleeps(5);
}

/*
 * Displays pause and waits for ESC to be pressed to return.
 */
void pause_display() {
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
 * Keyboard "callback". After it completes, game_update is called.
 * Keys:
 * ESC - displays pause;
 * Arrow DOWN - moves falling brick one pos lower;
 * Arrow UP - move brick down immediately;
 * Arrow LEFT & Arrow RIGHT - moves brick left and right respectively.
 */
void key_work() {
    int k = -1;
    char pressed;
    while (k != UNKNOWN) {
        key_decode(&k, &pressed);
        if (k == ESCAPE) {
            if (pressed) {
                pause_display();
            }
        }
        if (k == ARROW_DOWN) {
            if (pressed) {
                if (! arrow_down_pressed) {
                    brick->next_y++;
                    arrow_down_pressed = 1;
                }
            } else {
                arrow_down_pressed = 0;
            }
        }
        if (k == ARROW_LEFT) {
            if (pressed) {
                if (! arrow_left_pressed) {
                    brick->next_x--;
                    arrow_left_pressed = 1;
                }
            } else {
                arrow_left_pressed = 0;
            }
        }
        if (k == ARROW_RIGHT) {
            if (pressed) {
                if (! arrow_right_pressed) {
                    brick->next_x++;
                    arrow_right_pressed = 1;
                }
            } else {
                arrow_right_pressed = 0;
            }
        }
        if (k == ARROW_UP) {
            if (pressed) {
                if (! arrow_up_pressed) {
                    do {
                        brick->next_y++;
                    } while (game_update() == 0);
                    arrow_up_pressed = 1;
                }
            } else {
                arrow_up_pressed = 0;
            }
        }
        game_update();
    }
}

/*
 * Applies gravity fall to brick.
 */
void brick_gravity_fall() {
    brick->next_y++;
}

/*
 * Creates new random brick at the middle of top of game field.
 */
void brick_spawn() {
    brick->type = (rand()) % NUM_POS;
    brick->x = FIELD_WIDTH / 2;
    brick->y = 0;
    brick->next_x = brick->x;
    brick->next_y = brick->y;
}

/*
 * Prints brick of type I with top-left corner at (x,y) with char c.
 */
void draw_I(int x, int y, char c) {
    field[x][y] = c;
    field[x][y+1] = c;
    field[x][y+2] = c;
    field[x][y+3] = c;
}

/*
 * Prints brick of type I_90 with top-left corner at (x,y) with char c.
 */
void draw_I_90(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x+2][y] = c;
    field[x+3][y] = c;
}

/*
 * Prints brick of type J with top-left corner at (x,y) with char c.
 */
void draw_J(int x, int y, char c) {
    field[x+1][y] = c;
    field[x+1][y+1] = c;
    field[x+1][y+2] = c;
    field[x][y+2] = c;    
}

/*
 * Prints brick of type J_90 with top-left corner at (x,y) with char c.
 */
void draw_J_90(int x, int y, char c) {
    field[x][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c;
    field[x+2][y+1] = c;    
}

/*
 * Prints brick of type J_180 with top-left corner at (x,y) with char c.
 */
void draw_J_180(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x][y+1] = c;
    field[x][y+2] = c;    
}

/*
 * Prints brick of type J_270 with top-left corner at (x,y) with char c.
 */
void draw_J_270(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x+2][y] = c;
    field[x+2][y+1] = c;    
}

/*
 * Prints brick of type L with top-left corner at (x,y) with char c.
 */
void draw_L(int x, int y, char c) {
    field[x][y] = c;
    field[x][y+1] = c;
    field[x][y+2] = c;
    field[x+1][y+2] = c;    
}

/*
 * Prints brick of type L_90 with top-left corner at (x,y) with char c.
 */
void draw_L_90(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x+2][y] = c;
    field[x+2][y+1] = c;   
}

/*
 * Prints brick of type L_180 with top-left corner at (x,y) with char c.
 */
void draw_L_180(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x+1][y+1] = c;
    field[x+1][y+2] = c;    
}

/*
 * Prints brick of type L_270 with top-left corner at (x,y) with char c.
 */
void draw_L_270(int x, int y, char c) {
    field[x][y+1] = c;
    field[x+1][y+1] = c;
    field[x+2][y+1] = c;
    field[x+2][y] = c;    
}

/*
 * Prints brick of type O with top-left corner at (x,y) with char c.
 */
void draw_O(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c; 
}

/*
 * Prints brick of type S with top-left corner at (x,y) with char c.
 */
void draw_S(int x, int y, char c) {
    field[x+1][y] = c;
    field[x+2][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c; 
}

/*
 * Prints brick of type S_90 with top-left corner at (x,y) with char c.
 */
void draw_S_90(int x, int y, char c) {
    field[x][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c;
    field[x+1][y+2] = c; 
}

/*
 * Prints brick of type Z with top-left corner at (x,y) with char c.
 */
void draw_Z(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x+1][y+1] = c;
    field[x+2][y+1] = c; 
}

/*
 * Prints brick of type Z_90 with top-left corner at (x,y) with char c.
 */
void draw_Z_90(int x, int y, char c) {
    field[x+1][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c;
    field[x][y+2] = c; 
}

/*
 * Prints brick of type T with top-left corner at (x,y) with char c.
 */
void draw_T(int x, int y, char c) {
    field[x][y] = c;
    field[x+1][y] = c;
    field[x+2][y] = c;
    field[x+1][y+1] = c; 
}

/*
 * Prints brick of type T_90 with top-left corner at (x,y) with char c.
 */
void draw_T_90(int x, int y, char c) {
    field[x+1][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c;
    field[x+1][y+2] = c; 
}

/*
 * Prints brick of type T_180 with top-left corner at (x,y) with char c.
 */
void draw_T_180(int x, int y, char c) {
    field[x+1][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c;
    field[x+2][y+2] = c; 
}

/*
 * Prints brick of type T_270 with top-left corner at (x,y) with char c.
 */
void draw_T_270(int x, int y, char c) {
    field[x][y] = c;
    field[x][y+1] = c;
    field[x+1][y+1] = c;
    field[x][y+2] = c; 
}

/*
 * Deletes a completed row from game field.
 */
void row_delete(int row) {
    for (int k = row - 1; k >= 0; k--) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[j][k+1] != BRICK_CHAR && field[j][k] != BRICK_CHAR) {
                field[j][k+1] = field[j][k];
            }
        }
    }
}

/*
 * Checks for completed rows and deletes them if found.
 */
void rows_delete_completed() {
    for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
        int c = 0;
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[j][i] == OTHER_CHAR) {
                c++;
            }
        }
        if (c == FIELD_WIDTH) {
            rows_completed++;
            row_delete(i);
            i--;
            continue;
        }
    }
}

