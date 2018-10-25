/*
 * Contains system constants and port read & write functions
 */

#ifndef _SYS_H
#define _SYS_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void outb(unsigned char value, unsigned short int port);
unsigned char inb(unsigned short int port);

#endif
