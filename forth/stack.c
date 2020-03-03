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

/* RetStack methods */

void init_ret_stack(RetStack* s, int n) {
	s->data = (int*) malloc(n * sizeof(int));
	s->cap = n;
	s->top = 0;
}


char rstack_empty(RetStack *s) {
	return s->top == 0;
}

void rstack_push(RetStack *s, int x) {
	s->data[s->top] = x;
	s->top++;
}

int rstack_pop(RetStack *s) {
	s->top--;
	return s->data[s->top];
}

int rstack_top(RetStack *s) {
	return s->data[s->top-1];
}
/* Forth methods */

void _dup(Stack *st, RetStack* ret_st) {
	Token *t1, *t2;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_top(st);
	t2 = copy_token(t1);
	stack_push(st, t2);	
}

void _2dup(Stack *st, RetStack* ret_st) {
	Token *t1, *t2, *t3, *t4;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	t3 = copy_token(t1);
	t4 = copy_token(t2);
	stack_push(st, t2);	
	stack_push(st, t1);	
	stack_push(st, t4);	
	stack_push(st, t3);	
}

void _over(Stack *st, RetStack* ret_st) {
	Token *t1, *t2, *t3;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t2 = stack_pop(st);
	t3 = copy_token(t2);
	stack_push(st, t3);
	stack_push(st, t1);		
	stack_push(st, t2);	
}

void _rot(Stack *st, RetStack* ret_st) {
	Token *t1;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	_swap(st, ret_st);
	stack_push(st, t1);	
	_swap(st, ret_st);
}

void _cl(Stack *st, RetStack* ret_st) {
	clear_screen();
}

void _dot(Stack *st, RetStack* ret_st) {
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

void _dot_s(Stack *s, RetStack* ret_st) {
	for (int i = 0; i < s->top; i++) {
		printf("%s ", s->data[i]->value);
	}
}

void _drop(Stack *st, RetStack* ret_st) {
	Token *t1;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	delete_token(t1);
}

void _swap(Stack *st, RetStack* ret_st) {
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

void _forget(Stack *st, RetStack* ret_st) {
	
}

void _abs(Stack *st, RetStack* ret_st) {
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

void _sum(Stack *st, RetStack* ret_st) {
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

void _sub(Stack *st, RetStack* ret_st) {
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

void _mul(Stack *st, RetStack* ret_st) {
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

void _div(Stack *st, RetStack* ret_st) {
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

void _mod(Stack *st, RetStack* ret_st) {
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

void _eq(Stack *st, RetStack* ret_st) {
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

void _more(Stack *st, RetStack* ret_st) {
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

void _less(Stack *st, RetStack* ret_st) {
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

void _ret_to_st(Stack *st, RetStack* ret_st) {
	Token *t1;
	if (rstack_empty(ret_st)) {
		puts("error: ret stack underflow\n");
		return;
	}
	int v = rstack_pop(ret_st);
	t1 = new_token();
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	free(buff);
	t1->value = buff2;
	t1->value_len = len + 1;
	stack_push(st, t1);
}

void _st_to_ret(Stack *st, RetStack* ret_st) {
	Token *t1;
	if (stack_empty(st)) {
		puts("error: stack underflow\n");
		return;
	}
	t1 = stack_pop(st);
	if (! is_int(t1->value)) {
		puts("error: can't apply to non-int\n");
		return;
	}
	int v = atoi(t1->value);
	rstack_push(ret_st, v);
	delete_token(t1);
}

void _ret_copy_st(Stack *st, RetStack* ret_st) {
	Token *t1;
	if (rstack_empty(ret_st)) {
		puts("error: ret stack underflow\n");
		return;
	}
	int v = rstack_top(ret_st);
	t1 = new_token();
	char* buff = (char*) malloc(256 * sizeof(char));
	int len = itoa(v, buff, 10); 
	char* buff2 = (char*) malloc((len+1) * sizeof(char));
	memcpy(buff2, buff, len+1);
	free(buff);
	t1->value = buff2;
	t1->value_len = len + 1;
	stack_push(st, t1);
}

void _if(Stack *st, RetStack* ret_st) {

}

void _else(Stack *st, RetStack* ret_st) {
	
}

void _then(Stack *st, RetStack* ret_st) {
	
}
