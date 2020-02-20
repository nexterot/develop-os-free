/*
 *  Lexer.
 */
#ifndef _LEXER_H
#define _LEXER_H
 
#include "memory.h"
#include "screen.h"
#include "printf.h"
 
#define LINE_BUFFER_SIZE 256
#define TOKENS_BUFFER_SIZE 128
 
/* Lexer */
struct lexer{};
typedef struct lexer Lexer;

/* TOKEN_TYPE */
enum token_type {
	INT,
	WORD,
	WORD_DUP,
	WORD_DROP,
	WORD_SWAP,
	WORD_CL,
	OP_PLUS,
	OP_MINUS,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_LAST,
	INVALID,
	EMPTY,
};
typedef enum token_type TOKEN_TYPE;

 /* Token */
struct token{
	 int int_value;
	 char* value;
	 unsigned int value_len;
	 TOKEN_TYPE type;
};
typedef struct token Token;

/* Lexer methods */
Token* next_token(Lexer* l, char* buff, int* buff_shift);
void skip_spaces(char *buff, int* buff_shift);

/* Token methods */
void print_token(Token* t);
void print_token_value(Token* t);
void delete_token(Token* t);

/* Extra */
char is_digit(char c);
char is_lower(char c);
char is_alpha(char c);
char to_upper(char c);
char str_cmp(char *s1, const char *s2);
char is_space(char c);
int atoi(const char* str);

#endif
