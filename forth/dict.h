/*
 *  Dict.
 */
#ifndef _DICT_H
#define _DICT_H
 
#include "lexer.h"

#define DATA_STACK_SIZE 1024*1024*64

/* Dict */
typedef struct dict_elem {
	char* name;
	Token** tokens;
	int tokens_len;
	struct dict_elem* next;
} DictElem;

typedef struct dict {
	DictElem* start;
} Dict;

/* Dict methods */
DictElem* find_word(Dict* dic, const char* name);
DictElem* new_dict_elem(const char* name, int name_len);
void dict_elem_add_token(DictElem* d, Token* t);
void delete_dict_elem(DictElem* d);
void add_word(Dict* dic, DictElem* d);

#endif
