#ifndef _MEMORY_H
#define _MEMORY_H
#include "multiboot.h"
#include "types.h"

void mem_init(multiboot_info_t* mbd);
void* malloc(size_t size);
void free(void* ptr, unsigned int size);

void mem_debug(); // for debug only; prints sectors of free memory

typedef struct free_zone {
	char F;
	char R;
	char E;
	char E_;
	struct free_zone* prev_zone;
	struct free_zone* next_zone;
	unsigned int size;
} free_zone_t;

#endif
