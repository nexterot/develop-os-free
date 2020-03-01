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

#include "forth/stack.h"
#include "forth/lexer.h"
#include "forth/parser.h"

 
void main(multiboot_info_t* mbd, unsigned int magic) {   
    mem_init(mbd);
    key_init();
    rtc_seed();
    
    Lexer* lex = new_lexer();
    Parser* prs = new_parser();
    
    Stack st;
    init_stack(&st, DATA_STACK_SIZE);
    
    Dict dic;
    dic.start = NULL;
    
    char buff[LINE_BUFFER_SIZE];
    Token** tokens = (Token**) malloc(TOKENS_BUFFER_SIZE * sizeof(Token*));
	for (;;) {
		int i = 0, j = 0;
		puts("> ");
		gets(buff);
		putchar('\n');
		Token* t;
		while (buff[i] != '\0') {
			skip_spaces(buff, &i);
			if (buff[i] == '\0') break;
			t = next_token(lex, buff, &i);
			// puts("\ntoken: ");
			// print_token(t);
			if (t == NULL) {
				puts("lex error\n");
				skip(buff, &i);
				continue;
			}
			tokens[j++] = t;
		}
		// printf("\nj = %d\n", j);
		parse(prs, &st, &dic, tokens, j);
	}
}
