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
 
enum LEXER_STATE {
	LEXER_STATE_INIT,
	LEXER_STATE_INT,
	LEXER_STATE_WORD,
	LEXER_STATE_MINUS,
	LEXER_STATE_OP,
};
 
/* Lexer */
typedef struct lexer{
	enum LEXER_STATE state;
} Lexer;

/* TOKEN_TYPE */
enum TOKEN_TYPE {
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

 /* Token */
typedef struct token{
	 int int_value;
	 char* value;
	 unsigned int value_len;
	 enum TOKEN_TYPE type;
} Token;

/* Lexer methods */
Lexer* new_lexer();
Token* next_token(Lexer* l, char* buff, int* buff_shift);
void skip_spaces(char *buff, int* buff_shift);

/* Token methods */
void print_token(Token* t);
void print_token_value(Token* t);
void delete_token(Token* t);
Token* new_token();
Token* copy_token(Token* t);

/* Extra */
char is_digit(char c);
char is_lower(char c);
char is_alpha(char c);
char to_upper(char c);
char str_cmp(char *s1, const char *s2);
char is_space(char c);
int atoi(const char* str);
void* memcpy(void* destptr, const void* srcptr, size_t num);
#endif
