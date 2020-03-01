/*
 *  Dict.
 */
 
#include "dict.h"

/* Dict methods */
#define DICT_SIZE 32

DictElem* find_word(Dict* dic, const char* name) {
	//puts("find_word called\n");
	DictElem* next_d = dic->start;
	while (next_d != NULL) {
		//printf("find_word: %s\n", next_d->name);
		if (str_cmp(next_d->name, name)) {
			return next_d;
		}
		next_d = next_d->next;
	}
	return NULL;
}


DictElem* new_dict_elem(const char* name, int name_len) {
	DictElem* d = (DictElem*) malloc(sizeof(DictElem));
	d->tokens = (Token**) malloc(DICT_SIZE * sizeof(Token*));
	d->tokens_len = 0;
	d->name = (char*) malloc((name_len+1) * sizeof(char));
	memcpy(d->name, name, name_len);
	d->name[name_len] = '\0';
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
	if (dic->start == NULL) {
		dic->start = d;
		//puts("add_word: first\n");
		for (int i = 0; i < d->tokens_len; i++) {
			Token* t = d->tokens[i];
			/*
			puts("add word token: ");
			print_token(t);
			puts(" ");
			print_token_value(t);
			puts("\n");
			*/
		}
		return;
	}
	DictElem* next_d = dic->start;
	while (next_d->next != NULL) {
		next_d = next_d->next;
	}
	next_d->next = d;
	//puts("add_word: not first\n");
}
