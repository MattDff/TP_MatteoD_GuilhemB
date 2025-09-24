#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#define MAGIC_NUMBER 0x0123456789ABCDEFL

typedef struct HEADER_TAG {
    struct HEADER_TAG* ptr_next; /* Pointeur vers le prochain bloc libre */
    size_t bloc_size;             /* Taille du bloc mémoire en octets */
    long magic_number;            /* 0x0123456789ABCDEFL */
} HEADER;

static HEADER* free_list = NULL; 

void* malloc_3is(size_t size) {
    if (size == 0) return NULL;

    HEADER* current = free_list;
    HEADER* previous = NULL;


    while (current != NULL) {
        if (current->bloc_size >= size + sizeof(HEADER)) {
            if (current->bloc_size > size + sizeof(HEADER) + sizeof(HEADER)) {
                HEADER* new_block = (HEADER*)((char*)current + sizeof(HEADER) + size);
                new_block->bloc_size = current->bloc_size - size - sizeof(HEADER);
                new_block->ptr_next = current->ptr_next;
                new_block->magic_number = MAGIC_NUMBER;

                current->ptr_next = new_block;
                current->bloc_size = size;
            } else {
                if (previous) {
                    previous->ptr_next = current->ptr_next;
                } else {
                    free_list = current->ptr_next;
                }
            }
            current->magic_number = MAGIC_NUMBER;
            return (void*)(current + 1);
        }
        previous = current;
        current = current->ptr_next;
    }

    current = (HEADER*)sbrk(size + sizeof(HEADER));
    if (current == (void*)-1) return NULL;

    current->ptr_next = NULL;
    current->bloc_size = size;
    current->magic_number = MAGIC_NUMBER;

    return (void*)(current + 1);
}

void free_3is(void* ptr) {
    HEADER* block = (HEADER*)ptr - 1; 
    if (block->magic_number != MAGIC_NUMBER) {
        printf("Error : Not enough space !\n");
        return;
    }

    block->ptr_next = free_list;
    free_list = block;

    HEADER* current = free_list;
    while (current && current->ptr_next) {
        if ((char*)current + sizeof(HEADER) + current->bloc_size == (char*)current->ptr_next) {
            current->bloc_size += sizeof(HEADER) + current->ptr_next->bloc_size;
            current->ptr_next = current->ptr_next->ptr_next;
        } else {
            current = current->ptr_next;
        }
    }
}