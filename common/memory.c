#include "memory.h"

static void* mem_base = NULL;
static void* mem_end = NULL;

void mem_init(multiboot_info_t* mbd) {
	memory_map_t* mmap = mbd->mmap_addr;
	memory_map_t* max_ram = mmap;
	while (mmap < mbd->mmap_addr + mbd->mmap_length) {
		if (mmap->type == 1 && (mmap->length_high > max_ram->length_high || ((mmap->length_high == max_ram->length_high) && (mmap->length_low > max_ram->length_low)))) {
			max_ram = mmap;
		}
		mmap = (memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
	}
	mem_base = max_ram->base_addr_low + 0x100000;
	mem_end = mem_base + max_ram->length_low;
}

void* malloc(size_t size) {
	if (mem_base == NULL || mem_end == NULL || size <= 0) {
		return NULL;
	}
	if (mem_base + size >= mem_end) {
		return NULL;
	}
	void* copy = mem_base;
	mem_base += size;
	return copy;
}


