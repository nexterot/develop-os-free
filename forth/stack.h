/*
 *  Stack.
 */
#ifndef _STACK_H
#define _STACK_H
 
#include "lexer.h"
#include "dict.h"

#define DATA_STACK_SIZE 1024*1024*64

/* Stack */
struct stack {
	Token** data;
	int cap;
	int top;
};
typedef struct stack Stack;

/* Stack methods */
void init_stack(Stack *s, int n);
char stack_empty(Stack *s);
void stack_push(Stack *s, Token *t);
Token* stack_pop(Stack *s);
Token* stack_top(Stack *s);
void execute_word(Stack* st, Dict* dic, DictElem* d);

#endif
