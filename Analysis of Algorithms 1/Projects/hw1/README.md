# Analysis of Algorithms: Project 1

## Overview

This repository contains the implementation and analysis of the QuickSort algorithm with different pivoting strategies and a hybrid implementation of QuickSort and Insertion Sort. The project was completed as part of the BLG 335E course at the Faculty of Computer and Informatics Engineering, Department of Computer Engineering.

## Contents

- `QuickSort.cpp`: Main implementation file for QuickSort with different pivoting strategies and the hybrid QuickSort-Insertion Sort.
- `data`: Directory containing the datasets used for testing.
- `logs`: Directory containing the log files generated during the sorting process.
- `report.pdf`: Detailed report of the project.

## Implementation Details

### 1. Naive Implementation of QuickSort

The basic QuickSort algorithm implemented without any optimization:
1. Chooses the last element as the pivot.
2. Partitions the array into two subarrays around the pivot.
3. Recursively applies QuickSort on the subarrays until reaching the base case.

### 2. QuickSort with Different Pivoting Strategies

#### Last Element
- Chooses the last element of the array as the pivot.

#### Random Element
- Chooses a random element of the array as the pivot.

#### Median of Three
- Chooses three elements at random and selects the median of these elements as the pivot.

### 3. Hybrid QuickSort and Insertion Sort

A hybrid method that uses QuickSort for arrays larger than a threshold `k` and Insertion Sort for arrays smaller than or equal to `k`.

## DataSet

The simplified DataSet contains:
- City [String]
- Population [Integer]

Multiple permutations of the same DataSet (e.g., `population1.csv`, `population2.csv`, etc.) are provided to test the code.

## Compilation and Usage

To compile the code:
```bash
g++ QuickSort.cpp -o QuickSort
```
To run the compiled executable:
```bash
./QuickSort [DataSetFileName].csv [pivot strategy] [k] [output file] [verbose]
```

## Command Line Arguments

DataSetFileName: Name of the DataSet file.
Pivot Strategy:
l: Last Element
r: Random Element
m: Median of Three
Threshold k: If k is 1, naive QuickSort runs. Otherwise, the hybrid approach with Insertion Sort runs.
Output File: Name of the output CSV file.
Verbose (optional): If v is provided, a log.txt file is generated with partition details for QuickSort.

### Example:
```bash
./QuickSort population1.csv r 5 out.csv v
```
## Output
The program prints the elapsed time for sorting to the command line:
```bash
Time taken by QuickSort with pivot strategy 'r' and threshold 5: [Elapsed Time] ns.
```

