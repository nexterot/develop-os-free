#include "printf.h"
#include "screen.h"
#include "types.h"
#include "cursor.h"

int cursor_x;
int cursor_y;

void main()
{   
    clear_screen();
    disable_cursor();
    enable_cursor(0, 15);
    printf("hello");
    move_cursor(5, 0);
}
