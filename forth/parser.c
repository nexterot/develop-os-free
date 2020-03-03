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

void parse(Parser* p, Stack* st, RetStack* ret_st, Dict* dic, Token* t) {
	DictElem *d, *new_d;
	switch(p->state) {
	// COMPILE STATE
	case PARSER_STATE_COMPILE:
		switch(p->rule) {
		case PARSER_RULE_START:
			printf("new word %s, len %d\n", t->value, t->value_len);
			new_d = new_dict_elem(t->value, t->value_len);
			add_word(dic, new_d);
			p->rule = PARSER_RULE_BODY;
			break;
		case PARSER_RULE_BODY:
			if (str_cmp(t->value, ";")) {
				printf("end word %s\n", dic->top->name);
				p->state = PARSER_STATE_EXECUTE;
			} else {
				printf("word %s: add token %s\n", dic->top->name, t->value);
				dict_elem_add_token(dic->top, t);
			}
			break;
		default:
			puts("unknown PARSER_RULE\n");
			return;
		}
		break;
	// EXECUTE STATE
	case PARSER_STATE_EXECUTE:
		d = find_word(dic, t->value);
		if (d != NULL) {
			execute_word(st, ret_st, dic, d);
		} else if (is_int(t->value)) {
			stack_push(st, t);
		} else if (str_cmp(t->value, ":")) {
			p->state = PARSER_STATE_COMPILE;
			p->rule = PARSER_RULE_START;
		} else {
			puts("invalid token\n");
			return;
		}
		break;
	default:
		puts("unknown PARSER_STATE\n");
		return;
	}
}
