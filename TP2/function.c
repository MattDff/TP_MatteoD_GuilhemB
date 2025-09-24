#include <stddef.h>
#include <stdlib.h>

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

#define BLOCK_SIZE sizeof(Block)

static Block* freeList = NULL;
void* my_malloc(size_t size) {
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
