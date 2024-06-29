// C Program to design a shell in Linux 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 

#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 100 // max number of commands to be supported 

#define HISTORY_MAX_SIZE 100 // max number of commands to be supported 
#define MAX_LINE_LENGTH 250

static char *history[HISTORY_MAX_SIZE];
static unsigned history_count = 0;

// Clearing the shell using escape sequences 
#define clear() printf("\033[H\033[J") 

// Greeting shell during startup 
void init_shell() 
{ 
	clear(); 
	printf("\n\n\n\n******************"
		"************************"); 
	printf("\n\n\n\t****MY SHELL****"); 
	printf("\n\n\n\n*******************"
		"***********************"); 
	char* username = getenv("ÖMER YILDIZ"); 
	printf("\n\n\nUSER is: @%s", username); 
	printf("\n"); 
	sleep(1); 
	clear(); 
} 

// Function to take input 
int takeInput(char* str) 
{ 
	char* buf; 

	buf = readline("\n>>> "); 
	if (strlen(buf) != 0) { 
		add_history(buf); 
		strcpy(str, buf); 
		return 0; 
	} else { 
		return 1; 
	} 
} 

// Function to print Current Directory. 
void printDir() 
{ 
	char cwd[1024]; 
	getcwd(cwd, sizeof(cwd)); 
	printf("\nDir: %s", cwd); 
} 

// Function where the system command is executed 
void execArgs(char** parsed) 
{ 
	// Forking a child 
	pid_t pid = fork(); 

	if (pid == -1) { 
		printf("\nFailed forking child.."); 
		return; 
	} else if (pid == 0) { 
		if (execvp(parsed[0], parsed) < 0) { 
			printf("\nCould not execute command.."); 
		} 
		exit(0); 
	} else { 
		// waiting for child to terminate 
		wait(NULL); 
		return; 
	} 
} 

// Function where the piped system commands is executed 
void execArgsPiped(char** firstCommand, char** secondCommand) {
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 < 0) {
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execvp(firstCommand[0], firstCommand);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid1, NULL, 0);
        pid2 = fork();
        if (pid2 < 0) {
            exit(EXIT_FAILURE);
        } else if (pid2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execvp(secondCommand[0], secondCommand);
            exit(EXIT_FAILURE);
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid2, NULL, 0);
        }
    }
}
// Function where the piped system commands is executed 
void execArgsSemicolon(char** parsed, char** parsedCommand) 
{ 
	pid_t p1, p2; 
	int status;
	p1 = fork(); 
	if (p1 < 0) { 
		printf("\nCould not fork"); 
		return; 
	} 

	if (p1 == 0) { 
		// Child 1 executing.. 
		printf("hello child 1");

		if (execvp(parsed[0], parsed) < 0) { 
			printf("\nCould not execute command 1.."); 
			exit(0); 
		} 
	}
	p2 = fork(); 

	if (p2 < 0) { 
		printf("\nCould not fork"); 
		return; 
	} 
	if (p2 == 0) { 
		
		if (execvp(parsedCommand[0], parsedCommand) < 0) { 
			printf("\nCould not execute command 2.."); 
			exit(0); 
		} 
	} else { 
		// parent executing, waiting for two children 
		
		waitpid(p1, &status, 0);
		waitpid(p2, &status, 0);
	} 
} 

// Help command builtin 
void openHelp() 
{ 
	puts("\n***WELCOME TO MY SHELL HELP***"
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>quit"
		"\n>all other general commands available in UNIX shell"
		"\n>pipe handling"
		"\n>semicolon handling"
		"\n>improper space handling"); 

	return; 
} 
void add_command_to_history( const char *command )
{
   if (history_count < HISTORY_MAX_SIZE) {
        history[history_count++] = strdup( command );
   } else {
        free( history[0] );
        for (unsigned index = 1; index < HISTORY_MAX_SIZE; index++) {
            history[index - 1] = history[index];
        }
        history[HISTORY_MAX_SIZE - 1] = strdup( command );
    }
}
void listHistory(){
    for(int i = 0 ; history[i] != NULL &&  i != HISTORY_MAX_SIZE ; i++)
        printf("[%d]=> %s \n",i,history[i]);

}

