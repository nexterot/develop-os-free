#ifndef _SCREEN_H
#define _SCREEN_H
#include "types.h"
#include "sys.h"
#include "cursor.h"

void clear_screen();
void putchar(int c);
int puts(const char* s);
int getchar();
char* gets(char* s);

#endif
