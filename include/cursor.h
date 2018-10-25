/*
 * Contains functions to work with cursor on screen
 */

#ifndef _CURSOR_H
#define _CURSOR_H

#include "sys.h"

int cursor_x, cursor_y;

void disable_cursor();
void enable_cursor(unsigned short int cursor_start, unsigned short int cursor_end);
void move_cursor(int x, int y);
void update_cursor();

#endif
