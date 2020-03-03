/*
 *  Dict.
 */
#ifndef _DICT_H
#define _DICT_H
 
#include "lexer.h"
#include "stack.h"

#define DICT_SIZE 32

/* Dict */
typedef struct dict_elem {
	char* name;
	struct dict_elem* prev;
	Token** tokens;
	int tokens_len;
	void (*func_ptr)(Stack*, RetStack*);
	char is_immediate;
} DictElem;

typedef struct dict {
	DictElem* top;
} Dict;

/* Dict methods */
void init_dict(Dict* dic);
DictElem* find_word(Dict* dic, const char* name);
DictElem* new_dict_elem(const char* name, int name_len);
void dict_elem_add_token(DictElem* d, Token* t);
void delete_dict_elem(DictElem* d);
void add_word(Dict* dic, DictElem* d);
void forget_word(Dict* dic, const char* name);
void execute_word(Stack* st, RetStack* ret_st, Dict* dic, DictElem* d);

#endif
