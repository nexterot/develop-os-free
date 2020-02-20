/*
 *  Parser.
 */
 
#include "parser.h"

/* Parser methods */
void parse(Parser* p, Stack* st, Token** tokens, int tokens_num) {
	p->tokens = tokens;
	p->tokens_num = tokens_num;
	p->tokens_pos = 0;
	while (has_next(p)) {
		Token* t = next(p);
		switch(t->type) {
		case INT:
			stack_push(st, t);
			break;
		case WORD_DUP:
			break;
		case WORD_DROP:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			stack_pop(st);
			break;
		case WORD_SWAP:
			break;
		case WORD_CL:
			clear_screen();
			return;
		case OP_PLUS:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			Token* t1 = stack_pop(st);
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			Token* t2 = stack_pop(st);
			t1->int_value += t2->int_value;
			// should delete t2
			stack_push(st, t1);
			break;
		case OP_MINUS:
			break;
		case OP_MUL:
			break;
		case OP_DIV:
			break;
		case OP_MOD:
			break;
		case OP_LAST:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			Token* t = stack_pop(st);
			print_token_value(t);
			break;
		case WORD:
			break;
		case EMPTY:
			break;
		case INVALID:
			puts("error: invalid token\n");
			break;
		default:
			puts("error: unrecognized lexem\n");
			print_token(&t);
		}
	}
	puts("OK\n");
}

Token* next(Parser* p) {
	return p->tokens[p->tokens_pos++];
}

char has_next(Parser* p) {
	return p->tokens_pos < p->tokens_num;
}
