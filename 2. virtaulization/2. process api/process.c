#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// compile: gcc process.c -o process
// run: ./process

int main(int argc, char *argv[])
{
    printf("hello world (pid: %d)\n", (int)getpid());

    int rc = fork();

    if (rc < 0)
    { // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    { // child process (new)
        printf("hello, I am child (pid: %d)\n", (int)getpid());
        char *myargs[3];
        myargs[0] = strdup("wc");        // program: "wc" (word count)
        myargs[1] = strdup("process.c"); // argument: file to count
        myargs[2] = NULL;                // marks end of array
        execvp(myargs[0], myargs);       // runs word count

        // only print this if execvp() failed
        printf("this shouldn't print out");
        free(myargs[0]);
        free(myargs[1]);
    }
    else
    {               // parent process
        wait(NULL); // wait for the child process to finish
        printf("hello, I am parent of %d (pid: %d)\n", rc, (int)getpid());
    }

    return 0;
}