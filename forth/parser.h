/*
 *  Parser.
 */
#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"
#include "stack.h"
#include "printf.h"

typedef struct par{
	Token** tokens;
	int    tokens_num;
	int    tokens_pos;
} Parser;

/* Parser methods */
void parse(Parser* p, Stack* st, Token** tokens, int tokens_num);
Token* next(Parser* p);
char has_next(Parser* p);

#endif
