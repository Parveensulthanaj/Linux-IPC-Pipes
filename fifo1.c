//C Program that illustrate communication between two process using named pipes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 100

int main() {
    int fd;
    char write_msg[] = "Hello from parent process via FIFO!";
    char read_msg[BUFFER_SIZE];
    pid_t pid;

    // Create the named pipe (FIFO)
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        // Continue even if FIFO already exists
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process - Writer
        fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) {
            perror("open for writing");
            exit(EXIT_FAILURE);
        }

        write(fd, write_msg, strlen(write_msg) + 1);
        printf("Parent sent: %s\n", write_msg);

        close(fd); // Close after writing
    } else {
        // Child process - Reader
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            perror("open for reading");
            exit(EXIT_FAILURE);
        }

        read(fd, read_msg, sizeof(read_msg));
        printf("Child received: %s\n", read_msg);

        close(fd); // Close after reading
    }

    // Optional: Parent can unlink (delete) the FIFO after use
    if (pid > 0) {
        unlink(FIFO_NAME);
    }

    return 0;
}