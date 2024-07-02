# BLG 335E Analysis of Algorithms I - Fall 2023 Project 2

## Project Overview
This project involves implementing several heap-related algorithms in C++ without using any external libraries. The tasks include:

1. **MAX-HEAPIFY Procedure**
2. **BUILD-MAX-HEAP Procedure**
3. **HEAPSORT Procedure**
4. **Priority Queue Operations**
5. **d-ary Heap Operations**

## Implementation Details

### MAX-HEAPIFY Procedure 
- Ensures the max-heap property is maintained.
- Runs in O(log n) time.
- Well-documented code with comments explaining each step.

### BUILD-MAX-HEAP Procedure 
- Produces a max-heap from an unordered array.
- Runs in linear time.
- Includes detailed explanations for the logic behind each step.

### HEAPSORT Procedure 
- Utilizes MAX-HEAPIFY and BUILD-MAX-HEAP to sort an array in place.
- Detailed step-by-step explanation provided.
- Analysis of time complexity and any optimizations made.

### Priority Queue Operations 
Implemented procedures:
- `MAX-HEAP-INSERT`: Inserts a new element into the max-heap.
- `HEAP-EXTRACT-MAX`: Removes and returns the maximum element.
- `HEAP-INCREASE-KEY`: Increases the key of a specified element.
- `HEAP-MAXIMUM`: Returns the maximum element without removing it.

Includes usage examples in real-world scenarios and clear explanations for each procedure.

### d-ary Heap Operations [20 points]
Implemented functions:
1. **Height Calculation**
   - Function: `dary_calculate_height`
   - Takes the number of elements `n` and degree `d`.
   - Returns the height of the d-ary heap.

2. **EXTRACT-MAX Implementation**
   - Function: `dary_extract_max`
   - Performs the EXTRACT-MAX operation in a d-ary max-heap.
   - Analysis of running time in terms of `d` and `n`.

3. **INSERT Implementation**
   - Function: `dary_insert_element`
   - Performs the INSERT operation in a d-ary max-heap.
   - Analysis of running time in terms of `d` and `n`.

4. **INCREASE-KEY Implementation**
   - Function: `dary_increase_key`
   - Performs the INCREASE-KEY operation.
   - Analysis of running time in terms of `d` and `n`.

## DataSet
The dataset includes information about cities and their populations:
- `City` [String]
- `Population` [Integer]

Files provided: `population1.csv`, `population2.csv`, `population3.csv`, `population4.csv`.

## Deliverables

C++ code file titled `Heapsort.cpp` implementing the specified procedures. The code accept the following command line arguments:

1. The name of the DataSet file.
2. The name of the function (one of the 12 implemented functions).
3. The name of the output file.
4. Optional arguments `d`, `i`, and `k` for specific functions.

#### Example Commands
```sh
./Heapsort population2.csv heapsort out.csv
./Heapsort population3.csv heap_increase_key out.csv i435 k2749
./Heapsort population1.csv dary_calculate_height out.csv d5
