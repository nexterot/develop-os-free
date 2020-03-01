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

void execute_word(Stack* st, Dict* dic, DictElem* d) {
	char skip_to_else_or_then = 0;
	int count_ifs = 0;
	//printf("executing word %s, len=%d\n", d->name, d->tokens_len);
	for (int i = 0; i < d->tokens_len; i++) {
		//printf("skip: %d, count: %d\n", skip_to_else_or_then, count_ifs);
		Token* t = copy_token(d->tokens[i]);
		/*
		puts("executing token: ");
		print_token(t);
		puts("\n");
		*/
		Token *t1, *t2;
		DictElem *d;
		if (count_ifs > 0) {
			if (t->type == TOKEN_IF) {
				count_ifs++;
			} else if (t->type == TOKEN_THEN) {
				count_ifs--;
			}
			continue;
		}
		if (skip_to_else_or_then) {
			if (t->type == TOKEN_IF) {
				count_ifs++;
			}
			if ((t->type != TOKEN_ELSE) && (t->type != TOKEN_THEN)) {
				continue;
			}
		}
		switch(t->type) {
		case TOKEN_INT:
			stack_push(st, t);
			break;
		case TOKEN_DUP:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_top(st);
			t2 = copy_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_DROP:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			delete_token(t1);
			break;
		case TOKEN_SWAP:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			stack_push(st, t1);
			stack_push(st, t2);
			break;
		case TOKEN_CL:
			clear_screen();
			break;
		case TOKEN_ABS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			t1->int_value = abs(t1->int_value);
			stack_push(st, t1);
			break;
		case TOKEN_PLUS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value += t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_MINUS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value -= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_MUL:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value *= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_DIV:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value /= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_MOD:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value %= t1->int_value;
			delete_token(t1);
			stack_push(st, t2);
			break;
		case TOKEN_DOT:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			print_token_value(t1);
			delete_token(t1);
			break;
		case TOKEN_WORD:
			d = find_word(dic, t->value);
			if (d == NULL) {
				puts("error: unknown word\n");
				return;
			}
			execute_word(st, dic, d);
			break;
		case TOKEN_IF:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (! (t1->int_value)) {
				skip_to_else_or_then = 1;
			}
			delete_token(t1);
			break;
		case TOKEN_ELSE:
			skip_to_else_or_then = !skip_to_else_or_then;
			break;
		case TOKEN_THEN:
			if (skip_to_else_or_then) {
				skip_to_else_or_then = 0;
			}
			break;
		case TOKEN_EQ:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value = (t2->int_value == t1->int_value) ? 1 : 0;
			stack_push(st, t2);
			delete_token(t1);
			break;
		case TOKEN_MORE:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value = (t2->int_value > t1->int_value) ? 1 : 0;
			stack_push(st, t2);
			delete_token(t1);
			break;
		case TOKEN_LESS:
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t1 = stack_pop(st);
			if (stack_empty(st)) goto L_STACK_UNDERFLOW;
			t2 = stack_pop(st);
			t2->int_value = (t2->int_value < t1->int_value) ? 1 : 0;
			stack_push(st, t2);
			delete_token(t1);
			break;
		case TOKEN_COLON:
		case TOKEN_SEMICOLON:
			puts("error: compilation inside a word\n");
			return;
		default:
			puts("error: unrecognized token\n");
			print_token(t);
			putchar('\n');
			return;
		}
	}
	return; 
			
L_STACK_UNDERFLOW:
	puts("error: stack underflow\n");
	return;
}
