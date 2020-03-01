/*
 *  Stack.
 */
 
#include "stack.h"

/* Stack methods */
void init_stack(Stack* s, int n) {
	s->data = (Token**) malloc(n * sizeof(Token*));
	s->cap = n;
	s->top = 0;
}

char stack_empty(Stack *s) {
	return s->top == 0;
}

void stack_push(Stack *s, Token *t) {
	s->data[s->top] = t;
	s->top++;
}

Token* stack_pop(Stack *s) {
	s->top--;
	return s->data[s->top];
}

Token* stack_top(Stack *s) {
	return s->data[s->top-1];
}

void execute_word(Stack* s, Dict* dic, DictElem* d) {
	//printf("executing word %s, len=%d\n", d->name, d->tokens_len);
	for (int i = 0; i < d->tokens_len; i++) {
		Token* t = copy_token(d->tokens[i]);
		char err = execute_token(s, dic, t);
		if (err) return;
	}
}

char execute_token(Stack* st, Dict* dic, Token* t) {
	/*
	puts("executing token: ");
	print_token(t);
	puts("\n");
	*/
	Token *t1, *t2;
	DictElem *d;
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
		break;
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
		d = find_word(dic, t->value);
		if (d == NULL) {
			puts("error: unknown word\n");
			return 1;
		}
		execute_word(st, dic, d);
		break;
	case TOKEN_IF:
	case TOKEN_ELSE:
	case TOKEN_THEN:
		puts("error: only in compilation state\n");
		return 1;
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
		puts("error: compilation inside a word\n");
		return 1;
	default:
		puts("error: unrecognized token\n");
		print_token(t);
		putchar('\n');
		return 1;
	}
	return 0;
	
L_STACK_UNDERFLOW:
	puts("error: stack underflow\n");
	return 1;
}
