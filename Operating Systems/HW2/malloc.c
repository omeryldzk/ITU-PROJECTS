#include "/home/ubuntu/hostvolume/ox/malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  // Include sys/types.h for pid_t
#include <sys/wait.h>
#include <inttypes.h> // Include inttypes.h for platform-independent format specifiers

// Global variable to track the last allocated block
static FreeBlock* lastAllocatedBlock = NULL;

// Global variables to track initialization status and heap start address
static FreeBlock* heapStart = NULL;
static int isInitialized = 0;
// New structure to track allocated memory blocks
static AllocatedMemory allocatedMemoryList[MAX_ALLOCATED_POINTERS];
static int allocatedPointerCount = 0;

// Function to add allocated memory pointer to the list
void addAllocatedMemory(void *ptr, int size) {
    if (allocatedPointerCount < MAX_ALLOCATED_POINTERS) {
        allocatedMemoryList[allocatedPointerCount].ptr = ptr;
        allocatedMemoryList[allocatedPointerCount].size = size;
        allocatedPointerCount++;
    } else {
        fprintf(stderr, "Maximum allocated memory pointers reached.\n");
        exit(1);
    }
}

// Function to remove allocated memory pointer from the list
void removeAllocatedMemory(void *ptr) {
    int i;
    for (i = 0; i < allocatedPointerCount; i++) {
        if (allocatedMemoryList[i].ptr == ptr) {
            // Shift elements to fill the gap
            for (int j = i; j < allocatedPointerCount - 1; j++) {
                allocatedMemoryList[j] = allocatedMemoryList[j + 1];
            }
            allocatedPointerCount--;
            break;
        }
    }
}

