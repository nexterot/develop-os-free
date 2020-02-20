/*
 *  Lexer.
 */
 
/* Lexer */
struct lexer{};
typedef struct lexer Lexer;

/* TOKEN_TYPE */
enum token_type {
	INT,
	WORD,
	OP_PLUS,
	OP_MINUS,
	OP_MUL,
	OP_DIV,
	OP_MOD,
};
typedef enum token_type TOKEN_TYPE;

 /* Token */
struct token{
	 char* value;
	 unsigned int value_len;
	 TOKEN_TYPE type;
};
typedef struct token Token;

/* Lexer methods */
void next_token(Lexer* l, Token* t);

/* Token methods */
void print_token(Token* t);
void delete_token(Token* t);

/* Extra */
char is_digit(char c);
