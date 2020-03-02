/*
 *  Stack.
 */
#ifndef _STACK_H
#define _STACK_H
 
#include "lexer.h"

#define DATA_STACK_SIZE 1024*1024*64

/* Stack */
struct stack {
	Token** data;
	int cap;
	int top;
};
typedef struct stack Stack;

/* Stack methods */
void init_stack(Stack *st, int n);
char stack_empty(Stack *st);
void stack_push(Stack *st, Token *t);
Token* stack_pop(Stack *st);
Token* stack_top(Stack *st);

void _dup(Stack *st);
void _cl(Stack *st);
void _dot(Stack *st);
void _dot_s(Stack *st);
void _drop(Stack *st);
void _swap(Stack *st);
void _forget(Stack *st);
void _abs(Stack *st);
void _sum(Stack *st);
void _sub(Stack *st);
void _mul(Stack *st);
void _div(Stack *st);
void _mod(Stack *st);
void _eq(Stack *st);
void _more(Stack *st);
void _less(Stack *st);

#endif
