/*
 *  Lexer.
 */
 
#include "lexer.h"

/* Lexer methods */
Lexer* new_lexer() {
	Lexer* l = (Lexer*) malloc(sizeof(Lexer));	
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
		//printf("char: %c\n", c);
		if (is_whitespace(c) || c == '\0') {
			break;
		}
	}	
	if (i == 0) {
		return NULL;
	}
	t->value = (char*)malloc((i+1) * sizeof(char));
	for (int j = 0; j < i; j++) {
		t->value[j] = buff[b_shift+j];
	}
	t->value[i] = '\0';
	t->value_len = i;
	return t;
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
void print_token_value(Token* t) {
	puts(t->value);
}

Token* new_token() {
	return (Token*) malloc(sizeof(Token));
}

Token* copy_token(Token* t) {
	Token* t2 = new_token();
	t2->value = (char*)malloc((t->value_len+1) * sizeof(char));
	memcpy(t2->value, t->value, t->value_len);
	t2->value[t->value_len] = '\0';
	t2->value_len = t->value_len;
	return t2;
}

void delete_token(Token* t) {
	if (t == NULL) return;
	free(t->value);
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

int itoa(int value, char *sp, int radix) {
    char tmp[256]; // be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;
    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;
    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }
    int len = tp - tmp;
    if (sign) 
    {
        *sp++ = '-';
        len++;
    }
    while (tp > tmp)
        *sp++ = *--tp;
    *sp = '\0';
    return len;
}

char is_int(const char* str) {
	if (str[0] == '-' || str[0] == '+') {
		str++;
	}
	while (*str != '\0') {
		if (! is_digit(*str)) return 0;
		str++;
	}
	return 1;
}

void* memcpy(void* destptr, const void* srcptr, int num) {
	char *d = destptr;
	const char *s = srcptr;
	for (int i = 0; i < num; i++) {
		d[i] = s[i];
	}
	return destptr;
}

int abs(int x) {
	if (x < 0) return -x;
	return x;
}
