#include "utils.h"

static void *tron_verify_pointer(void *pointer)
{
    if (!pointer)
    {
        printf("Error memory: Could not allocate memory.");
        exit(1);
    }
}

void *tron_malloc(size_t size)
{
    void *pointer = malloc(size);
    verify_pointer(pointer);
    return pointer;
}

void *tron_calloc(size_t number_of_members, size_t size)
{
    void *pointer = calloc(number_of_members, size);
    verify_pointer(pointer);
    return pointer;
}
