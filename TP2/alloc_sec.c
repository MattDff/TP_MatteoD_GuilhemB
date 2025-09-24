#include <alloc_sec.h>;

HEADER * free_list = NULL;



void * malloc_3is(size_t size){

    HEADER *currentent = free_list;

    while (currentent != NULL) {
        if (currentent->bloc_size < size){
            
        }
        currentent = currentent->ptr_next;
    }

    return;
}

void free_3is(void * pointer){
    return;
}