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
	int len = 1000;
	// initialize multi-dimensional array
	int** multi_array = (int**)malloc(len * sizeof(int*));
	if (multi_array == NULL) {
		puts("null!\n");
		return;
	}
	for (int i = 0; i < len; i++) {
		multi_array[i] = malloc(len * sizeof(int));
		if (multi_array[i] == NULL) {
			printf("null %d!\n", i);
			return;
		}
		for (int j = 0; j < len; j++) {
			multi_array[i][j] = i * j;
		}
	}
	// print x^3 for x in range [1, 10)
	for (int i = 1; i <= 10; i++) {
		printf("%d\n", multi_array[i][i * i]);
	}
	const int THIRTEEN_MILLION = 13000000;
	// try to malloc more memory (assumed we have ~15Mbytes RAM)
	char* more_memory = (char*) malloc(THIRTEEN_MILLION * sizeof(char));
	if (more_memory == NULL) {
		puts("should free up previous memory first!\n");
	}
	// free memory
	for (int i = 0; i < len; i++) {
		free(multi_array[i], len * sizeof(int));
	}
	free(multi_array, len * sizeof(int*));
	// another try
	more_memory = (char*) malloc(THIRTEEN_MILLION * sizeof(char));
	if (more_memory == NULL) {
		puts("shouldn't see this message\n");
		return;
	} else {
		puts("success\n");
	}
	free(more_memory, THIRTEEN_MILLION * sizeof(char));
	puts("done\n");
}
