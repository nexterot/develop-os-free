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

#include "forth/lexer.h"
#include "forth/parser.h"
 

void main(multiboot_info_t* mbd, unsigned int magic) {   
    mem_init(mbd);
    key_init();
    rtc_seed();
    
    Lexer lex;
    Parser prs;
    
	Token t;
	for (;;) {
		next_token(&lex, &t);
		print_token(&t);
		delete_token(&t);
	}
}
