/*
 *  Lexer.
 */
 
#include "lexer.h"

int token_err = 0;

/* Lexer methods */
Lexer* new_lexer() {
	Lexer* l = (Lexer*) malloc(sizeof(Lexer));	
	l->state = LEXER_STATE_INIT;
	return l;
}

Token* next_token(Lexer* l, char* buff, int* buff_shift) {
	int b_shift = *buff_shift;
	char only_digits = 1;
	char only_alphas = 1;
	int i;
	for (i = 0; b_shift+i < LINE_BUFFER_SIZE; i++, (*buff_shift)++) {
		char c = buff[b_shift+i];
		if (c == ' ' || c == '\t') {
			break;
		}
		if (c == '\n' || c == '\0') {
			if (i == 0) {
				return NULL;
			}
			break;
		}
		if (c == '-') {
			if (i > 0) {
				break;
			}
		}
		if (c != '-' && !is_digit(c)) only_digits = 0;
		if (!is_alpha(c)) only_alphas = 0;
		if (is_lower(c)) buff[b_shift+i] = to_upper(c);
	}
	Token* t = (Token*) malloc(sizeof(Token));
	t->value = (char*)malloc((i+1) * sizeof(char));
	for (int j = 0; j < i; j++) {
		t->value[j] = buff[b_shift+j];
	}
	t->value[i] = '\0';
	t->value_len = i;
	if (t->value_len == 0)  {
		t->type = EMPTY;
	} else if (str_cmp(t->value, "DUP")) {
		t->type = WORD_DUP;
	} else if (str_cmp(t->value, "DROP")) {
		t->type = WORD_DROP;
	} else if (str_cmp(t->value, "SWAP")) {
		t->type = WORD_SWAP;
	} else if (str_cmp(t->value, "CL")) {
		t->type = WORD_CL;
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
	} else if (only_digits) {
		t->type = INT;
		t->int_value = atoi(t->value);
	} else {
		t->type = INVALID;
	}
	return t;
}

void skip_spaces(char *buff, int* buff_shift) {
	while (is_space(buff[*buff_shift])) {
		(*buff_shift)++;
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
	case WORD_CL:
		puts("WORD_CL ");
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
	case INVALID:
		puts("INVALID ");
		break;
	case EMPTY:
		puts("EMPTY ");
		break;
	default:
		puts("unrecognized token ");
	}
	puts(t->value);
}

void print_token_value(Token* t) {
	switch(t->type) {
	case INT:
		printf("%d ", t->int_value);
		break;
	case WORD_DUP:
	case WORD_DROP:
	case WORD_SWAP:
	case WORD_CL:
	case OP_PLUS:
	case OP_MINUS:
	case OP_MUL:
	case OP_DIV:
	case OP_MOD:
	case OP_LAST:
	case WORD:
	case INVALID:
	case EMPTY:
	default:
		puts(t->value);
	}
}

Token* new_token() {
	return (Token*) malloc(sizeof(Token));
}

Token* copy_token(Token* t) {
	Token* t2 = new_token();
	t2->type = t->type;
	switch(t->type) {
	case INT:
		t2->int_value = t->int_value;
		break;
	case WORD_DUP:
	case WORD_DROP:
	case WORD_SWAP:
	case WORD_CL:
	case OP_PLUS:
	case OP_MINUS:
	case OP_MUL:
	case OP_DIV:
	case OP_MOD:
	case OP_LAST:
	case WORD:
		t2->value = (char*)malloc((t->value_len+1) * sizeof(char));
		memcpy(t2->value, t->value, t->value_len);
		t2->value_len = t->value_len;
		break;
	case INVALID:
	case EMPTY:
		break;
	default:
		break;
	}
	return t2;
}

void delete_token(Token* t) {
	switch(t->type) {
	case INT:
		break;
	case WORD_DUP:
	case WORD_DROP:
	case WORD_SWAP:
	case WORD_CL:
	case OP_PLUS:
	case OP_MINUS:
	case OP_MUL:
	case OP_DIV:
	case OP_MOD:
	case OP_LAST:
	case WORD:
		free(t->value);
		break;
	case INVALID:
	case EMPTY:
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


char is_space(char c) {
	return c == ' ' || c == '\t';
}

int atoi(const char* str) {
	int c = 1;
	if (str[0] == '-') {
		c = -1;
		str++;
	}
	int k = 0;
	while (*str) {
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
