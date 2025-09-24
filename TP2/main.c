#include <stdio.h>
#include "function.c"

int main() {
    // Test des allocations
    void* ptr1 = malloc_3is(100);
    void* ptr2 = malloc_3is(200);
    void* ptr3 = malloc_3is(50);

    printf("Allocations réussies : %p, %p, %p\n", ptr1, ptr2, ptr3);

    free_3is(ptr2);
    printf("Bloc libéré : %p\n", ptr2);

    strcpy((char*)ptr1, "Dépassement de mémoire");
    free_3is(ptr1);

    return 0;
}