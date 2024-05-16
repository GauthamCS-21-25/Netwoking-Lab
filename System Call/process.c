#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t c_pid;
    
    c_pid = fork();

    if (c_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (c_pid == 0) {

        printf("Child process: PID=%d, Parent PID=%d\n", getpid(), getppid());

        char *program = "/bin/ls";
        char *args[] = {"ls", "-l", NULL};

        if (execvp(program, args) == -1) {
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }
    } else {

        printf("Parent process: PID=%d, Child PID=%d\n", getpid(), c_pid);

        int status;
        waitpid(c_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not exit normally\n");
        }
    }

    printf("Process with PID=%d exiting\n", getpid());
    exit(EXIT_SUCCESS);
}
