#include "printf.h"
#include "screen.h"
#include "types.h"
#include "cursor.h"

int cursor_x;
int cursor_y;

void main()
{   
    clear_screen();
    enable_cursor(0, 15);
    char *s = "Hello, world!\n";
    s[5] = '!';
    puts(s);
    putchar(s[5]);
}
