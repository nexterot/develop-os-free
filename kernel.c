/*
 *  Forth interpreter.
 */

#include "include/multiboot.h"
#include "sys.h"
#include "memory.h"
#include "printf.h"
#include "screen.h"
#include "cursor.h"
#include "time.h"
#include "keyboard.h"
 
void run();
 
/*  
 * Entry point accessed from 'loader.s'. 
 */
void main(multiboot_info_t* mbd, unsigned int magic) {   
    mem_init(mbd);
    key_init();
    rtc_seed();
	run();
}

void run() {
	char buff[256];
	for (;;) {
		gets(&buff);
		putchar('\n');
	}
}
