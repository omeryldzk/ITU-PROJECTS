#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/wait.h>



#define MAX_LINE_LENGTH 250

#define WRITE_END 1                              // INPUT_END means where the pipe takes input
#define READ_END 0                             // OUTPUT_END means where the pipe produces output
                                                 



void execute_single_command(char *commands) {
    char *args1[3];
    char *args2[3];
    char *token;
    char *delim = " ;";

    int index = 0;
    token = strtok(commands, delim); // Komutları ' ;' karakterine göre ayır

    while (index != 2) {
        args1[index] = token;
        index++;
        token = strtok(NULL, delim); // Bir sonraki komuta geç
    }
    args1[index] = NULL; // args dizisinin sonunu işaretlemek için NULL ekleyin
    int index2 = 0;
    while (index2 != 2) {
        args2[index2] = token;
        index2++;
        token = strtok(NULL, delim); // Bir sonraki komuta geç
    }
    args2[index2] = NULL; // args dizisinin sonunu işaretlemek için NULL ekleyin
    int pid = fork();
    if (pid == 0) { // Çocuk işlemi oluştur
        execvp(args1[0], args1); // İlk komutu ebeveyn işlemde çalıştır
        perror("execvp failed"); // Hata durumunda çıktı ver
        exit(EXIT_FAILURE);
    } else {
        // Ana işlemdeyiz
        int status;
        // Çocuk işleminin tamamlanmasını bekleyelim
        waitpid(pid, &status, 0);
        // İkinci komutu çalıştıralım
        execvp(args2[0], args2); // İkinci komutu çocuk işlemde çalıştır
        perror("execvp failed"); // Hata durumunda çıktı ver
        exit(EXIT_FAILURE);

    }
}



void execute_commands(char *commands) {

    char *single_commands[MAX_LINE_LENGTH];

    char *pipe_commands[MAX_LINE_LENGTH];

    char *pipe_token = strtok(commands, "|");

    int i =0;

    while (pipe_token != NULL) {

        printf("%s\n", pipe_token);

        pipe_commands[i++] = pipe_token;

        pipe_token = strtok(NULL, "|");

    }
        int fd[2];
        int j = 0;
        if (pipe(fd) == -1) {
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }
        int pid = fork();
        
        if (pid == -1) {

            perror("Fork failed");
            exit(EXIT_FAILURE);

        } else if (pid == 0) {  // Child process

            dup2(fd[READ_END], STDIN_FILENO);
            close(fd[WRITE_END]);
            execute_single_command(pipe_commands[j++]);
            exit(EXIT_SUCCESS);

        } else {  // Parent process
            // Parent: Output is to child via pipe[1]
            int status;
            waitpid(pid, &status, 0);
            // Change stdout to pipe[1]
            dup2(fd[WRITE_END], STDOUT_FILENO);
            close(fd[READ_END]);
            execute_single_command(pipe_commands[j]);
            j++;
        }

    }



int main(int argc, char *argv[]) {

    // Check if a batch file is provided

    if (argc != 2) {

        fprintf(stderr, "Usage: %s batch_file\n", argv[0]);

        exit(EXIT_FAILURE);

    }



    // Read the batch file

    char line[MAX_LINE_LENGTH];

    FILE *batch_file = fopen(argv[1], "r");


    if (batch_file == NULL) {

        perror("Error opening file");

        exit(EXIT_FAILURE);

    }



    while (fgets(line, sizeof(line), batch_file) != NULL) {

        // Remove newline character if present

        line[strcspn(line, "\n")] = 0;



        // Execute the command

        execute_commands(line);

    }



    // Close the file

    fclose(batch_file);



    return 0;

}