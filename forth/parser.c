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
		case TOKEN_INT:
			stack_push(st, t);
			break;
		case TOKEN_DUP:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_top(st);
			t2 = copy_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_DROP:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			delete_token(t1);
			break;
		case TOKEN_SWAP:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			stack_push(st, t1);
			stack_push(st, t2);
			break;
		case TOKEN_CL:
			clear_screen();
			return;
		case TOKEN_ABS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			t1->int_value = abs(t1->int_value);
			stack_push(st, t1);
			break;
		case TOKEN_PLUS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value += t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_MINUS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value -= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_MUL:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value *= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_DIV:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value /= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_MOD:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value %= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_DOT:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			print_token_value(t1);
			delete_token(t1);
			break;
		case TOKEN_WORD:
		case TOKEN_IF:
		case TOKEN_ELSE:
		case TOKEN_THEN:
		case TOKEN_EQ:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value = (t2->int_value == t1->int_value) ? 1 : 0;
			stack_push(st, t2);
			delete_token(t1);
			break;
		case TOKEN_MORE:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value = (t2->int_value > t1->int_value) ? 1 : 0;
			stack_push(st, t2);
			delete_token(t1);
			break;
		case TOKEN_LESS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value = (t2->int_value < t1->int_value) ? 1 : 0;
			stack_push(st, t2);
			delete_token(t1);
			break;
		case TOKEN_COLON:
		case TOKEN_SEMICOLON:
			break;
		default:
			puts("error: unrecognized token\n");
			print_token(t);
			return;
		}
	}
	if (tokens_num > 0) {
		puts("OK\n");
	}
	return;
	
L_STACK_UNDERFLOW:
	puts("error: stack underflow\n");
	return;
}

Token* next(Parser* p) {
	return p->tokens[p->tokens_pos++];
}

char has_next(Parser* p) {
	return p->tokens_pos < p->tokens_num;
}
