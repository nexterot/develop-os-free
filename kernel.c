#include "include/multiboot.h"
#include "memory.h"
#include "printf.h"
#include "screen.h"

int cursor_x;
int cursor_y;

void program();

void main(multiboot_info_t* mbd, unsigned int magic)
{   
	clear_screen();
	mem_init(mbd);
	program();
}

void program() {
	int len = 20;
	long long* array = (long long*)malloc(len);
	if (array == NULL) {
		printf("null!\n");
		return;
	}
	for (int i = 0; i < len; i++) {
		array[i] = i * i;
	}
	for (int i = 0; i < len; i++) {
		printf("array[%d]=%llu, &array[%d]=%p\n", i, array[i], i, &array[i]);
	}
	char* invalid = (char*)malloc(100000000);
	if (invalid == NULL) {
		printf("null!\n");
		return;
	}
}
