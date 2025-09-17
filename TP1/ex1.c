#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define COMMAND_SIZE 64

int data = 10;
int BSS;
char STR[] = "Text";


int main(){
    int pid, status;

    int stack = 10;
    char* heap = malloc(COMMAND_SIZE);
    int *mmap = malloc(COMMAND_SIZE);
    
    printf("Data: %p\n\r",&data);
    printf("BSS: %p\n\r",&BSS);
    printf("Str: %p\n\r",&STR); 
    printf("Heap: %p\n\r",&heap); 
    printf("Stack: %p\n\r",&stack); 
    printf("Main Function: %p\n\r",&main);
    printf("LibC Function: %p\n\r",&printf);
    printf("Mmap: %p\n\r",&stack);

    pid = fork();

    if (pid != 0) { // Parent code
        wait(&status);
    }
    else { // Child code
        char pid_str[COMMAND_SIZE];
        snprintf(pid_str, sizeof(pid_str), "%d", getppid());

        char *command[] = {"pmap","-X", pid_str, NULL};

        execv("/usr/bin/pmap", command);
        perror("Error while trying to execute the command.");
        exit(EXIT_FAILURE);
    }

    return 0;
}