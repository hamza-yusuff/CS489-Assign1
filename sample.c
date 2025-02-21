#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 256
    
int main(int argc, char** argv) {
    // Ensure that the user supplied exactly one command-line argument
    if (argc != 2) { 
        fprintf(stderr, "Please provide the address of a file as an input.\n");
        return -1;
    }

    // Validate that the file exists before executing the command
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: The specified file does not exist.\n");
        return -1;
    }
    fclose(file);

    // Securely construct the command using snprintf
    char cmd[BUFSIZE];
    if (snprintf(cmd, BUFSIZE, "wc -c < %s", argv[1]) >= BUFSIZE) {
        fprintf(stderr, "Error: File path is too long.\n");
        return -1;
    }

    // Use execvp instead of system() to mitigate command injection
    char *args[] = {"/bin/sh", "-c", cmd, NULL};
    execvp(args[0], args);

    // If execvp fails
    perror("Error executing command");
    return -1;
}
