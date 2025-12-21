# Process API

- in this lesson we will know how to create and control processes.

- this lesson will be a practical example of how to create and control processes

- look at the c program attached

```c
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
    }
    else
    { // parent process
        wait(NULL);     // wait for the child process to finish
        printf("hello, I am parent of %d (pid: %d)\n", rc, (int)getpid());
    }
```

- the `fork()` call in C language will create an identical process from it's caller. from this line we have 2 processes running.

- if the value of `rc` is -ev, then an error happened, if it's 0, then the child process is running, otherwise (+ve) the parent is running

- notice how we made the parent to wait for the child to complete using the `wait(null)` call. this way we're sure that always the child process is going to say hello first.

---

- sometimes we want other processes to run different programs. to do this; we're going to use `exec()` system call

- look at this version of the program

```c
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
        myargs[0] = strdup("wc");   // program: "wc" (word count)
        myargs[1] = strdup("p3.c"); // argument: file to count
        myargs[2] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs word count
        printf("this shouldn't print out");
    }
    else
    {               // parent process
        wait(NULL); // wait for the child process to finish
        printf("hello, I am parent of %d (pid: %d)\n", rc, (int)getpid());
    }
```

- `execvp()` doesn't create another process. it **_transforms_** the current into a different running program
  - heap, stack, other memory parts are re-initialized
  - it's like the main program never ran

- actaully, the `shell` (which is also a program). does something similar to what we did.

- when you type a command. the following happens
  - find (locate) that executable
  - call `fork()` to create a new process to run the executable
  - call variant of `exec()` to run the command

- another example where the separation of `fork()`, and `exec()` shines is when running this command `wc process.c > newFile.txt` in the shell
    - the shell would close the **_stdout_** and opens the `newFile.txt` after calling `fork()` and before `exec()`. so, any output will auto. redirected to that file

--- 

- there are many other system calls, like `kill()` which is used to send **_signals_** to process.

---

### Sum-up

- `fork()`: creates a new proc. by duplicating the calling proc.
  - both proc. continue execution after `fork()` call
  - returns 
    - `-ve` => failure
    - `0` => child proc. is running
    - `+ve` => parent proc. is running (return value = parent process id)
    - parent & child have separate memory spaces

- `exec()`: **_replaces_** the current process image with a new program
  - does not create any new proc.
  - after `exec()` call, the old program is gone (like it never exists)
  - it returns only on failure. in success, control never comes back
  - has diffrerent variants `execl`, `execv`, `execvp`, `execve`
  - pid stays the same (same process, but new program)