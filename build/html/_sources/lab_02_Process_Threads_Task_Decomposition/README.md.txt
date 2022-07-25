# 02 - Process, Threads, Task Decomposition

___
## 1. Introduction
In this laboratory we wish to explore the operating system (OS) primitives available to hand-craft a parallel program. Operating Systems can be categorised into multi-threading and multi-process operating systems and both will tend to offer some form of encapsulation of both and scheduling of both. This can often be leveraged to facilitate distribution of processes and/or threads across multiple executing cores. It is this domain we wish to explore with this laboratory. 

We shall start by understanding the OS – in this case Linux - interface to threads and processes. Then we convert a piece of code for a single-core processor to be able to run on multiple cores.   

There are three lab sessions dedicated to this laboratory. Group work is recommended.


___
## 2. Learning Aims

1. The difference between a process and a thread and their resource-allocations
2. The articulation of that difference in a rich OS like Linux
3. The POSIX system-calls to instantiate threads and processes, specifically under Linux.
4. The use of the task decomposition pattern on a computationally expensive algorithm  
5. Pining threads and processes to a core
6. The interaction of threads and processes with the scheduler
7. The run-time behaviour of threads and processes


___
## 3. Task 1: Parallelisation with Threads
**Recommended ET, optional for IT**

### 3.1 Part 1: Threads, understanding, instantiation, manipulation, visualisation

In this section we shall be looking at creating a multi-threading program and the effects of processor
scheduling, if any.
Inspect the code in the file **L2P1_threads.c** and answer the following questions
1. Predict what it does.
2. What is necessary to integrate threading in a piece of c-code? (hint – don’t forget to check the makefile)
3. Build the code and let it run (`make … ./out.e`)
4. Does it do what you predicted?
5. How many units of execution are run?
6. You see the print of “Thread 1” and “Thread 2” in seeming random fashion – this is correct behaviour as the threads are scheduled by the Linux scheduler. What do you learn from this behaviour?

    Uncomment the line 
    
    `selectCPU(0)`

7. What does `selectCPU(0)` do?
8. Any behavioural change noticeable?
9. Do you think there is a temporal difference?
10. Why is a separate variable required to identify thread number?

    We posit that pthreads under Linux are kernel level threads, that is the kernel is responsible for scheduling the threads. We also posit that the process is the unit of resource ownership and a unit of scheduling, and that a thread is “only” a unit of scheduling. This implies that the threads run under the same Process ID (pid) and that each thread has its own Thread ID (tid).  
    
    The process ID can be gotten via the call
    
    `pid_t pid = getpid();  `
    
    and the thread ID can be gained by the call  
    
    `pid_t tid = syscall(SYS_gettid) ` 

11.  Prove the above posits by using these calls and printing out the results in appropriate positions in the code. Draw a call graph. What is the value of the TID if there is no thread running?


### 3.2 Part 2: Task Decomposition using Threads
**Recommended ET & IT**

We strongly recommend you keep the code you write for reference in the next section and, especially, for further use in the next labs

Examine the naïve Sobel implementation. Download it onto your board and let it run. It takes a
image.png file and generates a edge_image.png file. You can upload the image file to your PC
to view it. You should be able to substitute any (8-bit) colour .png file as the input. It’s not well tested – don’t overdo it.

Your task is to convert the code to a task-decomposition based architecture using threads – before you do so consider the following

1. How many units of execution do you really need? How many would be useful?
2. Draw the CFG of your solution
3. Where, when and why are you going to fork?
4. What do you intuit about the performance – the cost of thread creation versus the cost of the function?

[https://computing.llnl.gov/tutorials/pthreads/](https://computing.llnl.gov/tutorials/pthreads/)  is an excellent resource.

___
## 4. Task 2: Parallelisation with Processes

### 4.1 Processes, understanding, instantiation, manipulation, visualisation 
**Recommended ET**

Check out the code for part 1. Examine the code, download it, let it run, look at the output. 
1.	How is the code organised? 
2.	What do you notice on the output? 
3.	What effect are you looking at? 
    Shared memory is one solution to the problem we shall handle this in theory later but by adding the following code before the fork
    ```C
    // pointer to shared memory
    void *shmr;
    // file handler and length (size) of shared memory – you need to allocate a value to len
    int fd, len
    // open a shared memory handle
    fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0700);
    // allocate bytes
    ftruncate(fd, len);
    // map into memory map of process
    shmr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    ```


    you can treat the memory pointed to by `shmr` as if it were memory allocated by `malloc()`. This method only works for related processes. Note the position of the -lrt switch in the linker command (makefile). Note the child process inherits the handlers and shared memory region.

4.	What is the output now?
5.	What do you have to ensure in a read-write scenario where the entire array can be written to by the processes?
6.	Printout the PIDs of the two processes

### 4.2 Task Decomposition using Processes 
**Recommended ET & IT**

Implement the sobel algorithm as task decomposition implemented as a multi-process program. If you haven’t done the first section make sure your output looks exactly like those generated by the base code. If you have done the first part, there should be no difference between the output of this task and the output of your code for thread-based task-decomposition. 

___
## 5. Task 3: Time, Scheduling, Pinning 
**Recommended ET & IT**

### 5.1 Cores

Find out what the system calls

```C
get_nprocs_conf();
get_nprocs();
```

do and what your system tells you when you use them. Is there any difference in the information when you use the (keyboard) system call
 
```bash
lscpu
```
?

### 5.2 Time

Place the system calls, read the `man` pages first,  

```C
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
```

initalising all the variables etc. in appropriate positions in the code to measure the runtime of the process with and without threading and the process-based task-decomposition code you have just written. Let the code run in a loop ~ten times. 


7.	What does the `CLOCK_PROCESS_CPUTIME_ID` flag mean?
8.  What do you notice about the times? 

### 5.3 Scheduling

What does the following system call do?
```C
int policy = sched_getscheduler(0);
```
9. 	Put it into your code – what scheduler is being used?
10.	What system call is used to set the scheduler?
    Write the code to set a specific scheduler and a task priority use it to change the scheduler from whatever to `SCHED_FIFO`
11.	 Set the scheduler at the beginning of the main process – what scheduler do the spawned threads and processes use, why?  
12.	 Do the execution times of your program change? 


### 5.3 Pinning

Examine the code in the file `setAffinity.c` use the calls to examine the current affinity at the start of the code and to change the affinity to set the core for specific processes/threads. 

13.	Do the runtimes times of your program change? 
    Add a worker thread performing an endless loop. Open another terminal and run `htop` and then in the first terminal let your program run. 
14.	Which core is fully occupied and which threads are still running?
15.	Why is one of your threads running most of the time and the other sleeping?
    You can kill the thread by opening a third terminal and issuing the 
    ```bash
    kill <thread_id>
    ```
    command.
16.	If you kill the thread with the higher TID, what happens?
17.	If you kill the thread with the lower TID what happens?


___
## 6. Evaluation

This laboratory is not evaluated - you will almost certainly need it to be able to complete the marked laboratories.   
I **welcome** in-depth discussions on the contents of the lab and your solutions.  

___
## 7. Version

| Version | Date | By | Comments | Class Level | Module |
| :-- | :-- | :-- | :-- | :-- | :-- |
| V1.1 | 02.2022 | donn | Added some sections | 6S | MPC |
| V1.0 | 02.2021 | donn | First version | 6S | MPC |
