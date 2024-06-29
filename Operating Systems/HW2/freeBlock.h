#ifndef FREEBLOCK
#define FREEBLOCK

#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>

// Define the free block structure
typedef struct FreeBlock {
    size_t size;
    struct FreeBlock *next;
    int free ;
} FreeBlock;

#endif