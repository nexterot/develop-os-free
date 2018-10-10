#include "screen.h"

#define BLACK         0x0
#define BLUE          0x1
#define GREEN         0x2
#define RED           0x4
#define WHITE         0x7
#define FG_COLOR      WHITE
#define BG_COLOR      BLACK

#define MAX_COL       VGA_WIDTH	
#define MAX_ROW       VGA_HEIGHT
#define VRAM_SIZE     (MAX_COL*MAX_ROW)
#define DEF_VRAM_BASE 0xb8000

extern int cursor_x;
extern int cursor_y;

#define PUT(c) ( ((unsigned short *) (DEF_VRAM_BASE)) \
	[(cursor_y * MAX_COL) + cursor_x] = (FG_COLOR << 8 | (BG_COLOR << 12)) | (c))

static void cons_putc(int c) {
    switch (c) {
    case '\t':
        do {
            cons_putc(' ');
        } while ((cursor_x % 4) != 0);
        break;
    case '\r':
        cursor_x = 0;
        break;
    case '\n':
        cursor_y += 1;
        if (cursor_y >= MAX_ROW) {
            cursor_y = 0;
        }
        break;
    case '\b':
        if (cursor_x > 0) {
            cursor_x -= 1;
            PUT(' ');
        }
        break;
    default:
        PUT(c);
        cursor_x += 1;
        if (cursor_x >= MAX_COL) {
            cursor_x = 0;
            cursor_y += 1;
            if (cursor_y >= MAX_ROW) {
                cursor_y = 0;
            }
        }
    };
}

void _putchar(int c) {
    if (c == '\n') 
        cons_putc('\r');
    cons_putc(c);
}

void putchar(int c) {
    _putchar(c);
    update_cursor();
}

int puts(const char* s) {
	char c;
	int i = -1;
	while ((c = s[++i]) != '\0') {
		_putchar(c);
	}
	update_cursor();
	return i;
}

void clear_screen() {
    cursor_x = 0;
    cursor_y = 0;
    
    for (int i = 0; i < VRAM_SIZE; i++)
        cons_putc(' ');
    
    cursor_x = 0;
    cursor_y = 0;
    move_cursor(0, 0);
}
