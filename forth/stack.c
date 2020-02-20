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
