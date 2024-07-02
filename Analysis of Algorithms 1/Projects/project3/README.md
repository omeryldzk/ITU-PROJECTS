# Analysis of Algorithms I - Project 3

## Introduction
This project is part of the BLG335E Analysis of Algorithms I course, Fall 2023, supervised by Res. Asst. Erhan Biçer. The goal is to implement Binary Search Tree (BST) and Red-Black Tree (RBT) algorithms to store and manage population data of cities.

## Project Structure
The project consists of implementing the following functionalities:
- **TREE-INSERT:** Insert nodes into the BST and RBT.
- **TREE-DELETE:** Delete nodes from the BST and RBT.
- **TREE-SEARCH:** Search for nodes in the BST and RBT.
- **Sorting - Walking Functions:** Retrieve and return nodes in ascending order.
- **Other Fundamental Operations:** Find minimum, maximum, successor, and predecessor nodes.

## Files
- `redblacktree.cpp`: Implementation of the Red-Black Tree.
- `bst.cpp`: Implementation of the Binary Search Tree.
- `main.cpp`: Provided main function to test the implementations.
- `log.txt`: Log file generated after running the code.
- `rb_out.csv`: CSV file containing the ordered population data of cities for the RBT.
- `bst_out.csv`: CSV file containing the ordered population data of cities for the BST.

## Compilation and Execution
To compile and run the project:
```sh
g++ main.cpp -o main
./main <DATASET_FILE_NAME>.csv out.csv v
```