int InitMyMalloc(int HeapSize) {
    if (isInitialized || HeapSize <= 0) {
        return -1; // Already initialized or invalid input
    }

    // Round up HeapSize to the nearest multiple of PAGE_SIZE
    int roundedSize = ((HeapSize + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;

    // Allocate memory for the heap
    void *heap = mmap(NULL, roundedSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (heap == MAP_FAILED) {
        return -1; // mmap failed
    }

    // Initialize the header section
    FreeBlock *header = (FreeBlock *)heap;
    header->size = roundedSize - sizeof(FreeBlock);
    header->next = NULL;
    
    // Update global variables
    heapStart = header;
    isInitialized = 1;

    return 0; // Success
}
// Function to allocate memory based on strategy
void *MyMalloc(int size, int strategy) {
    if (!isInitialized || size <= 0) {
        return NULL; // Heap not initialized or invalid size
    }

    FreeBlock *currentBlock = heapStart;
    FreeBlock *bestFitBlock = NULL;
    int bestFitSize = 30000000;
    int worstFitSize = 0; // Initialize to maximum possible value
    int remainingSize = size + sizeof(FreeBlock); // Size including FreeBlock header
// For the Next Fit
    if(strategy == 3 && lastAllocatedBlock)
            currentBlock = lastAllocatedBlock;

    // Traverse the free list to find a suitable block based on strategy
    while (currentBlock != NULL) {
        if (currentBlock->size >= remainingSize || strategy == 3) {
            if (strategy == 0) { // Bestfit
                if (currentBlock->size <= bestFitSize) {
                    bestFitSize = currentBlock->size;
                    bestFitBlock = currentBlock;
                }
            } else if (strategy == 1) { // Worstfit
                if (currentBlock->size > worstFitSize) {
                    worstFitSize = currentBlock->size;
                    bestFitBlock = currentBlock;
                }
            } else if (strategy == 2) { // Firstfit
                    bestFitBlock = currentBlock;
                    break; // Exit loop on first fit
            } else if (strategy == 3) { // Nextfit
                    bestFitSize = currentBlock->size;
                    bestFitBlock = currentBlock;
                }

            }
        currentBlock = currentBlock->next;
    }

    lastAllocatedBlock = bestFitBlock;

    if (bestFitBlock == NULL) {
        return NULL; // No suitable block found
    }

    // Split the block if there is enough space
    if (bestFitBlock->size - remainingSize >= 0) {
        FreeBlock *newBlock = (FreeBlock *)((char *)bestFitBlock + remainingSize);
        newBlock->size = bestFitBlock->size - remainingSize;
        newBlock->next = bestFitBlock->next;
        bestFitBlock->size = remainingSize;
        bestFitBlock->next = newBlock;
    }

    // Remove the allocated block from the free list
    if (bestFitBlock == heapStart) {
        heapStart = bestFitBlock->next;
    } else {
        FreeBlock *prevBlock = (FreeBlock *)heapStart;
        while (prevBlock->next != bestFitBlock) {
            prevBlock = prevBlock->next;
        }
        prevBlock->next = bestFitBlock->next;
    }
    void *allocatedPtr = (void *)((char *)bestFitBlock + sizeof(FreeBlock)); //  Allocation result

    if (allocatedPtr != NULL) {
        addAllocatedMemory(allocatedPtr,size);
    }

    return allocatedPtr;


}

// Function to free memory allocated by MyMalloc and coalesce free memory spaces
int myFree(void *ptr) {
    if (!isInitialized || ptr == NULL) {
        return 0; // Heap not initialized or NULL pointer provided
    }

    // Calculate the pointer to the FreeBlock structure
    FreeBlock *blockToFree = (FreeBlock *)((char *)ptr - sizeof(FreeBlock));

    // Add the block back to the free list
    FreeBlock *currentBlock = heapStart;
    FreeBlock *prevBlock = NULL;

    // Find the appropriate position to insert the freed block
    while (currentBlock != NULL && currentBlock < blockToFree) {
        prevBlock = currentBlock;
        currentBlock = currentBlock->next;
    }

    // Insert the freed block back into the free list
    if (prevBlock == NULL) {
        // Insert at the beginning of the list
        blockToFree->next = heapStart;
        heapStart = blockToFree;
    } else {
        // Insert in between or at the end of the list
        prevBlock->next = blockToFree;
        blockToFree->next = currentBlock;
    }

    // Coalesce free memory spaces if adjacent blocks are free
    FreeBlock *nextBlock = blockToFree->next;
    if (nextBlock != NULL && (char *)blockToFree + blockToFree->size == (char *)nextBlock) {
        // Merge blockToFree with nextBlock
        blockToFree->size +=  nextBlock->size;
        blockToFree->next = nextBlock->next;
    }

    // Coalesce previous free space if applicable
    if (prevBlock != NULL && (char *)prevBlock + prevBlock->size  == (char *)blockToFree) {
        // Merge prevBlock with blockToFree
        prevBlock->size += blockToFree->size;
        prevBlock->next = blockToFree->next;
    }
    removeAllocatedMemory(ptr);
    return 1; // Successfully freed the memory block
}
// Function to print the available memory spaces in the heap with full addresses
// Function to print the memory layout (both allocated and free blocks)
   // Function to print the available memory spaces in the heap with addresses

// Function to print the memory layout (both allocated and free blocks)
void DumpFreeList() {
    FreeBlock *currentBlock = heapStart;

    printf("\nDumping Memory Layout:\n");
    printf("========================================================================\n");
    printf("|   Address   |    Size   |    Status    |         Description          |\n");
    printf("========================================================================\n");

    // Print free blocks
    while (currentBlock != NULL ) {
        printf("| %10p | %8d | %12s | %28s |\n", currentBlock, currentBlock->size, "Free", "Free Block");
        currentBlock = currentBlock->next;
    }

    // Print allocated blocks
    for (int i = 0; i < allocatedPointerCount; i++) {
        printf("| %10p | %8d | %12s | %28s |\n", allocatedMemoryList[i].ptr, allocatedMemoryList[i].size, "Allocated", "Allocated Memory");
    }

    printf("========================================================================\n");
}


int main() {
    int strategy;
    printf("Please enter the strategy type (0 for BestFit, 1 for WorstFit, 2 for FirstFit, 3 for NextFit): ");
    scanf("%d", &strategy);

    // Initialize the MyMalloc heap
    int initStatus = InitMyMalloc(4096); // Example heap size of 4096 bytes
    if (initStatus == -1) {
        printf("Error initializing MyMalloc.\n");
        exit(1);
    }
    int size=0;

    // Allocate memory for processes P1, P2, P3, P4
    void *allocatedMemoryP1 = MyMalloc(512, strategy);
    void *allocatedMemoryP2 = MyMalloc(1024, strategy);
    void *allocatedMemoryP3 = MyMalloc(16, strategy);
    void *allocatedMemoryP4 = MyMalloc(4, strategy);
    



    // Check if allocations succeeded
    if (allocatedMemoryP1 != NULL && allocatedMemoryP2 != NULL &&
        allocatedMemoryP3 != NULL && allocatedMemoryP4 != NULL) {
        printf("All allocations succeeded.\n");
    } else {
        printf("One or more allocations failed.\n");
    }
    int status;
    printf("Please enter the strategy type (0 for contigous, 1 for seperate: ");
    scanf("%d", &status);

    // Print DumpFreeList() output after allocations
    printf("\nDumpFreeList() Output after allocations:\n");
    DumpFreeList();

    // If Memory Blocks are contigous
    if (allocatedMemoryP1 != NULL && status == 0) {
        myFree(allocatedMemoryP4);
        myFree(allocatedMemoryP2);
        myFree(allocatedMemoryP3);
        // Print DumpFreeList() output after freeing memory for process P1
        printf("\nDumpFreeList() Output after freeing memory :\n");
        DumpFreeList();
    }
    if (allocatedMemoryP1 != NULL && status == 1) {
        myFree(allocatedMemoryP1);
        myFree(allocatedMemoryP3);
        printf("\nDumpFreeList() Output after freeing memory :\n");
        DumpFreeList();
    }
    // 
    void *allocatedMemoryP5 = MyMalloc(128, strategy);
    void *allocatedMemoryP6 = MyMalloc(256, strategy);
    void *allocatedMemoryP7 = MyMalloc(32, strategy);
    DumpFreeList();

    return 0;
} 