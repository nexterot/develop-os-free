/*
 * Contains funcitons and types to work with memory.
 */

#include "memory.h"

#define MIN_BLOCK 16    /* minimum size of allocated memory */

static void* mem_base = NULL;   /* points to last free block address */
static void* mem_end = NULL;    /* points to the end of memory range */
static free_zone_t* first_zone = NULL;  /* points to first free block */

/*
 * Creates a watermark in freed block of size 'sz', pointed to by 'ptr'.
 */
static void watermark(free_zone_t* ptr, unsigned int sz) {
    ptr->F = 'F';
    ptr->R = 'R';
    ptr->E = 'E';
    ptr->E_ = 'E';
    ptr->prev_zone = NULL;
    ptr->next_zone = NULL;
    ptr->size = sz;
    //printf("w %p %lu\n", ptr, ptr->size);
}

/*
 * Initializes memory. Must be called before first use of malloc.
 * Gets the biggest memory range from Multiboot struct 'mbd'
 * filled by GRUB.
 */
void mem_init(multiboot_info_t* mbd) {
    memory_map_t* mmap = mbd->mmap_addr;
    memory_map_t* max_ram = mmap;
    while (mmap < mbd->mmap_addr + mbd->mmap_length) {
        if (mmap->type == 1 && (mmap->length_high > max_ram->length_high || ((mmap->length_high == max_ram->length_high) && (mmap->length_low > max_ram->length_low)))) {
            max_ram = mmap;
        }
        //printf("in %d %p %p %lu\n", mmap->type, mmap->base_addr_low, mmap->base_addr_low + mmap->length_low, mmap->length_low);
        mmap = (memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
    }
    mem_base = max_ram->base_addr_low + 0x100000;
    mem_end = max_ram->base_addr_low + max_ram->length_low;
    //printf("i %p %p\n", mem_base, mem_end);
    first_zone = mem_base;
    watermark(mem_base, (unsigned int)mem_end - (unsigned int)mem_base);
}

/*
 * Inserts a newly freed block at address 'new_zone' into double-linked
 * list of free blocks.
 */
static void insert_into_list(free_zone_t* new_zone) {
    free_zone_t* z = first_zone;
    /* check if no free blocks */
    if (z == NULL) {
        first_zone = new_zone;
        return;
    }
    /* check if new zone has to be inserted at start */
    if (new_zone < z) {
        z->prev_zone = new_zone;
        new_zone->next_zone = z;
        first_zone = new_zone;
        return;
    }
    /* else find a place to insert zone */
    while (z->next_zone != NULL) {
        if ((new_zone > z) && (new_zone < (z->next_zone))) {
            //printf("insert: %p %p %p\n", z, new_zone, z->next_zone);
            z->next_zone->prev_zone = new_zone;
            new_zone->prev_zone = z;
            new_zone->next_zone = z->next_zone;
            z->next_zone = new_zone;
            //printf("insert: %p %p %p\n", z, new_zone, new_zone->next_zone);
            return;
        }
        z = z->next_zone;
    }
    /* insert in the end of the list */
    z->next_zone = new_zone;
    new_zone->prev_zone = z;
}

/*
 * Deletes free block 'zone' from the list.
 */
static void delete_from_list(free_zone_t* zone) {
    if (zone == NULL) {
        return;
    }
    
    /* to ruin watermark */
    zone->F  = 'U';  
    zone->R  = 'S';
    zone->E  = 'E';
    zone->E_ = 'D';
    
    if (zone->prev_zone != NULL) {
        zone->prev_zone->next_zone = zone->next_zone;
    }
    if (zone->next_zone != NULL) {
        zone->next_zone->prev_zone = zone->prev_zone;
    }
    if (zone == first_zone) {
        first_zone = zone->next_zone;
    }
}

/*
 * Splits a free block 'zone' into two blocks for malloc. 
 * The first part of size='split_size' will be allocated later.
 * Pointer to the second part is returned.
 */
static free_zone_t* split_zone(free_zone_t* zone, unsigned int split_size) {
    free_zone_t* new_zone = ((void*)zone) +  split_size;
    watermark(new_zone, zone->size - split_size);
    delete_from_list(zone);
    insert_into_list(new_zone);
    return new_zone;
}

/*
 * Finds a free block of size <= 'size' from the start of the list.
 * Returns pointer to this block if found, else NULL. 
 * If block found is longer than requested, it is split into two blocks.
 */
static void* find_in_list(unsigned int size) {
    free_zone_t* z = first_zone;
    while (z != NULL) {
        if (z->size == size) {
            delete_from_list(z);
            /*
            char* c = (char*)first_zone;
            free_zone_t* zz = first_zone;
            while (zz != NULL) {
                printf("find: eq %p %p %c%c%c%c %p %p %u\n", zz, ((void*)zz)+zz->size, c[0], c[1], c[2], c[3], zz->prev_zone, zz->next_zone, zz->size);
                zz = zz->next_zone;
            }
            */
            return z;
        } else if (z->size > size) {
            split_zone(z, size);
            /*
            char* c = (char*)first_zone;
            free_zone_t* zz = first_zone;
            while (zz != NULL) {
                //printf("find: less %p %p %c%c%c%c %p %p %u\n", zz, ((void*)zz)+zz->size, c[0], c[1], c[2], c[3], zz->prev_zone, zz->next_zone, zz->size);
                zz = zz->next_zone;
            }
            */
            return z;
        }
        z = z->next_zone;
    }
    return NULL;
}

/*
 * Cycles through the list and merges two zones if they are contiguous.
 */
static void merge_contiguous_zones() {
    free_zone_t* z = first_zone;
    while ((z != NULL) && (z->next_zone != NULL)) {
        if ((((void*)z) + (z->size)) == (z->next_zone)) {
            free_zone_t* deleted_zone = z->next_zone;
            delete_from_list(deleted_zone);
            z->size += deleted_zone->size;
            if (mem_base == (void*)deleted_zone) {
                mem_base = z;
            }
            //printf("merge: z=%p deleted block at %p+%u\n", z, deleted_zone, deleted_zone->size);
            continue;
        } 
        z = z->next_zone;
    }
}

/*
 * Allocates N >='size' bytes and returns pointer to them. 
 * N is the first integer after 'size' divided by MIN_BLOCK.
 */
void* malloc(size_t size) {
    if (mem_base == NULL || mem_end == NULL || size <= 0) {
        return NULL;
    }
    /* increment size to divide 16 */
    unsigned int r = size % MIN_BLOCK;
    if (r > 0) {
        size += (MIN_BLOCK - r);
    }
    /*
    char* c = (char*)first_zone;
    free_zone_t* z = first_zone;
    while (z != NULL) {
        //printf("malloc: before %u %p; %p %p %c%c%c%c %p %p %u\n", size, mem_base, z, ((void*)z)+(z->size), c[0], c[1], c[2], c[3], z->prev_zone, z->next_zone, z->size);
        z = z->next_zone;
    }
    */
    /* try to find memory in the list */
    if (mem_base + size >= mem_end) {
        //printf("malloc: finding in list\n");
        return find_in_list(size);
    }
    /* for sufficient first allocations */
    void* copy = mem_base;
    mem_base += size;
    delete_from_list((free_zone_t*)copy);   
    /* don't create watermarks if all memory used */
    if (mem_base + MIN_BLOCK <= mem_end) {
        watermark((free_zone_t*)mem_base, (unsigned int)mem_end - (unsigned int)mem_base);
        insert_into_list((free_zone_t*)mem_base);
    }
    /*
    c = (char*)first_zone;
    z = first_zone;
    while (z != NULL) {
        printf("malloc: after %p %p %c%c%c%c %p %p %u\n", z, ((void*)z)+z->size, c[0], c[1], c[2], c[3], z->prev_zone, z->next_zone, z->size);
        z = z->next_zone;
    }
    */
    return copy;
}

/*
 * Frees allocated memory at 'ptr' of size 'size'. 
 * Size argument is needed for the algorithm.
 * CAREFUL: calling on unallocated block gets undefined behavior.
 */
void free(void* ptr, unsigned int size) {
    /* increment size to divide 16 */
    unsigned int r = size % MIN_BLOCK;
    if (r > 0) {
        size += (MIN_BLOCK - r);
    }
    /*
    char* c = (char*)first_zone;
    free_zone_t* z = first_zone;
    while (z != NULL) {
        printf("free: before %p %p %c%c%c%c %p %p %u\n", z, ((void*)z)+z->size, c[0], c[1], c[2], c[3], z->prev_zone, z->next_zone, z->size);
        z = z->next_zone;
    }   
    */
    free_zone_t* zone_ptr = (free_zone_t*)ptr;
    watermark(zone_ptr, size);
    insert_into_list(zone_ptr);
    /*
    c = (char*)first_zone;
    z = first_zone;
    while (z != NULL) {
        //printf("free: between %p %p %c%c%c%c %p %p %u\n", z, ((void*)z)+z->size, c[0], c[1], c[2], c[3], z->prev_zone, z->next_zone, z->size);
        z = z->next_zone;
    }
    */
    merge_contiguous_zones();
    /*
    c = (char*)first_zone;
    z = first_zone;
    while (z != NULL) {
        printf("free: after %p %p %c%c%c%c %p %p %u\n", z, ((void*)z)+z->size, c[0], c[1], c[2], c[3], z->prev_zone, z->next_zone, z->size);
        z = z->next_zone;
    }
    */
}

/*
 * Debug free blocks of memory.
 */
void mem_debug() {
    char* c = (char*)first_zone;
    free_zone_t* z = first_zone;
    while (z != NULL) {
        printf("dbg: %p %p %c%c%c%c %p %p %u\n", z, ((void*)z)+z->size, c[0], c[1], c[2], c[3], z->prev_zone, z->next_zone, z->size);
        z = z->next_zone;
    }   
}

