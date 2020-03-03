/*
 *  Dict.
 */
 
#include "dict.h"

/* Dict methods */

void init_dict(Dict* dic) {
	dic->top = NULL;
	DictElem *d;
	// DUP
	d = new_dict_elem("DUP", 3);
	d->func_ptr = _dup;	
	d->is_immediate = 0;
	add_word(dic, d);
	// DROP
	d = new_dict_elem("DROP", 4);
	d->func_ptr = _drop;	
	d->is_immediate = 0;
	add_word(dic, d);
	// SWAP
	d = new_dict_elem("SWAP", 4);
	d->func_ptr = _swap;	
	d->is_immediate = 0;
	add_word(dic, d);
	// ABS
	d = new_dict_elem("ABS", 3);
	d->func_ptr = _abs;	
	d->is_immediate = 0;
	add_word(dic, d);
	// FORGET
	d = new_dict_elem("FORGET", 6);
	d->func_ptr = _forget;	
	d->is_immediate = 0;
	add_word(dic, d);
	// CL
	d = new_dict_elem("CL", 2);
	d->func_ptr = _cl;	
	d->is_immediate = 1;
	add_word(dic, d);	
	// .
	d = new_dict_elem(".", 1);
	d->func_ptr = _dot;	
	d->is_immediate = 1;
	add_word(dic, d);
	// .S
	d = new_dict_elem(".S", 2);
	d->func_ptr = _dot_s;	
	d->is_immediate = 1;
	add_word(dic, d);
	// +
	d = new_dict_elem("+", 1);
	d->func_ptr = _sum;	
	d->is_immediate = 0;
	add_word(dic, d);
	// -
	d = new_dict_elem("-", 1);
	d->func_ptr = _sub;	
	d->is_immediate = 0;
	add_word(dic, d);
	// *
	d = new_dict_elem("*", 1);
	d->func_ptr = _mul;	
	d->is_immediate = 0;
	add_word(dic, d);
	// /
	d = new_dict_elem("/", 1);
	d->func_ptr = _div;	
	d->is_immediate = 0;
	add_word(dic, d);
	// %
	d = new_dict_elem("%", 1);
	d->func_ptr = _mod;	
	d->is_immediate = 0;
	add_word(dic, d);
	// =
	d = new_dict_elem("=", 1);
	d->func_ptr = _eq;	
	d->is_immediate = 0;
	add_word(dic, d);
	// <
	d = new_dict_elem("<", 1);
	d->func_ptr = _less;	
	d->is_immediate = 0;
	add_word(dic, d);
	// >
	d = new_dict_elem(">", 1);
	d->func_ptr = _more;	
	d->is_immediate = 0;
	add_word(dic, d);
	// >R
	d = new_dict_elem(">R", 2);
	d->func_ptr = _st_to_ret;	
	d->is_immediate = 0;
	add_word(dic, d);
	// R>
	d = new_dict_elem("R>", 2);
	d->func_ptr = _ret_to_st;	
	d->is_immediate = 0;
	add_word(dic, d);
	// R@
	d = new_dict_elem("R@", 2);
	d->func_ptr = _ret_copy_st;	
	d->is_immediate = 0;
	add_word(dic, d);
	// @
	d = new_dict_elem("@", 1);
	d->func_ptr = _ret_copy_st;	
	d->is_immediate = 0;
	add_word(dic, d);
	// !
	d = new_dict_elem("!", 1);
	d->func_ptr = _ret_copy_st;	
	d->is_immediate = 0;
	add_word(dic, d);
	// CONSTANT
	d = new_dict_elem("CONSTANT", 8);
	d->func_ptr = _ret_copy_st;	
	d->is_immediate = 0;
	add_word(dic, d);
	// VARIABLE
	d = new_dict_elem("VARIABLE", 8);
	d->func_ptr = _ret_copy_st;	
	d->is_immediate = 0;
	add_word(dic, d);
	// THEN 
	d = new_dict_elem("THEN", 4);
	d->func_ptr = _then;	
	d->is_immediate = 1;
	add_word(dic, d);
	// BEGIN 
	d = new_dict_elem("BEGIN", 5);
	d->func_ptr = _then;	
	d->is_immediate = 1;
	add_word(dic, d);
}

DictElem* find_word(Dict* dic, const char* name) {
	//printf("searching for word %s\n", name);
	DictElem* prev_d = dic->top;
	while (prev_d != NULL) {
		//printf("word %s\n", prev_d->name);
		if (str_cmp(prev_d->name, name)) {
			//printf("found!\n");
			return prev_d;
		}
		prev_d = prev_d->prev;
	}
	//printf("not found!\n");
	return NULL;
}


DictElem* new_dict_elem(const char* name, int name_len) {
	DictElem* d = (DictElem*) malloc(sizeof(DictElem));
	d->tokens = (Token**) malloc(DICT_SIZE * sizeof(Token*));
	d->tokens_len = 0;
	d->name = (char*) malloc((name_len+1) * sizeof(char));
	memcpy(d->name, name, name_len);
	d->name[name_len] = '\0';
	d->func_ptr = NULL;
	return d;
}

void dict_elem_add_token(DictElem* d, Token* t) {
	d->tokens[d->tokens_len] = t;
	(d->tokens_len)++;
}

void delete_dict_elem(DictElem* d) {
	free(d->name);
	for (int i = 0; i < d->tokens_len; i++) {
		free(d->tokens[i]);
	}
	free(d->tokens);
}

void add_word(Dict* dic, DictElem* d) {
	d->prev = dic->top;
	dic->top = d;
}


void forget_word(Dict* dic, const char* name) {
	DictElem* d = dic->top;
	if (str_cmp(d->name, name)) {
		dic->top = d->prev;
		delete_dict_elem(d);
		return;
	}
	while (d->prev != NULL) {
		if (str_cmp(d->prev->name, name)) {
			d->prev = d->prev->prev;
			delete_dict_elem(d->prev);
			return;
		}
		d = d->prev;
	}
}

int find_token(DictElem* d, Token* t) {
	for (int i = 0; i < d->tokens_len; i++) {
		if (d->tokens[i] == t) {
			return i;
		}
	}
	return -1;
}

void execute_word(Stack* st, RetStack* ret_st, Dict* dic, DictElem* d) {
	if (d->func_ptr != NULL) {
		(*d->func_ptr)(st, ret_st);
		return;
	}
	Token* t;
	DictElem *d2;
	for (int i = 0; i < d->tokens_len; i++) {
		t = d->tokens[i];
		d2 = find_word(dic, t->value);
		if (d2 != NULL) {
			execute_word(st, ret_st, dic, d2);
		} else if (str_cmp(t->value, "IF")) {
			Token *t1;
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			int v = atoi(t1->value);
			delete_token(t1);
			if (!v) {
				i = find_token(d, t->jump) - 1;
				if (str_cmp(t->jump->value, "ELSE")) {
					i++;
				}
			}
		} else if (str_cmp(t->value, "ELSE")) {
			i = find_token(d, t->jump) - 1;
		} else if (str_cmp(t->value, "UNTIL")) {
			Token *t1;
			if (stack_empty(st)) {
				puts("error: stack underflow\n");
				return;
			}
			t1 = stack_pop(st);
			int v = atoi(t1->value);
			delete_token(t1);
			if (!v) {
				i = find_token(d, t->jump) - 1;
			}
		} else if (is_int(t->value)) {
			stack_push(st, copy_token(d->tokens[i]));
		} else {
			puts("invalid token\n");
			return;
		}
	}
}
