/*
 * Contains functions to work with time.
 */

#ifndef _TIME_H
#define _TIME_H

#include "sys.h"
#include "keyboard.h"

#define SECOND 1193182  /* 1 second ~ 1193182 ticks */

void delay(unsigned int ticks);
void sleeps(unsigned int seconds);

#endif
