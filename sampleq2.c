#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_HOLES 8
#define READ_END 0
#define WRITE_END 1

// Function to perform first fit allocation
int first_fit(int holes[], int num_holes, int request_size) {
    int i;
    for (i = 0; i < num_holes; ++i) {
        if (holes[i] >= request_size) {
            holes[i] -= request_size;
            return i; // Return index of the hole allocated
        }
    }
    return -1; // No suitable hole found
}

int main() {
    int holes[NUM_HOLES] = {10, 4, 20, 18, 7, 9, 12, 15};
    int pipe_fd[2]; // File descriptors for the pipe
    int num_requests;
    
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) { // Child process
        close(pipe_fd[WRITE_END]); // Close write end of pipe in child process
        int request_size;
        while (read(pipe_fd[READ_END], &request_size, sizeof(int)) > 0) {
            int index = first_fit(holes, NUM_HOLES, request_size);
            if (index != -1)
                printf("Allocated %d MB from hole %d\n", request_size, index);
            else
                printf("No suitable hole found for %d MB request\n", request_size);
        }
        close(pipe_fd[READ_END]); // Close read end of pipe in child process
        exit(0);
    } else { // Parent process
        close(pipe_fd[READ_END]); // Close read end of pipe in parent process
        printf("Enter the number of requests: ");
        scanf("%d", &num_requests);
        for (int i = 0; i < num_requests; ++i) {
            int request_size;
            printf("Enter the size of request %d: ", i + 1);
            scanf("%d", &request_size);
            write(pipe_fd[WRITE_END], &request_size, sizeof(int)); // Send request size through pipe
        }
        close(pipe_fd[WRITE_END]); // Close write end of pipe in parent process
        wait(NULL); // Wait for child process to finish
    }

    return 0;
}