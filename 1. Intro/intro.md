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
