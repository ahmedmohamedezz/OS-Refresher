# Operating Systems Three Easy Pieces (OSTEP)

- what happens when a program runs ?
  - it executes it's instructions. 
  - the processor (fetch, decode, execute)
  - the processor fetches an instruction from memory, decodes it (to know what type of operation it is), then executes it. then fetch the next and so no until the program completes

- os: responsible for providing the programs with easy way to use the resources of a device.
  - it's also knwon as **_resource manager_** as it manage resources like (memory, cpu, devices) to be use by multiple programs.
  - it provides APIs that programs can call in order to make use of the system (aka **_system calls_**)

---

### Virtualization

- the os is in charge of making sure the system operates efficiently in an easy to use manner.

- one important way to do this is via a general technique known as **_virtualization_**

- achieving it, the os takes a physical resource (processor, memory, disk) and turned it into a more general easy-to-use virtual form of itself

- that's why the os is sometimes called **_virtual machine_**

- let's see how the os virtualize the CPU. 

- look at `Cpu.java` program, suppose we have a cpu with single core. this program burns the cpu for a given time in an infinite loop, and print the input message every iteration

- compile, and run different instances from the program (check comment to do that). you'll notice that it seems that they all are running at the same time, despite that the no. of CPU cores is much lower than the number of the processes running on the os

- the illusion that we have many CPUs to run any number of programs is how the os virtaulize the CPU

- another example that each program running on the os think that he owns the whole memory. despite that the os is responsible for virtualizing it and mapping each application **_address space_** locations to real locations in physical memory.

---

### Concurrency

- we concurrency to refer to different things that should all run at the same time within the same program.

- it has many problems within the os, and within any muilt-threaded program.

- run the `ThreadCounter.java` program. it starts 2 threads incrementing a shared counter, and the main thread waits them to finish `join()`, then prints the result of the counter.

- running the program with `N` as the counter, we should see `2N` printed at the end. but unfortunately that's not the case.

> note that incrementing the counter composes 3 different instructions (loading into register, increment, store back in memory)

---

### Persistence

- data stored in RAM can be lost if power gone away, or the program crashed. so we need HW & SW to be able to store data **_persistently_**.

- the sw that manages the disk is called the **_file system_**

---

### Milestones in OS history

#### Simple libraries

- in the beginning, the operating system didn’t do too much.

- it was just a set of libraries of commonly-used functions

- on these old mainframe systems, one program ran at a time,
as controlled by a human operator. 

- This mode of computing was known as **_batch processing_**, as a number of jobs were set up and then run in a “batch” by the operator.
  - batch processing was a way to maximize expensive CPU usage by running many jobs sequentially with no user interaction.

#### Protection

-  One important aspect was the realization that code run on behalf of the OS was special; it had control of devices and thus should be treated differently than normal application code.

-  imagine if you allowed any application to read from anywhere on the disk; the notion of privacy goes out the window

- thus, we need to make a **_filesystem_**

- instead of providing OS routines as libraries where you just make a **_procedure call_** to use them. we need to make the transition into the OS more formal, controlled process.

- this where **_system calls_** jump-in.

- system calls transfers control to OS in high privilaged mode called **_kernel mode_**.

- applications run in **_user mode_** (hw restrict op. they can do).
  - can't initiate I/O request to desk
  - can't access physical memory
  - can't send a packet on the network

- system calls are initiated through a special HW instruction called **_trap_**

#### Multi-programming

- multiprogramming became commonplace due to the desire to make better use of machine resources.

- Instead of just running one job at a time, the OS would load a number of jobs into memory and _switch_ rapidly between them, thus improving _CPU utilization_.

- from this point, memory protection became important. we don't want one program to access the memory of another one.

- also, we need to know how to deal with concurrency issues.


- then overtime, mini-computers were introduced, then personal-computers (PCs). also Unix came to the world, DOS, and the early OSs. until introducing android and smart phones.
