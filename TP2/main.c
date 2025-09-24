#include <stdio.h>
#include <string.h>

#include "function.c"

int main() {
    void* ptr1 = malloc_3is(100);
    void* ptr2 = malloc_3is(200);
    void* ptr3 = malloc_3is(50);

    printf("Successfully allocated : %p, %p, %p\n", ptr1, ptr2, ptr3);

    free_3is(ptr2);
    printf("Block freed : %p\n", ptr2);

    return 0;
}