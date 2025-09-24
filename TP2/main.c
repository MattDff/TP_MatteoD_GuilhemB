#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAGIC_NUMBER 0x0123456789ABCDEFL

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre */
    size_t bloc_size; /* taille du memory bloc en octets*/
    long magic_number; /* 0x0123456789ABCDEFL */
} HEADER;

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

#define BLOCK_SIZE sizeof(Block)

static Block* freeList = NULL;

void * malloc_3is(size_t size) {
    size += BLOCK_SIZE;
    if (!freeList) {
        freeList = (Block*)sbrk(size);
        if (freeList == (void*) -1) {
            return NULL;
        }
        freeList->size = size;
        freeList->next = NULL;
        return (void*)(freeList + 1);
    }
    Block* current = freeList;
    while (current) {
        if (current->size >= size) {
            (void*)(current + 1);
        }
        current = current->next;
    }
    Block* newBlock = (Block*)sbrk(size);
    if (newBlock == (void*) -1) {
        return NULL;
    } newBlock->size = size;
    newBlock->next = NULL;
    return (void*)(newBlock + 1);
}

/*
void * malloc_3is(size_t size) {
    size_t total_size = sizeof(HEADER) + size + sizeof(long);
    HEADER *block = sbrk(total_size);
    if (block == (void*)-1) return NULL; 

    block->bloc_size = size;
    block->magic_number = MAGIC_NUMBER;
    block->ptr_next = NULL;

    long *end_magic = (long*)((char*)block + sizeof(HEADER) + size);
    *end_magic = MAGIC_NUMBER;

    return (void*)((char*)block + sizeof(HEADER));
}
*/

void free_3is(void * pointer){
    return;
}

// ---------------- Main for the tests ----------------------------

int main(){
    return 0;
}