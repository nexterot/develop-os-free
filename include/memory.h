#ifndef _MEMORY_H
#define _MEMORY_H
#include "multiboot.h"
#include "types.h"

void mem_init(multiboot_info_t* mbd);
void* malloc(size_t size);

#endif
