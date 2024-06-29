#ifndef MALLOC
#define MALLOC

#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>

#include "/home/ubuntu/hostvolume/ox/freeBlock.h"


#define PAGE_SIZE getpagesize()

# define MAP_FIXED 0x10
# ifdef __MAP_ANONYMOUS 
# define MAP_ANONYMOUS __MAP_ANONYMOUS
# else
# define MAP_ANONYMOUS 0x20
# endif
# define MAP_ANON MAP_ANONYMOUS
# define INT_MAX
#define MAX_ALLOCATED_POINTERS 100
# define BESTFIT 0
# define WORSTFIT 1
# define FIRSTFIT 2
# define NEXTFIT 3


// Structure to track allocated memory blocks
typedef struct {
    void *ptr;
    int size; // Size of allocated memory block
} AllocatedMemory;
#endif