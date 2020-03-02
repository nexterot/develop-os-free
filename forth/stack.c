/*
 *  Stack.
 */
 
#include "stack.h"

/* Stack methods */
void init_stack(Stack* s, int n) {
	s->data = (Token**) malloc(n * sizeof(Token*));
	s->cap = n;
	s->top = 0;
}

char stack_empty(Stack *s) {
	return s->top == 0;
}

void stack_push(Stack *s, Token *t) {
	s->data[s->top] = t;
	s->top++;
}

Token* stack_pop(Stack *s) {
	s->top--;
	return s->data[s->top];
}

Token* stack_top(Stack *s) {
	return s->data[s->top-1];
}

void _dup(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_top(st);
	t2 = copy_token(t1);
	stack_push(st, t2);	
}

void _cl(Stack *st) {
	clear_screen();
}

void _dot(Stack *st) {
	Token *t1;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	print_token_value(t1);
	putchar(' ');
	delete_token(t1);
}

void _dot_s(Stack *s) {
	for (int i = 0; i < s->top; i++) {
		printf("%s ", s->data[i]->value);
	}
}

void _drop(Stack *st) {
	Token *t1;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	delete_token(t1);
}

void _swap(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (stack_empty(st)) {
		delete_token(t1);
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	stack_push(st, t1);
	stack_push(st, t2);
}

void _forget(Stack *st) {
	
}

void _abs(Stack *st) {
	Token *t1;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		delete_token(t1);
		puts("error: can't apply to non-int\n");
		return;
	}
	int v = abs(atoi(t1->value));
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	free(t1->value);
	free(buff);
	t1->value = buff2;
	t1->value_len = len + 1;
	stack_push(st, t1);
}

void _sum(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 += v1;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);
}

void _sub(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 -= v1;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);	
}

void _mul(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 *= v1;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);	
}

void _div(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 /= v1;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);	
}

void _mod(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 %= v1;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);	
}

void _eq(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 = (v2 == v1) ? 1 : 0;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);		
}

void _more(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 = (v2 > v1) ? 1 : 0;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);		
}

void _less(Stack *st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	if (! is_int(t2->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v1 = atoi(t1->value);
	int v2 = atoi(t2->value);
	v2 = (v2 < v1) ? 1 : 0;
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v2, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	delete_token(t1);
	free(t2->value);
	free(buff);
	t2->value = buff2;
	t2->value_len = len + 1;
	stack_push(st, t2);		
}
