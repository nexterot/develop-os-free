#ifndef _CURSOR_H
#define _CURSOR_H
#include "sys.h"

void disable_cursor();
void enable_cursor(unsigned short int cursor_start, unsigned short int cursor_end);
void move_cursor(int x, int y);

#endif
