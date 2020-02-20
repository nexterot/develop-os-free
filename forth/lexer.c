/*
 *  Lexer.
 */
 
#include "lexer.h"

/* Lexer methods */
void next_token(Lexer* l, Token* t) {
	char buff[256];
	int i;
	char only_digits = 1;
	for (i = 0; i < 256; i++) {
		char c = getchar();
		if (c == '\n' || c == ' ') {
			break;
		}
		if (!is_digit(c)) only_digits = 0;
		buff[i] = c;
		putchar(c);
	}
	putchar('\n');
	t->value = (char*)malloc(i * sizeof(char));
	for (int j = 0; j < i; j++) {
		t->value[j] = buff[j];
	}
	t->value_len = i;
	if (only_digits) {
		t->type = INT;
	} else if ((t->value_len == 1) && (t->value[0] == '+')) {
		t->type = OP_PLUS;
	} else if ((t->value_len == 1) && (t->value[0] == '-')) {
		t->type = OP_MINUS;
	} else if ((t->value_len == 1) && (t->value[0] == '*')) {
		t->type = OP_MUL;
	} else if ((t->value_len == 1) && (t->value[0] == '/')) {
		t->type = OP_DIV;	
	} else if ((t->value_len == 1) && (t->value[0] == '%')) {
		t->type = OP_MOD;	
	} else {
		t->type = WORD;
	}
}

/* Token methods */
void print_token(Token* t) {
	switch(t->type) {
	case INT:
		puts("INT ");
		break;
	case WORD:
		puts("WORD ");
		break;
	case OP_PLUS:
		puts("OP_PLUS ");
		break;
	case OP_MINUS:
		puts("OP_MINUS ");
		break;
	case OP_MUL:
		puts("OP_MUL ");
		break;
	case OP_DIV:
		puts("OP_DIV ");
		break;
	case OP_MOD:
		puts("OP_MOD ");
		break;
	default:
		puts("UNKNOWN ");
	}
	for (int i = 0; i < t->value_len; i++) {
		putchar(t->value[i]);
	}
	putchar('\n');
}

void delete_token(Token* t) {
	free(t->value);
}


/* Extra */
char is_digit(char c) {
	return (c >= 48) && (c <= 57);
}