// Function to execute builtin commands 
int ownCmdHandler(char** parsed) 
{ 
	int NoOfOwnCmds = 4, i, switchOwnArg = 0; 
	char* ListOfOwnCmds[NoOfOwnCmds]; 
	char* username; 

	ListOfOwnCmds[0] = "quit"; 
	ListOfOwnCmds[1] = "cd"; 
	ListOfOwnCmds[2] = "help"; 
	ListOfOwnCmds[3] = "history"; 

	for (i = 0; i < NoOfOwnCmds; i++) { 
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
			switchOwnArg = i + 1; 
			break; 
		} 
	} 

	switch (switchOwnArg) { 
	case 1: 
		printf("\n Goodbye \n"); 
		exit(0); 
	case 2: 
		chdir(parsed[1]); 
		return 1; 
	case 3: 
		openHelp(); 
		return 1; 
	case 4: 
		listHistory();
		return 1; 
	default: 
		break; 
	} 

	return 0; 
} 

// function for finding pipe 
int parsePipe(char* str, char** strpiped) 
{ 
	int i; 
	for (i = 0; i < 2; i++) { 
		strpiped[i] = strsep(&str, "|"); 
		if (strpiped[i] == NULL) 
			break; 
	} 

	if (strpiped[1] == NULL) 
		return 0; // returns zero if no pipe is found. 
	else { 
		return 1; 
	} 
} 
// function for finding pipe 
int parseSemiCol(char* str, char** strpiped) 
{ 
	int i; 
	for (i = 0; i < 2; i++) { 
		strpiped[i] = strsep(&str, ";"); 
		if (strpiped[i] == NULL) 
			break; 
	} 

	if (strpiped[1] == NULL) 
		return 0; // returns zero if no semicolon is found. 
	else { 
		return 1; 
	} 
} 
// function for parsing command words 
void parseSpace(char* str, char** parsed) 
{ 
	int i; 

	for (i = 0; i < MAXLIST; i++) { 
		parsed[i] = strsep(&str, " "); 

		if (parsed[i] == NULL) 
			break; 
		if (strlen(parsed[i]) == 0) 
			i--; 
	} 
} 

int processString(char* str, char** parsed, char** parsedpipe, char** parsedcommand) 
{ 

	char* strpiped[2]; 
	int piped = 0; 
	int semicol = 0;

	piped = parsePipe(str, strpiped); 
	semicol = parseSemiCol(str , strpiped);

	if (piped) { 
		parseSpace(strpiped[0], parsed); 
		parseSpace(strpiped[1], parsedpipe); 

	} 
	else if (semicol){
		parseSpace(strpiped[0], parsed); 
		parseSpace(strpiped[1], parsedcommand); 
		semicol++;
	}
	else { 
		parseSpace(str, parsed); 
	} 

	if (ownCmdHandler(parsed)) 
        return 0; 
    else
        return 1 + piped + semicol; 

	
} 

int main(int argc, char *argv[]) {
	if(argc < 2){
		char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
		char* parsedArgsPiped[MAXLIST]; 
		char* parsedArgsSemicolon[MAXLIST]; 

		int execFlag = 0; 
		init_shell(); 

		while (1) { 
			// print shell line 
			printDir(); 
			// take input 
			
			if (takeInput(inputString)) {
				
				continue; 

			}

				
			// process 
			add_command_to_history(inputString);
			execFlag = processString(inputString, 
			parsedArgs, parsedArgsPiped , parsedArgsSemicolon); 
			// execflag returns zero if there is no command 
			// or it is a builtin command, 
			// 1 if it is a simple command 
			// 2 if it is including a pipe, 
			// 3 if it is including a semicolon.

			// execute 
			if (execFlag == 1) 
				execArgs(parsedArgs); 

			if (execFlag == 2) 
				execArgsPiped(parsedArgs, parsedArgsPiped); 
			if (execFlag == 3) 
				execArgsSemicolon(parsedArgs, parsedArgsSemicolon); 
		} 
	return 0; 
} 
	if (argc == 2){
		// Read the batch file

		char line[MAX_LINE_LENGTH];
		char *parsedArgs[MAXLIST]; 
		char* parsedArgsPiped[MAXLIST]; 
		char* parsedArgsSemicolon[MAXLIST]; 
		int execFlag = 0;


		FILE *batch_file = fopen(argv[1], "r");


		if (batch_file == NULL) {

			perror("Error opening file");

			exit(EXIT_FAILURE);

		}



		while (fgets(line, sizeof(line), batch_file) != NULL) {

			// Remove newline character if present

			line[strcspn(line, "\n")] = 0;


			execFlag = processString(line, 
			parsedArgs, parsedArgsPiped , parsedArgsSemicolon); 
			// Execute the command
			if (execFlag == 1) 
				execArgs(parsedArgs); 
			if (execFlag == 2) 
				execArgsPiped(parsedArgs, parsedArgsPiped); 
			if (execFlag == 3) 
				execArgsSemicolon(parsedArgs, parsedArgsSemicolon); 
		

		}



		// Close the file

		fclose(batch_file);

		}

}
