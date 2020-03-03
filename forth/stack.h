/*
 *  Stack.
 */
#ifndef _STACK_H
#define _STACK_H
 
#include "lexer.h"

#define DATA_STACK_SIZE 1024

/* Stack */
typedef struct stack {
	Token** data;
	int cap;
	int top;
} Stack;

/* RetStack */
typedef struct ret_stack {
	int* data;
	int cap;
	int top;
} RetStack;


/* Stack methods */
void init_stack(Stack *st, int n);
char stack_empty(Stack *st);
void stack_push(Stack *st, Token *t);
Token* stack_pop(Stack *st);
Token* stack_top(Stack *st);

/* RetStack methods */
void init_ret_stack(RetStack *st, int n);
char rstack_empty(RetStack *st);
void rstack_push(RetStack *st, int x);
int rstack_pop(RetStack *st);
int rstack_top(RetStack *st);

/* Forth methods */
void _dup(Stack *st, RetStack* ret_st);
void _cl(Stack *st, RetStack* ret_st);
void _dot(Stack *st, RetStack* ret_st);
void _dot_s(Stack *st, RetStack* ret_st);
void _drop(Stack *st, RetStack* ret_st);
void _swap(Stack *st, RetStack* ret_st);
void _forget(Stack *st, RetStack* ret_st);
void _abs(Stack *st, RetStack* ret_st);
void _sum(Stack *st, RetStack* ret_st);
void _sub(Stack *st, RetStack* ret_st);
void _mul(Stack *st, RetStack* ret_st);
void _div(Stack *st, RetStack* ret_st);
void _mod(Stack *st, RetStack* ret_st);
void _eq(Stack *st, RetStack* ret_st);
void _more(Stack *st, RetStack* ret_st);
void _less(Stack *st, RetStack* ret_st);
void _ret_to_st(Stack *st, RetStack* ret_st);
void _st_to_ret(Stack *st, RetStack* ret_st);
void _ret_copy_st(Stack *st, RetStack* ret_st);

#endif
