/*
 *  Lexer.
 */
 
#include "lexer.h"

/* Lexer methods */
Lexer* new_lexer() {
	Lexer* l = (Lexer*) malloc(sizeof(Lexer));	
	l->state = LEXER_STATE_EXECUTE;
	l->read = LEXER_READ_START;
	return l;
}

Token* next_token(Lexer* l, char* buff, int* buff_shift) {
	l->read = LEXER_READ_START;
	int b_shift = *buff_shift;
	int i;
	Token* t = (Token*) malloc(sizeof(Token));
	for (i = 0; b_shift+i < LINE_BUFFER_SIZE; i++, (*buff_shift)++) {
		char c = buff[b_shift+i] = to_upper(buff[b_shift+i]);
		switch(l->state) {
		case LEXER_STATE_COMPILE:
			puts("COMPILE STATE\n");
			goto L_NULL;
		case LEXER_STATE_EXECUTE:
			switch(l->read) {
			case LEXER_READ_START:
				if (is_whitespace(c) || c == '\0') {
					goto L_NULL;
				} else if (c == '/') {
					l->read = LEXER_READ_DIV;
					t->type = TOKEN_DIV;
				} else if (c == '*') {
					l->read = LEXER_READ_MUL;
					t->type = TOKEN_MUL;
				} else if (c == '+') {
					l->read = LEXER_READ_PLUS;
					t->type = TOKEN_PLUS;
				} else if (c == '-') {
					l->read = LEXER_READ_MINUS;
					t->type = TOKEN_MINUS;
				} else if (c == '%') {
					l->read = LEXER_READ_MOD;
					t->type = TOKEN_MOD;
				} else if (c == '.') {
					l->read = LEXER_READ_DOT;
					t->type = TOKEN_DOT;
				} else if (c == ':') {
					l->read = LEXER_READ_COLON;
					t->type = TOKEN_COLON;
				} else if (c == ';') {
					l->read = LEXER_READ_SEMICOLON;
					t->type = TOKEN_SEMICOLON;
				} else if (c == '>') {
					l->read = LEXER_READ_MORE;
					t->type = TOKEN_MORE;
				} else if (c == '<') {
					l->read = LEXER_READ_LESS;
					t->type = TOKEN_LESS;
				} else if (c == '=') {
					l->read = LEXER_READ_EQ;
					t->type = TOKEN_EQ;
				} else if (is_digit(c)) {
					l->read = LEXER_READ_INT;	
					t->type = TOKEN_INT;
				} else if (is_alpha(c)) {
					l->read = LEXER_READ_WORD;
					t->type = TOKEN_WORD;
				} else {
					puts("INVALID CHAR\n");
					goto L_NULL;
				}			
				break;
			case LEXER_READ_COLON:
			case LEXER_READ_SEMICOLON:
			case LEXER_READ_EQ:
			case LEXER_READ_LESS:
			case LEXER_READ_MORE:
			case LEXER_READ_DIV:
			case LEXER_READ_MUL:
			case LEXER_READ_MOD:
			case LEXER_READ_DOT:
				if (is_whitespace(c) || c == '\0') {
					goto L_OP;
				} else {
					goto L_NULL;
				}
			case LEXER_READ_INT:
				if (is_whitespace(c) || c == '\0') {
					goto L_INT;
				} else if (is_digit(c)) {
					break;
				} else {
					goto L_NULL;
				}
			case LEXER_READ_WORD:
				if (is_whitespace(c) || c == '\0') {
					goto L_WORD;
				} else if (is_alphanum(c)) {
					break;
				} else {
					goto L_NULL;
				}
			case LEXER_READ_PLUS:
			case LEXER_READ_MINUS:
				if (is_whitespace(c) || c == '\0') {
					goto L_OP;
				} else if (is_digit(c)) {
					t->type = TOKEN_INT;
					l->read = LEXER_READ_INT;
					break;
				} else {
					goto L_NULL;
				}
			default: 
				puts("INVALID LEXER_READ\n");
				goto L_NULL;
			}
			break;
		default:
			puts("INVALID LEXER_STATE\n");
			goto L_NULL;
		}
	}	

L_OP:
	return t;

L_WORD:
	t->value = (char*)malloc((i+1) * sizeof(char));
	for (int j = 0; j < i; j++) {
		t->value[j] = buff[b_shift+j];
	}
	t->value[i] = '\0';
	t->value_len = i;
	if (str_cmp(t->value, "SWAP")) {
		t->type = TOKEN_SWAP;
	} else if (str_cmp(t->value, "DROP")) {
		t->type = TOKEN_DROP;
	} else if (str_cmp(t->value, "DUP")) {
		t->type = TOKEN_DUP;
	} else if (str_cmp(t->value, "ABS")) {
		t->type = TOKEN_ABS;
	} else if (str_cmp(t->value, "CL")) {
		t->type = TOKEN_CL;
	} else if (str_cmp(t->value, "IF")) {
		t->type = TOKEN_IF;
	} else if (str_cmp(t->value, "ELSE")) {
		t->type = TOKEN_ELSE;
	} else if (str_cmp(t->value, "THEN")) {
		t->type = TOKEN_THEN;
	} else {
		t->type = TOKEN_WORD;
	}
	return t;
	
L_INT:
	t->int_value = atoi(buff+b_shift);
	return t;
	
L_NULL:
	free(t);
	return NULL;
}

