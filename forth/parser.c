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
		Token* t = next(p), *t1, *t2;
		switch(t->type) {
		case INT:
			stack_push(st, t);
			break;
		case WORD_DUP:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_top(st);
			t2 = copy_token(t1);
			stack_push(st, t2);
			break;
		case WORD_DROP:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			delete_token(t1);
			break;
		case WORD_SWAP:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t2 = stack_pop(st);
			stack_push(st, t1);
			stack_push(st, t2);
			break;
		case WORD_CL:
			clear_screen();
			return;
		case WORD_ABS:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			t1->int_value = abs(t1->int_value);
			stack_push(st, t1);
			break;
		case OP_PLUS:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t2 = stack_pop(st);
			t2->int_value += t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case OP_MINUS:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t2 = stack_pop(st);
			t2->int_value -= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case OP_MUL:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t2 = stack_pop(st);
			t2->int_value *= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case OP_DIV:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t2 = stack_pop(st);
			t2->int_value /= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case OP_MOD:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t2 = stack_pop(st);
			t2->int_value %= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case OP_LAST:
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			print_token_value(t1);
			delete_token(t1);
			break;
		case WORD:
			break;
		case EMPTY:
			puts("error: empty token\n");
			return;
		case INVALID:
			puts("error: invalid token\n");
			return;
		default:
			puts("error: unrecognized lexem\n");
			print_token(t);
			return;
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
