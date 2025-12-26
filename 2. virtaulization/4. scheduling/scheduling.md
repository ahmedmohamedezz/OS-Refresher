# Scheduling 

- before getting into the range of possible policies, let us first make a number of simplifying assumptions about the processes running in the system, sometimes collectively called the **_workload_**. Determining the workload is a critical part of building policies, and the more you know about workload, the more fine-tuned your policy can be.

- let us start with these assumptions (un-realistic for now)
    - each process (aka **_job_**) runs for the same amount of time
    - all jobs arrive at the same time
    - all jobs only use the CPU (perform no I/O)
    - the run-time of each job is unknown

- another key-thing we will need is how to compare different scheduling algorithms. we need a **_metric_**.

- we will use the turnaround time metric for now
  
  
- turnaround time is the (completion time - arrival time)
  - `T_turnaround = T_completion - T_arrival`

- note that turnaround time is a _performance metric_ which in order to optimize performance, the cost is preventing a few jobs from running, thus decreasing _fairness_ which is another metric.

---

### First-In, First-Out (First Come, First Serve)

- suppose we have 3 jobs A, B, C.
- arrived at the same time (assume A, then B, then C)
- each will run for 10 seconds

- FCFS we run `A` (0 - 10), then `B` (10 - 20), then `C` (20 - 30)

- the avg turnaround time is `(10 + 20 + 30) / 3` = `20` seconds

- now, assume relaxing our first assumption: _all run for the same amount of time_. what workload we can construct to make FCFS perform poorly ?
    - sort desc by the time needed to finish => more waiting time for nxt jobs

- this is known as **_convoy effect_**

- `convoy effect`: when a number of relatively-short potential consumers of a resource get **_queued_** behind a heavy-weight resource consumer.

- so, what algorithm can we use to avoid this queueing problem ?
  - can we run the shortest job first ?

---

### Shortest Job First (SJF)

- as the name says, we will run the shortest first, then the 2nd shortest and so on.

- let's test on our last example where `A`, `B`, and `C` arrive at the same time. 
  - A needs 100 seconds to complete
  - B needs 10 seconds to complete
  - C needs 10 seconds to complete

- with FCFS
  - the worst case is `(100 + 110 + 120) / 3` which is `110` seconds

- with SJF
  - we can get `(10 + 20 + 130) / 3` which evaluates to `50` seconds

- however, up-till-now we're assuming that they all arrive at the same time. but that's not the case in real systems

- so, let us relax our 2nd assumption: _all arrive at the same time_. think of the worst case that can happen now with SJF ?
    - if the heaviest arrived first, and the light-weight arrived last

- let see what will be the turnaround time if
  - `A` arrived first (t = 0)
  - `B` and `C` arrived at (t = 10)
  
- `T = ((100 - 0) + (110 - 10) + (120 - 10)) / 3` which eval. to `103.33` seconds

- what can we do ? can we run the job with the shortest time to complete first ?

---

### Aside: Preemtive vs Non-preemptive Schedulers

- In the old days of batch computing, a number of _non-preemptive_ schedulers were developed; such systems would run each job to completion. before considering whether to run a new job. 

- Virtually, all modern schedulers are _preemptive_, and quite willing to stop one process from running in order to run another. 

- This implies that the scheduler employs the mechanisms we learned about previously; in particular, the scheduler can
perform a _context switch_, stopping one running process temporarily and resuming (or starting) another.

---

### Shortest Time-to-Completion First (Preemptive SJF)

- the scheduler can certainly do something when `B`, and `C` jobs arrive. it can _preempt_ job `A`, and run another job

- by definition SJF is a _non-preemptive_ scheduler. however, another well-know scheduler adds _preemption_ to it. it's called the **_Shortest Time-to-Completion First_** (STCF), or **_Preemptive SJF_** (PSJF)

- our new scheduler will stop `A` when `B`, and `C` arrive which leads to `((20 - 10) - (30 - 10) - (120 - 0)) / 3` = `50` seconds

---

- the last approach seems like a robust & suitable one. but again, the introduction of time shared machines changed everything.

- now users would sit at a terminal and demand interactive performance from the system as well. And thus, a new metric was born: **_response time_**.

