#ifndef HEAP_ALLOCATOR_H
#define HEAPA_LLOCATOR_H

#include <stddef.h>
#include <stdint.h>

#define		HEAP_SIZE	(128*1024)
#define		MIN_HEAP_SIZE	1024


//Memory block representation structure
typedef struct memory_block
{
    size_t size;
    int mem_flag;
    struct memory_block *next_link;
}Block;

Block *find_best_fit( uint8_t size );
void *malloc_fun( size_t size );
void split_block(Block *block, size_t size);
void my_free( void *ptr );
void print_heap_status();
#endif
