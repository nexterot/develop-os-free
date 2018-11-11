/*
 * Contains keyboard input functions.
 */
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "sys.h"
#include "memory.h"

/* arrows codes */ 
#define UNKNOWN -1
#define ARROW_UP -2
#define ARROW_DOWN -3
#define ARROW_LEFT -4
#define ARROW_RIGHT -5

int get_key();
void key_polling();
void key_decode(int *key, char *pressed);

#endif
