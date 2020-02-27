/*
 *  Parser.
 */
 
#include "parser.h"

/* Parser methods */
Parser* new_parser() {
	Parser* p = (Parser*) malloc(sizeof(Parser));	
	p->state = PARSER_STATE_EXECUTE;
	p->rule = PARSER_RULE_START;
	return p;
}

void parse(Parser* p, Stack* st, Token** tokens, int tokens_num) {
	p->tokens = tokens;
	p->tokens_num = tokens_num;
	p->tokens_pos = 0;
	while (has_next(p)) {
		Token* t = next(p), *t1, *t2;
		switch(p->state) {
			
		case PARSER_STATE_COMPILE:
			switch(p->rule) {
			case PARSER_RULE_START:
				switch(t->type) {
				case TOKEN_CL:
					clear_screen();
					break;
				case TOKEN_WORD:
					p->rule = PARSER_RULE_BODY;
					break;
				default:
					puts("syntax error: expecting word name\n");
					return;
				}
				break;
			case PARSER_RULE_BODY:
				switch(t->type) {
				case TOKEN_INT:
				case TOKEN_DUP:
				case TOKEN_DROP:
				case TOKEN_SWAP:
				case TOKEN_CL:
				case TOKEN_ABS:
				case TOKEN_PLUS:
				case TOKEN_MINUS:
				case TOKEN_MUL:
				case TOKEN_DIV:
				case TOKEN_MOD:
				case TOKEN_DOT:
				case TOKEN_WORD:
				case TOKEN_IF:
				case TOKEN_ELSE:
				case TOKEN_THEN:
				case TOKEN_EQ:
				case TOKEN_MORE:
				case TOKEN_LESS:
					break;
				case TOKEN_COLON:
					puts("syntax error: ambiguous colon\n");
					return;
				case TOKEN_SEMICOLON:
					p->state = PARSER_STATE_EXECUTE;
					break;
				default:
					puts("error: unrecognized token\n");
					print_token(t);
					putchar('\n');
					return;
				}
				break;
			default:
				puts("unknown PARSER_RULE\n");
				return;
			}
			break;
			
		case PARSER_STATE_EXECUTE:
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
				return;
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
				break;
			case TOKEN_IF:
			case TOKEN_ELSE:
			case TOKEN_THEN:
				puts("error: only in compilation state\n");
				return;
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
				p->state = PARSER_STATE_COMPILE;
				p->rule = PARSER_RULE_START;
				break;
			case TOKEN_SEMICOLON:
				puts("error: already in execution state\n");
				return;
			default:
				puts("error: unrecognized token\n");
				print_token(t);
				putchar('\n');
				return;
			}
			break;
			
		default:
			puts("INVALID PARSER_STATE\n");
			return;
		}
	}
	if (tokens_num > 0) {
		puts("OK\n");
	}
	return;
	
L_STACK_UNDERFLOW:
	puts("error: stack underflow\n");
	return;
}

Token* next(Parser* p) {
	return p->tokens[p->tokens_pos++];
}

char has_next(Parser* p) {
	return p->tokens_pos < p->tokens_num;
}
