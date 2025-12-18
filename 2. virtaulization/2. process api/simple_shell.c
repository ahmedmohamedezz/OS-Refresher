#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <unistd.h>     // fork, getpid, exec
#include <sys/wait.h>   // wait

// run using
// 1. compile: gcc simple_shell.c -o simple_shell
// 2. run: ./simple_shell

int main()
{
    printf("Simulating shell...\n");
    printf("Shell pid: %d\n", (int)getpid());

    // create a new process
    int pid = fork();
    if (pid < 0)
    {
        // in case of failure
        fprintf(stderr, "fork failed");
        exit(1);
    }

    // replace the new process with the program you want to run
    if (pid == 0)
    {
        // child process
        printf("Child pid: %d\n", (int)getpid());

        // suppose the user runs `ls -l` command
        char *args[] = {"ls", "-l", NULL};

        // replace the child process program
        execvp(args[0], args);

        // will only reach this line if execvp() failed
        fprintf(stderr, "exec failed");
        exit(1);
    }
    else
    {
        // parent process
        // if job is not executed in background -> wait for child
        wait(NULL);
        printf("Child process finished, back to shell pid: %d\n", (int)getpid());
    }

    return 0;
}
