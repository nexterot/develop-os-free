#include "cursor.h"

extern int cursor_x;
extern int cursor_y;

void disable_cursor() {
	outb(0x0A, 0x3D4);
	outb(0x20, 0x3D5);
}

void enable_cursor(unsigned short int cursor_start, unsigned short int cursor_end) {
	outb(0x0A, 0x3D4);
	outb((inb(0x3D5) & 0xC0) | cursor_start, 0x3D5);
 
	outb(0x0B, 0x3D4);
	outb((inb(0x3E0) & 0xE0) | cursor_end, 0x3D5);
}

void move_cursor(int x, int y) {
	int pos = VGA_WIDTH * y + x;
	outb(0x0F, 0x3D4);
	outb((unsigned char) (pos & 0xFF), 0x3D5);
	outb(0x0E, 0x3D4);
	outb((unsigned char) ((pos >> 8) & 0xFF), 0x3D5);
	cursor_x = x;
	cursor_y = y;
}

void update_cursor() {
	move_cursor(cursor_x, cursor_y);
}
