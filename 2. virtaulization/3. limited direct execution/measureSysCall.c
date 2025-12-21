#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main()
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    pid_t pid = getpid(); // system call

    clock_gettime(CLOCK_MONOTONIC, &end);

    long sec = end.tv_sec - start.tv_sec;
    long nsec = end.tv_nsec - start.tv_nsec;

    long elapsed_ns = sec * 1000000000L + nsec;

    printf("getpid() took %ld ns (pid=%d)\n", elapsed_ns, (int)pid);
    return 0;
}