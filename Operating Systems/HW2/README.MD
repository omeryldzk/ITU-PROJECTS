# MyMalloc Library

This repository contains the implementation of a custom memory allocation library called MyMalloc, created for a project in the Computer Operating Systems course (BLG 312E).

## Table of Contents

- [Introduction](#introduction)
- [Code Overview](#code-overview)
- [Memory Allocation Strategies](#memory-allocation-strategies)
- [Code Execution](#code-execution)
- [Conclusion](#conclusion)
- [Installation](#installation)
- [Usage](#usage)
- [Contact](#contact)

## Introduction

This project involves the implementation of a custom memory allocation library, MyMalloc, which includes functions for initializing the memory heap, allocating memory blocks, freeing memory, and managing various memory allocation strategies.

## Code Overview

1. **Header Files and Global Variables**: The code includes necessary header files for system calls and defines global variables for tracking memory allocation and heap initialization status.

2. **Memory Structures**: Two main structures are defined - `FreeBlock` for managing free memory blocks and `AllocatedMemory` to track allocated memory pointers.

3. **Initialization (`InitMyMalloc`)**: This function initializes the memory heap using `mmap` and sets up the initial free block.

4. **Memory Allocation (`MyMalloc`)**: Allocates memory based on different strategies (BestFit, WorstFit, FirstFit, NextFit) and manages free blocks accordingly.

5. **Memory Freeing (`myFree`)**: Frees memory allocated by MyMalloc and coalesces free memory spaces to optimize memory management.

6. **Memory Layout Dumping (`DumpFreeList`)**: Prints the memory layout including both allocated and free blocks for debugging purposes.

7. **Main Function**: Demonstrates the usage of MyMalloc by allocating memory for processes, freeing memory, and printing memory layout after each step.

## Memory Allocation Strategies

- **BestFit**: Finds the smallest suitable free block that fits the requested size.
- **WorstFit**: Finds the largest suitable free block to minimize fragmentation.
- **FirstFit**: Allocates memory in the first available free block that meets the size criteria.
- **NextFit**: Similar to FirstFit but starts searching for a fit from the last allocated block for efficiency.

## Code Execution

The code demonstrates the initialization of MyMalloc with a heap size of 4096 bytes. It allocates memory for four processes (P1, P2, P3, P4) based on the chosen allocation strategy. The code also includes options for contiguous or separate memory blocks for each process. After each allocation and deallocation, the memory layout is printed using `DumpFreeList` to visualize the status of allocated and free memory blocks.

## Conclusion

The provided code showcases a basic implementation of a memory allocation library in C, highlighting different allocation strategies and memory management techniques. For standard dynamic memory allocation within a program, `malloc` is recommended. For more control over memory attributes or specialized memory management, `mmap` is preferred due to its flexibility and system-level control capabilities.

## Installation

Instructions to set up the project locally:

```bash
# Clone the repository
git clone https://github.com/yourusername/MyMalloc.git

# Navigate to the project directory
cd MyMalloc

# Compile the code
gcc -o mymalloc mymalloc.c
```
## Usage
Provide examples and explanations of how to use the project:
```bash
# Run the compiled program
./mymalloc
```
## Contact
For any questions or suggestions, please contact Ömer YILDIZ at yildizom20@itu.edu.tr.

