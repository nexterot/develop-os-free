#ifndef _SYS_H
#define _SYS_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

unsigned char inb(unsigned short int port);
void outb(unsigned char value, unsigned short int port);

#endif
