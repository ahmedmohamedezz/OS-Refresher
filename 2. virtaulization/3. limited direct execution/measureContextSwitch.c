#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// utility to pin 2 processes to operate on the same CPU core
void pin_to_core(int core_id) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core_id, &mask);

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }
}


int main() {
    // arr[0]: read end
    // arr[1]: write end

    int p2c[2]; // parent writes, child read
    int c2p[2]; // child writes, parent read

    pipe(p2c);
    pipe(c2p);

    pid_t pid = fork();

    struct timespec start, end;
    char buf = 'x';
    const int N = 100000;

    if((int) pid == 0) {
        // child
        pin_to_core(0);

        for (int i = 0; i < N; i++) {
            // Child waits for parent to send a byte
            // nothing in the pipe => the child blocks.
            // blocking => triggers a context switch
            // OS suspends the child and schedules the parent.
            read(p2c[0], &buf, 1);

            // writing to a pipe can wake up a blocked reader.
            write(c2p[1], &buf, 1);  // wake parent (blocking on reading c2p[0])
        }
    } else {
        // parent
        pin_to_core(0);
        
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        for (int i = 0; i < N; i++) {
            write(p2c[1], &buf, 1);  // wake child
            read(c2p[0], &buf, 1);   // block → context switch
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        long ns =
            (end.tv_sec - start.tv_sec) * 1000000000L +
            (end.tv_nsec - start.tv_nsec);

        // Each loop causes TWO context switches
        printf("Context switch time ≈ %ld ns\n", ns / (2 * N));
    }
    
}

// Time →
// Parent (P)           Child (C)
// ---------            ---------
// write p2c[1]   ──►  read p2c[0]  (P wakes C)
// (read blocks)      process byte
//                    write c2p[1] ──►  (C wakes P)
//                    (blocks on next read)
// read c2p[0]  ◄──
// (process byte)
// (next write) write p2c[1] ──►  (wake child)