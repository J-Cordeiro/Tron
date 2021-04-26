// THIS DOCUMENT CONTAINS: 
// The utilitary functions used in the code

#include "utils.h"

#include <stdio.h>

// Verify if pointer was allocated
static void tron_verify_pointer(void *pointer) {
    if (!pointer) {
        printf("Error memory: Could not allocate memory.");
        exit(1);
    }
}

// Allocate specific memory size to a pointer 
void *tron_malloc(size_t size) {
    void *pointer = malloc(size);
    tron_verify_pointer(pointer);
    return pointer;
}

// Diallocate a pointer memory
void *tron_calloc(size_t number_of_members, size_t size) {
    void *pointer = calloc(number_of_members, size);
    tron_verify_pointer(pointer);
    return pointer;
}