void skip_spaces(char *buff, int* buff_shift) {
	while (is_whitespace(buff[*buff_shift])) {
		(*buff_shift)++;
	}
}

void skip(char *buff, int* buff_shift) {
	while (!is_whitespace(buff[*buff_shift])) {
		(*buff_shift)++;
	}
}

/* Token methods */
void print_token(Token* t) {
	switch(t->type) {
	case TOKEN_INT:
		puts("TOKEN_INT ");
		break;
	case TOKEN_DUP:
		puts("TOKEN_DUP ");
		break;
	case TOKEN_DROP:
		puts("TOKEN_DROP ");
		break;
	case TOKEN_SWAP:
		puts("TOKEN_SWAP ");
		break;
	case TOKEN_CL:
		puts("TOKEN_CL ");
		break;
	case TOKEN_ABS:
		puts("TOKEN_ABS ");
		break;
	case TOKEN_PLUS:
		puts("TOKEN_PLUS ");
		break;
	case TOKEN_MINUS:
		puts("TOKEN_MINUS ");
		break;
	case TOKEN_MUL:
		puts("TOKEN_MUL ");
		break;
	case TOKEN_DIV:
		puts("TOKEN_DIV ");
		break;
	case TOKEN_MOD:
		puts("TOKEN_MOD ");
		break;
	case TOKEN_DOT:
		puts("TOKEN_DOT ");
		break;
	case TOKEN_WORD:
		puts("TOKEN_WORD ");
		break;
	default:
		puts("unrecognized token ");
	}
	puts(t->value);
}

void print_token_value(Token* t) {
	switch(t->type) {
	case TOKEN_INT:
		printf("%d ", t->int_value);
		break;
	default:
		puts(t->value);
	}
}

Token* new_token() {
	return (Token*) malloc(sizeof(Token));
}

Token* copy_token(Token* t) {
	if (t == NULL) return NULL;
	Token* t2 = new_token();
	t2->type = t->type;
	switch(t->type) {
	case TOKEN_INT:
		t2->int_value = t->int_value;
		break;
	case TOKEN_DUP:
	case TOKEN_DROP:
	case TOKEN_SWAP:
	case TOKEN_CL:
	case TOKEN_ABS:
	case TOKEN_IF:
	case TOKEN_ELSE:
	case TOKEN_THEN:
	case TOKEN_PLUS:
	case TOKEN_MINUS:
	case TOKEN_MUL:
	case TOKEN_DIV:
	case TOKEN_MOD:
	case TOKEN_DOT:
	case TOKEN_EQ:
	case TOKEN_COLON:
	case TOKEN_SEMICOLON:
	case TOKEN_WORD:
		t2->value = (char*)malloc((t->value_len+1) * sizeof(char));
		memcpy(t2->value, t->value, t->value_len);
		t2->value_len = t->value_len;
		break;
	default:
		break;
	}
	return t2;
}

void delete_token(Token* t) {
	if (t == NULL) return;
	switch(t->type) {
	case TOKEN_INT:
		break;
	case TOKEN_DUP:
	case TOKEN_DROP:
	case TOKEN_SWAP:
	case TOKEN_CL:
	case TOKEN_ABS:
	case TOKEN_IF:
	case TOKEN_ELSE:
	case TOKEN_THEN:
	case TOKEN_PLUS:
	case TOKEN_MINUS:
	case TOKEN_MUL:
	case TOKEN_DIV:
	case TOKEN_MOD:
	case TOKEN_DOT:
	case TOKEN_EQ:
	case TOKEN_COLON:
	case TOKEN_SEMICOLON:
	case TOKEN_WORD:
		free(t->value);
		break;
	default:
		break;
	}
	free(t);
}

/* Extra */
char is_digit(char c) {
	return (c >= 48) && (c <= 57);
}

char is_lower(char c) {
	return (c >= 97) && (c <= 122);
}

char is_alpha(char c) {
	return ((c >= 97) && (c <= 122)) || ((c >= 65) && (c <= 90));
}

char is_alphanum(char c) {
	return is_alpha(c) || is_digit(c);
}

char to_upper(char c) {
	if ((c >= 97) && (c <= 122)) return c - 32;
	return c;
}

char str_cmp(const char *s1, const char *s2) {
	char c1, c2;
	int i = 0;
	do {
		c1 = s1[i];
		c2 = s2[i];
		if (c1 != c2) return 0;
		i++;
	} while (c2 != '\0');
	return c1 == '\0';
}


char is_whitespace(char c) {
	return (c == ' ') || (c == '\t') || (c == '\n');
}

int atoi(const char* str) {
	int c = 1;
	if (str[0] == '-') {
		c = -1;
		str++;
	} else if (str[0] == '+') {
		str++;
	}
	int k = 0;
	while (is_digit(*str)) {
		k = 10 * k + (*str) - '0';
		str++;
	}
	return c * k;
}

void* memcpy(void* destptr, const void* srcptr, size_t num) {
	for (size_t i = 0; i < num; i++) {
		((char*)destptr)[i] = ((char*)srcptr)[i];
	}
	return destptr;
}

int abs(int x) {
	if (x < 0) return -x;
	return x;
}
