/*
--------------------------------------------------------------------------------------------------------------------
NAME: Sanjay.S
DATE:
DESCRIPTION:
SAMPLE I/P:
SAMPLE O/P:
-------------------------------------------------------------------------------------------------------------------
*/
 
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ass1_baremetal.h"

static uint8_t heap[HEAP_SIZE];

Block *memory_list = NULL;

//Initializing heap
void initialize_heap()
{
    memory_list = (Block *)heap;
    memory_list -> size = HEAP_SIZE;
    memory_list ->mem_flag = true;
    memory_list -> next_link = NULL;
}

void *malloc_fun( size_t size )
{
    if (size == 0 || size > HEAP_SIZE )
    {
	return NULL; //return Invalid size
    }

    //To ensure the minimum block size
    if (size < MIN_HEAP_SIZE)
    {
	size = MIN_HEAP_SIZE;
    }

    //Finding the best-fit block
    Block *best_fit = find_best_fit(size);
    if ( !best_fit )
    {
	return NULL;
    }

    if (best_fit -> size > size + sizeof(Block))
    {
	split_block( best_fit, size );
    }

    best_fit -> mem_flag = false;

    return (void*)((uint8_t*)best_fit + sizeof(Block));
}

Block *find_best_fit( uint8_t size )
{
    Block *best_fit = NULL;
    Block *current = memory_list;

    while(current != NULL)
    {
	if (current -> mem_flag == true && current -> size >= size )
	{
	    if ( !best_fit || current -> size < best_fit -> size )
	    {
		best_fit = current;
	    }
	}
	current = current -> next_link;
    }

    return best_fit;
}

void split_block(Block *block, size_t size)
{
    Block *new_block = (Block *)((uint8_t *)block +sizeof(Block) + size);

    new_block -> size = block -> size - size - sizeof(Block);
    new_block -> mem_flag = true;
    new_block -> next_link = block -> next_link;

    block -> size = size + sizeof(Block);
    block -> next_link = new_block;
}

void my_free( void *ptr )
{
    if ( !ptr )
    {
	return;
    }

    Block *block = (Block *)((uint8_t *)ptr - sizeof(Block));

    block->mem_flag = true;

    //merging the free blocks;
    Block *current = memory_list;

    while( current && current -> next_link )
    {
	if ( current->mem_flag && current->next_link -> mem_flag )
	{
	    current -> size += current -> next_link -> size;
	    current -> next_link = current -> next_link -> next_link;
	}
	else
	{
	    current = current -> next_link;
	}
    }

}

void print_heap_status()
{
    Block* current = (Block*)heap;
    printf("Heap Status:\n");
    while ((uint8_t*)current < heap + HEAP_SIZE) 
    {
        printf("Block at %p: Size = %zu, %s\n", (void*)current, current->size, current->mem_flag ? "Free" : "Allocated");
        current = current->next_link;
    }
}

int main()
{
    initialize_heap();
    print_heap_status();

    //Examples
    void *ptr1 = malloc_fun(sizeof(int)*128);
    printf("Allocated 1KB at %p\n", ptr1);
    print_heap_status();

    void *ptr2 = malloc_fun(sizeof(uint8_t)*1000);
    printf("Allocated 1KB at%p\n", ptr2);
    print_heap_status();

    void* ptr3 = malloc_fun(128 * 8 * 1024);
    printf("Allocated 128KB at %p\n", ptr3);
    print_heap_status();

   
    my_free(ptr1);
    printf("Freed 1KB at %p\n", ptr1);
    print_heap_status();

    my_free(ptr2);
    printf("Freed 1KB at %p\n", ptr2);
    print_heap_status();

    my_free(ptr3);
    printf("Freed 128KB at %p\n", ptr3);
    print_heap_status();

    return 0;
}
