// C Program that illustrate communication between two process using unnamed pipes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
    int pipefd[2];  // pipefd[0]: read end, pipefd[1]: write end
    pid_t pid;
    char write_msg[] = "Hello from parent process!";
    char read_msg[BUFFER_SIZE];

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        close(pipefd[0]); // Close unused read end

        // Write to the pipe
        write(pipefd[1], write_msg, strlen(write_msg) + 1);
        printf("Parent sent: %s\n", write_msg);

        close(pipefd[1]); // Close write end after writing
    } else {
        // Child process
        close(pipefd[1]); // Close unused write end

        // Read from the pipe
        read(pipefd[0], read_msg, sizeof(read_msg));
        printf("Child received: %s\n", read_msg);

        close(pipefd[0]); // Close read end after reading
    }

    return 0;
}