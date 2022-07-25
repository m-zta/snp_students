# 03 - Data Decomposition
___
## 1. Introduction
In this laboratory we wish to explore the application of the data-decomposition pattern on the Sobel edge-detection problem encountered in the previous lab. You will need to decide on a geometric decomposition of the image and then implement the parallelisation of the Sobel algorithm.  

There are three lab sessions dedicated to this laboratory. The laboratory will be marked. Group work is highly recommended. 
___
## 2. Learning Aims

1.	The difference between a process and a thread and their resource-allocations
2.	The articulation of that difference in a rich OS like Linux
3.	The POSIX system-calls to instantiate threads and processes, specifically under Linux.
4.	The use of the task decomposition pattern on a computationally expensive algorithm  
5.	Pining threads and processes to a core
6.	The interaction of threads and processes with the scheduler
7.	The run-time behaviour of threads and processes



___
## 3. Task 1: Parallelisation with Threads
**Recommended ET & IT**

Convert the Sobel algorithm base code into a data-decomposition based program. 

We have supplied you with a couple of base utilities, namely code for processing pixels - which you can decide upon yourselves and combine these routines to create, manage and process chunks of data. There is a main program available but you need to ask yourself does this conform to the pattern I require? Change at will. 

In its current instantiation `process_chunk()` processes the entire picture into black and white by pixel. It also illustrates the max indices and general usage of the helper functions. Feel free to create your own. 

You task is to decide what geometrical pattern you want to use and why, the reasoning behind the size of the chunks, the implementation pattern etc. and then implement it and characterise it 

Be prepared to explain the following: 

1. What was the process of deciding on the chunks? 
2. How did you handle boundary data? 
3. What other design constraints did you consider? 
4. Would you do it that way again? 



___
## 4. Evaluation

This laboratory is evaluated on the basis of the discussions with you on your code/solution. You can receive a maximum of 5 marks for this laboratory.    

A perfect solution is good. An imperfect solution is acceptable. If your solution doesn’t work, then it an identification of the problems and mistakes made will be accepted for grading. 

I **welcome** in-depth discussions on the contents of the lab and your solutions **during** the implementation process.  


___
## 5. Version

| Version | Date | By | Comments | Class Level | Module |
| :-- | :-- | :-- | :-- | :-- | :-- |
| V1.1 | 02.2022 | donn | Added some sections | 6S | MPC |
| V1.0 | 02.2021 | donn | First version | 6S | MPC |