- response time is defined as the time from when the job arrives to the first time it is scheduled `T_reponse-time = T_first-run - T_arrival`

- let's calc. the response time of each job in the last example
  - `A`: 0, `B`: 0, and `C`: 10
  - avg `10 / 3 = 3.33`


- what if again all jobs arrived at the same time -> slower response time. which is not good for interactivity.
  - because a job would wait other jobs to complete first before it gets scheduled once

- so, we need a scheduler that is sensitive to response time ?

---

### Rouns Robin (RR)

- instead of running jobs to completion, _RR_ runs a job for a _time slice_ (aka __scheduling quantum__) and then switches to the next job in the run queue. 

- It repeatedly does so until the jobs are finished. For this reason, _RR_ is sometimes called _timeslicing_.

- note that the length of a time slice must be a multiple of the **_timer-interrupt_** period; thus if the timer interrupts every 10 milliseconds, the time slice could be 10, 20, or any other multiple of 10 ms.

- assume 3 jobs arrived at the _same time_, each of which wishes to run for 5 seconds. 

- avg response time is
  - for RR: `(0 + 1 + 2) / 3 = 1` second (using time slice = 1 sec)
  - for SJF: `(0 + 5 + 10) / 3 = 5` seconds

- avg response trun-round time is
  - for RR: `(13 + 14 + 15) / 3 = 14` second (using time slice = 1 sec)
  - for SJF: `(5 + 10 + 15) / 3 = 10` seconds

- it may seem that the shortest the RR time slice is, the better response time it has. but in fact, if it's too short the cost of _context switching_ will dominate overall performance.

- the length of the time slice presents a trade-off to a system designer, making it long enough to _amortize_ the cost of switching without making it so long that the system is no longer _responsive_.

- The general technique of **_amortization_** is commonly used in systems when there is a fixed cost to some operation. By incurring that cost less often (by performing the operation fewer times), the total cost to the system is reduced. 

- For example, if the time slice is set to _10 ms_, and the context-switch cost is _1 ms_, roughly _10%_ of time is spent context switching and is thus wasted. 

- If we want to amortize this cost, we can increase the time slice to _100 ms_, In this case, less than _1%_ of time is spent context switching, and thus the cost of time-slicing has been amortized.

- so, if response time is our only metric. RR is the best

- More generally, any policy (such as RR) that is fair, i.e., that evenly divides the CPU among active processes on a small time scale, will perform poorly on metrics such as turnaround time.

---

### Incorporating I/O

- a scheduler cleary has to make a decision when a job initiates an I/O request. because it's blocked waiting for I/O completion.
  - should probably run another job

- it also need to make a decision when I/O completes. an interrupt is raised. and the OS runs to move this process from (blocked -> ready) state
  - may also decide to run the process

- suppose we have 2 jobs A, B
  - each need to run `50ms` on the CPU
  - but A initiates an I/O every 10ms (suppose I/O need 10ms to complete)

- sure, we need to make use of resources when A is blocked.

- if our scheduler is _STCF_, How should such a scheduler account for the fact that A is broken up into 5 10-ms sub-jobs, whereas B is just a single 50-ms CPU demand (CPU burst)?
  - the answer is **_overlap_**

- A common approach is to treat each 10-ms sub-job of A as an independent job.
  - so SCTF will run 10ms sub-job of A first
  - then start running B for 10ms
  - then another sub-job of A is submitted pre-empting B for 10ms

- overlap operations to maximize the utilization of systems.

- With a basic approach to I/O in place, we come to our final assumption: that the scheduler knows the length of each job.

- this is likely the worst assumption we could make. In fact, the OS usually knows very little about the length of each job. Thus, 
  
1. how can we build an approach that behaves like SJF/STCF without such a priori knowledge?

2. Further, how can we incorporate some of the ideas we have seen with the RR scheduler so that response time is also quite good?

-  Shortly, we will see how to overcome this problem, by building a scheduler that uses the recent past to predict the future. 

- This scheduler is known as the **_multi-level feedback queue_**, and it is the topic of the next chapter.
  - a scheduler that uses the _recent past_ to _predict the future_.