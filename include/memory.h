/*
 * Contains funcitons and types to work with memory.
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#include "multiboot.h"
#include "types.h"
#include "printf.h"

/*
 * A block of free memory represented as a node in double-linked list.
 * Consists of at least 16 bytes:
 * - 'FREE' character watermark (bytes 0-3)
 * - pointer to previous node   (bytes 4-7)
 * - pointer to next node       (bytes 8-11)
 * - size of this node, min=16  (bytes 12-15)
 */
typedef struct free_zone {
    char F, R, E, E_;
    struct free_zone* prev_zone;
    struct free_zone* next_zone;
    unsigned int size;
} free_zone_t;


void mem_init(multiboot_info_t* mbd); /* should be called from main */
void* malloc(size_t size);
void free(void* ptr);

void mem_debug(); /* prints sectors of free memory */

#endif
