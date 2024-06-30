# Computer Organization Project 1 Report

## Overview

This repository contains the report and associated files for the Computer Organization Project 1 undertaken by students of Istanbul Technical University, Computer Engineering Department, under the course BLG 222E. The project involved the implementation of a primitive computer using Verilog hardware description language.

## Project Details

- **Course**: BLG 222E - Computer Organization
- **CRN**: 21334 - 21336
- **Lecturer**: Gokhan Ince
- **Group Members**:
  - 150200060: Omer Yıldız
  - 150190052: Utku Kadir Somer
- **Semester**: Spring 2024

## Contents

1. [Introduction](#introduction)
2. [Materials and Methods](#materials-and-methods)
    - Task Distribution
3. [Part 1: Designing a 16-bit register](#part-1-designing-a-16-bit-register)
4. [Part 2: Designing a register file](#part-2-designing-a-register-file)
    - Part 2-a: Designing a 16-bit IR register
    - Part 2-b: Designing a register system
    - Part 2-c: Designing an address register file (ARF)
5. [Part 3: Arithmetic Logic Unit (ALU)](#part-3-arithmetic-logic-unit-alu)
6. [Part 4: ALU System](#part-4-alu-system)
7. [Results](#results)
8. [Discussion](#discussion)
9. [Conclusion](#conclusion)

## Introduction

In this project, we have implemented (or simulated) a primitive computer using Verilog hardware description language. We are able to perform logical and arithmetic operations within this system. Additionally, we have the option to perform these operations sequentially and store the outcomes in a memory or register. The system is tested with provided test inputs.

## Materials and Methods

### Task Distribution

All parts of the project were implemented in collaboration. The debugging and reporting parts were also done as a group.

## Part 1: Designing a 16-bit Register

For the first part, we designed and implemented a 16-bit register that is utilized later in the implementation of the Register File and the Address Register File (ARF). The generated 16-bit register has 8 functions when enabled: clear, load, decrement, increment, clear and write low, only write low, only write high, sign extension, and write low.

## Part 2: Designing a Register File

### Part 2-a: Designing a 16-bit IR Register

In this section, we designed a 16-bit Instruction Register (IR) which is crucial for holding the instruction currently being executed.

### Part 2-b: Designing a Register System

This part involved designing a register system that includes multiple registers for holding different data and addresses necessary for the CPU operations.

### Part 2-c: Designing an Address Register File (ARF)

The Address Register File (ARF) was designed to manage addresses for different operations, ensuring proper data flow and storage management.

## Part 3: Arithmetic Logic Unit (ALU)

We designed an Arithmetic Logic Unit (ALU) capable of performing a variety of arithmetic and logic operations. This unit also includes a 4-bit output for status flags (zero, negative, carry, and overflow), handling operations using 2's complement logic.

## Part 4: ALU System

This part involved integrating the ALU into a cohesive system operating under a single clock, ensuring correct data flow and operation as per control signals. The integration tested the design's modularity and the robustness of individual components within a unified system.

## Results

The results section details the outcomes of our tests and simulations, showcasing the functional correctness of our design.

## Discussion

This section discusses the challenges encountered during the design and implementation phases, how we addressed them, and the iterative testing and refinement process to ensure compliance with the specified requirements.

## Conclusion

In conclusion, the project provided a deeper understanding of computer organization, emphasizing modular design, simulation testing, and system integration. We faced some challenges, especially with the ALU part and testbenches, but we overcame them through learning and collaboration.

