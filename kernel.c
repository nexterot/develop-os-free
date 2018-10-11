#include "printf.h"
#include "screen.h"
#include "types.h"
#include "cursor.h"

int cursor_x;
int cursor_y;

void main()
{   
    clear_screen();
    char string[256];
    for (;;) {
		gets(string);
		putchar('\n');
		puts(string);
		putchar('\n');
	}
}
