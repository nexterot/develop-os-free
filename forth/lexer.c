/*
 *  Lexer.
 */
 
#include "lexer.h"

/* Lexer methods */
void next_token(Lexer* l, Token* t) {
	char buff[256];
	int i;
	char only_digits = 1;
	char only_alphas = 1;
	for (i = 0; i < 256; i++) {
		char c = getchar();
		if (c == '\n' || c == ' ') {
			break;
		}
		putchar(c);
		if (!is_digit(c)) only_digits = 0;
		if (!is_alpha(c)) only_alphas = 0;
		if (is_lower(c)) c = to_upper(c);
		buff[i] = c;
	}
	putchar('\n');
	t->value = (char*)malloc((i+1) * sizeof(char));
	for (int j = 0; j < i; j++) {
		t->value[j] = buff[j];
	}
	t->value[i] = '\0';
	t->value_len = i;
	if (only_digits) {
		t->type = INT;
	} else if (str_cmp(t->value, "DUP")) {
		t->type = WORD_DUP;
	} else if (str_cmp(t->value, "DROP")) {
		t->type = WORD_DROP;
	} else if (str_cmp(t->value, "SWAP")) {
		t->type = WORD_SWAP;
	} else if (str_cmp(t->value, "+")) {
		t->type = OP_PLUS;
	} else if (str_cmp(t->value, "-")) {
		t->type = OP_MINUS;
	} else if (str_cmp(t->value, "*")) {
		t->type = OP_MUL;
	} else if (str_cmp(t->value, "/")) {
		t->type = OP_DIV;	
	} else if (str_cmp(t->value, "%")) {
		t->type = OP_MOD;	
	} else if (str_cmp(t->value, ".")) {
		t->type = OP_LAST;		
	} else if (only_alphas) {
		t->type = WORD;	
	} else {
		t->type = INVALID;
	}
}

/* Token methods */
void print_token(Token* t) {
	switch(t->type) {
	case INT:
		puts("INT ");
		break;
	case WORD_DUP:
		puts("WORD_DUP ");
		break;
	case WORD_DROP:
		puts("WORD_DROP ");
		break;
	case WORD_SWAP:
		puts("WORD_SWAP ");
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
	case OP_LAST:
		puts("OP_LAST ");
		break;
	case WORD:
		puts("WORD ");
		break;
	default:
		puts("INVALID ");
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

char is_lower(char c) {
	return (c >= 97) && (c <= 122);
}

char is_alpha(char c) {
	return ((c >= 97) && (c <= 122)) || ((c >= 65) && (c <= 90));
}

char to_upper(char c) {
	return c - 32;
}

char str_cmp(char *s1, const char *s2) {
	char c1, c2;
	int i = 0;
	do {
		c1 = s1[i];
		c2 = s2[i];
		if (c1 != c2) return 0;
		i++;
	} while (c2 != '\0');
	return 1;
}
