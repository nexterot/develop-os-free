#include "printf.h"
#include "screen.h"
#include "time.h"

int cursor_x;
int cursor_y;

void main()
{   
    int i = 0;
    for (;;) {
		clear_screen();
		printf("Time spent: %d", i += 1);
		delay(50);
	}
}
