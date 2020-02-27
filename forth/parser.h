/*
 *  Parser.
 */
#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"
#include "stack.h"
#include "printf.h"

 
enum PARSER_STATE {
	PARSER_STATE_COMPILE,
	PARSER_STATE_EXECUTE,
};

enum PARSER_RULE {
	PARSER_RULE_START,
	PARSER_RULE_BODY,
};

typedef struct par{
	Token** tokens;
	int    tokens_num;
	int    tokens_pos;
	enum PARSER_STATE state;
	enum PARSER_RULE rule;
} Parser;

/* Parser methods */
Parser* new_parser();
void parse(Parser* p, Stack* st, Token** tokens, int tokens_num);
Token* next(Parser* p);
char has_next(Parser* p);

#endif
