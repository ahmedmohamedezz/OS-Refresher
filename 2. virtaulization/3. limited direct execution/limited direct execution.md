# Mechanism: Limited Direct Execution

- to achieve virtualization, the OS needs to share the physical CPU

- the idea is simple. we will run a process for a little while, then another, and so forth by **_time sharing_** the CPU.

- challenges we need to overcome:
  - performance: how to share the CPU without excessive overhead
  - control: how to control the processes running. since OS is in charge of resources.

- to do so, one of the techniques is called **limited direct execution**
    - the os creates a process for this program
    - allocate memory to it
    - load the program from disk
    - setup stack, clear registers
    - execute the program's `main()` 
    - then, when program returns from main -> clear resources

- challenges:
  - how do we know that the program hasn't done anything that it shouldn't do ?
  - how to stop a running process & run another

---

### Problem 1: Restricted Operations

- as you may notice, this approach has the advantage of being fast. since processes exectue directly on the cpu

- so, we need to make sure that the running process is not wishing to perform a restricted operation such as:
  - issuing I/O requests to disk
  - gaining access to more system resources (memory, CPU)

> Q: how the HW & OS work together to let processes perform restricted operations without giving it complete control over the system ?


- HW assists the OS by providing execution modes (usermode, kernel mode)
- special instruction are also provided to **_trap_** into the kernel mode, and to **_return-from-trap_** back to user mode

- os tells the hw where the trap table resides in memory through putting its address in cpu register. so, the cpu knows when a trap happens where to look.

- one approach is to let any process do anything in terms of I/O
  - but this would prevent any kind of protection.
  - a file system will need to check for file permissions first. or a process could write the whole disk

- so, in the user mode. any code that runs is restricted in what it can do

- an attemp to I/O requests will result in raising an exception. the os then may kill the process

- only the kernel mode can do such operations. so, what a process should do when it wants to perform such operations (like reading from disk) ?

- the processes would then need to execute a **_system call_**. through out a special **_trap_** instruction.

- when a trap is called. the instruction jumps into the kernel mode raising the privilage level. and letting the os do the desired operation (if allowed) then call a special **_return-from-trap_** instruction which returns into the user program reducing the privilage level

- in order to let the trap knows which code to run inside the os system when a certain event occurs. the kernel does so by setting up a trap table at boot time.
    - the os tells the hw  what code to run when certain exceptional events occur.
    - the os informs the hw with the locations of trap handlers

![limited direct execution protocol](./LDE%20protocol.png)

- once the hardware is informed, it remembers the location of these handlers until the machine is next rebooted, and thus the hardware knows what to do (i.e., what code to jump to) when system calls and other exceptional events take place.

---

### Problem 2: Switching Between Processes

- the next problem is achieving switching between processes.

- if a process is running on the CPU, this by definition means the OS is not running. if the OS is not running, how can it do anything at all?

> Q: How can the operating system regain control of the CPU so that it can switch between processes?

#### Cooperative approach: wait for system calls
 
- used in some early systems
- in this style, the OS trusts the processes of the system to behave reasonably

- you might think what if a process intentionally or not (bugs) ended up in an infinite loop. and nevern made a system call. what can the OS do then ?

#### Non-Cooperative approach: OS takes control

> Q: How can the OS gain control of the CPU even if processes are not being cooperative? 


> Q: What can the OS do to ensure a rogue process does not take over the machine?

- the answer turns out to be simple and was discovered by a number
of people building computer systems many years ago: a **_timer interrupt_**

- a timer device can be programmed to raise an interrupt every
so many milliseconds

- when the interrupt is raised, the currently running process is halted and a pre-configured interrupt handler in the os runs. 

- and thus, the os re-gains control. and can do what it wants (even if processes act in a non-coop way)

- note that the hardware has some responsibility when an interrupt occurs, in particular to save enough of the state of the program that was running when the interrupt occurred such that a subsequent return-fromtrap instruction will be able to resume the running program correctly

#### Saving & Restoring Context

- the os **_scheduler_** is responsible for making a decision whether to continue running the currently-running process, or switch to a different one

- if the decision is switch. the os will execute a low-level piece of code refered to as a **_context-switch_**.

- in a context-switch, all the os has to do is to save a few register values for the currently executing process. and restore those values for the upcoming process (from its kernel stack, for example)
    
- in context switch. the os saves:
  - kernel stack pointer (unique/process): refers to the stack pointer value the CPU uses when that process is executing in kernel mode.
  - program counter
  - other registers values

- note that there are two types of register saves/restores that happen
during this protocol. 

   - the first is when the timer interrupt occurs; in this case, the _user register state_ of the running process is implicitly saved by the _hardware_, using the _kernel stack of that process_. 
   - the second is when the OS decides to switch from A to B; in this case, the _kernel register state_ is explicitly saved by the software (OS), but this time into memory in the _process structure of the process_

- the latter action moves the system from 
  - running as if it just trapped into the kernel from A 
  - to as if it just trapped into the kernel from B


#### What About Concurrency ?

- the OS does indeed need to be concerned as to what happens if, during interrupt or trap handling, another interrupt occurs.

> this is discussed in detail later in the book

- one simple thing an OS might do is _disable_ interrupts during interrupt processing

---

#### Summary

- The basic idea of **_LDE_** is straightforward: just run the program you want to run on the CPU, but first make sure to set up the hardware so as to limit what the process can do without OS assistance.

