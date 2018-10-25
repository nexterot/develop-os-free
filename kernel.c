/*
 *  Contains higher level logic for kernel.
 *  In this example it is shown how to use malloc/free.
 */

#include "include/multiboot.h"
#include "memory.h"
#include "printf.h"
#include "screen.h"

void program();

/*  
 * Entry point accessed from 'loader.s'. 
 */
void main(multiboot_info_t* mbd, unsigned int magic) {   
    clear_screen();
    mem_init(mbd);
    program();
}

/* 
 * Contains user-defined program for the kernel.
 */
void program() {    
    /* initialize 1000 x 1000 dynamic array of ints */
    int len = 1000;
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
    /* should print x^3 for x in range [1, 10) */
    for (int i = 1; i <= 10; i++) {
        printf("%d\n", multi_array[i][i * i]);
    }
    /* try to malloc more memory (assumed we have ~15Mbytes RAM) */
    const int THIRTEEN_MILLION = 13000000;
    char* more_memory = (char*) malloc(THIRTEEN_MILLION * sizeof(char));
    if (more_memory == NULL) {
        puts("should free up previous memory first!\n");
    }
    for (int i = 0; i < len; i++) {
        free(multi_array[i], len * sizeof(int));
    }
    free(multi_array, len * sizeof(int*));
    /* another try */
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
