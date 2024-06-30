# Basic Shell Program

This repository contains the implementation of a basic shell program in the C programming language for the Linux operating system, created for a project in the Computer Operating Systems course (BLG 312E).

## Table of Contents

- [Introduction](#introduction)
- [Program Structure](#program-structure)
- [Key Functions](#key-functions)
- [Command Execution](#command-execution)
- [Command History and Batch File Execution](#command-history-and-batch-file-execution)
- [Conclusion](#conclusion)
- [Installation](#installation)
- [Usage](#usage)
- [Contact](#contact)

## Introduction

This report presents the design and implementation of a basic shell program in the C programming language for the Linux operating system. A shell is a command-line interpreter that allows users to interact with the operating system by executing commands. The implemented shell provides functionalities such as executing commands, handling pipes, semicolons, built-in commands, command history, and batch file execution.

## Program Structure

The program is structured as follows:

- **Header Files**: The necessary header files such as `<stdio.h>`, `<stdlib.h>`, `<unistd.h>`, etc., are included.
- **Global Constants**: Constants like `MAXCOM`, `MAXLIST`, `HISTORY_MAX_SIZE`, and `MAX_LINE_LENGTH` are defined.
- **Global Variables**: Variables like `history` and `history_count` are used for command history storage.
- **Function Prototypes**: Prototypes for functions like `init_shell()`, `takeInput()`, `printDir()`, `execArgs()`, `execArgsPiped()`, etc., are declared.
- **Main Function**: The `main()` function handles user input, command processing, and execution.

## Key Functions

- `init_shell()`: Initializes the shell environment and displays a greeting message.
- `takeInput()`: Reads user input from the command line and adds it to history.
- `printDir()`: Prints the current working directory.
- `execArgs()`: Executes a single command entered by the user.
- `execArgsPiped()`: Handles piped commands by creating a pipe and executing commands in separate processes.
- `execArgsSemicolon()`: Handles commands separated by semicolons by executing them in separate processes.
- `ownCmdHandler()`: Handles built-in commands like `quit`, `cd`, `help`, and `history`.
- `parsePipe()` and `parseSemiCol()`: Functions to parse piped and semicolon-separated commands, respectively.
- `parseSpace()`: Parses command words based on spaces.

## Command Execution

The program reads user input or commands from a batch file and processes them based on whether they are single commands, piped commands, or commands separated by semicolons. Built-in commands are handled separately, and commands are executed in child processes.

## Command History and Batch File Execution

The shell maintains a command history and supports batch file execution by reading commands from a specified file and executing them.

## Conclusion

The implemented shell provides basic functionalities of a command-line interpreter and demonstrates handling of commands, pipes, semicolons, built-in commands, command history, and batch file execution.

## Installation

Instructions to set up the project locally:

```bash
# Clone the repository
git clone https://github.com/yourusername/BasicShell.git

# Navigate to the project directory
cd BasicShell

# Compile the code
gcc -o shell shell.c
```
## Usage
Provide examples and explanations of how to use the project:
```bash
# Run the compiled program
./shell
```
## Contact
For any questions or suggestions, please contact Ömer YILDIZ at yildizom20@itu.edu.tr.


