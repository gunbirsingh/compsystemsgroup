# Process Communication: Linux Processes and IPC
**Akhil Velagapudi**  
**Ramaseshan Parthasarathy**  

Given a list of integers find the Minimum, Maximum, and the Sum of the numbers. The list will be in a text file you generate. You may use any form of interprocess communication (IPC) to partition the file/array into chunks and distribute work to more than one processes (if there are multiple ones) (e.g., pipes, shared memory, or additional (perhaps more sophisticated) inherent process system calls).

## Usage
|         |Command Format                                     |Example                         | 
|---------|---------------------------------------------------|--------------------------------| 
| Part A: | `./pa.sh <input file> <output file>`              | `./pa.sh 100k.in part_a.out`   | 
| Part B: | `./pb.sh <input file> <output file> <# processes>`| `./pb.sh 100k.in part_b.out 8` | 
| Part C: | `./pc.sh <input file> <output file> <# processes>`| `./pc.sh 100k.in part_c.out 8` | 
| Part D: | `./pd.sh <input file> <output file> <depth>`      | `./pd.sh 100k.in part_d.out 4` | 

## Files
Code files:  
`info.h`  
`part_a.c`  
`part_b.c`  
`part_c.c`  
`part_d.c`  

Test input file:  
`100k.in`

Output files:  
`part_a.out`  
`part_b.out`  
`part_c.out`  
`part_d.out` 

## How it Works
The following approaces were implemented to compute the minimum, maximum, and sum of a list of numbers in an input file:
  1. Single process
  2. Multiple processes where each process spawns at most one process
  3. Multiple processes where first process spawns all other processes
  4. Multiple processes where each process can spawn multiple other processes (in our implementation, each parent process was forced to spawn exactly two child processes)
     * Number of slave processes = 2^(k - 1) where k represents the depth of the process tree

## Inter-Process Communication (IPC):  
The IPC occurs through a recursive function called process(int start, info_struct * info) which takes in *start*, the start position of each array division and a structure containing *min*, *max*, *sum*, and *count* called *info_start*. The function also has access to *size* (the number of total integers to process) and *perProcess* (the number of integers that each process is responsible for). The integers are loaded from the input file into an array, which is partitioned equally depending on the number of processes (or depth of process tree in the case of part D).

### Pipes
When the work load is split between the processes, the slave process needs to send the information that it computed to the master process. The "master process" is simply the one that is responsible for collecting and merging the data collected (*min*, *max*, and *sum*). The "slave process" processes the numbers assigned to it, sends the data to the master, and exits. Pipes were used to enable this communication. The slave process writes to the read end of a pipe and the master process blocks until something is written to the pipe. Then, the master process aggregates the data sent from the pipe.

### Shared Memory
Shared memory was also used through system calls such as `shmget`, `shmat`, `shmdt`, `shmctl`. An issue with creating multiple processes is that the numbers array that was used to store the integers needs to be copied every single time that `fork` is called. This is a waste of memory and CPU resources that was fixed by using shared memory. So, the numbers array was stored in a chunk of shared memory big enough to hold 1 million integers. Using IPC shared memory, every process is given a pointer to shared memory location. Also, there are no race conditions because the array is treated as read-only after it is initialized.

## Design Decisions and Explanations

### Number of Processes
The number of processes used by the programs is actually one of the input arguments. However, it seems best to use as many processes as there are CPU cores available. Theoretically, there should be no tangible benefits from using more processes than the number of cores because the workload cannot be more parallelized. Every slave process does computationally expensive work, which means that 8 active processes (that are not waiting) will saturate an 8-core processor and spinning up 16 active processes will provide no tangible benefit. This hypothesis is consistent with the testing that was done.

*One thing that we have learned as a result of this project is that increasing the number of processes does not always improve the parallelism of a program. In terms of performance, there are advantages and disadvantages to using more processes. Also, implementing a successful multi-process program is much more difficult that we intiially thought and race-conditions can easily pop up.*

### Process Structure
See "Inter-Process Communication (IPC)" section for information on process structure.

## Testing

### Time (μs) to Process List of Numbers
|Number of processes| Problem number   |**10**|**100**|**1000**|**10000**|**100000**|    
| :-------------:   |:----------------:|:----:|:-----:|:------:|:-------:|:--------:|    
| 1                 |A                 |1422  |1693   |1262    |1836     |13435     |    
| 4                 |B                 |2569  |2594   |2456    |3867     |9770      |    
|                   |C                 |2377  |2958   |2811    |3698     |10332     |  
|                   |D                 |2320  |2682   |2834    |3685     |8621      |  
| 8                 |B                 |2579  |2881   |3354    |4722     |10927     |      
|                   |C                 |2828  |2448   |2771    |3462     |9177      |    
|                   |D                 |2104  |2570   |2201    |3248     |8573      |    
| 16                |B                 |-     |3406   |3799    |5705     |15516     |      
|                   |C                 |-     |2899   |4935    |4047     |15957     |    
|                   |D                 |-     |3152   |4506    |5178     |14257     |    

* This test was run on a i7-4770 with 4 real cores that are hyper-threaded, 3.40 GHz base frequency, 3.90 GHz max turbo frequency, 8 MB cache, and 16 GB RAM.
* The data shows considerable slowdown with 16 processes for all multi-process programs. This is likely due to the overhead of creating more processes combined with no actual increase in parallelism.
* There is no substantial difference in run time between the 3 multi-process programs but the program for part D did run a bit faster. This is likely because the number of data merges that the program waits for is O(log(n)) rather than O(n).
* There was a significant amount of variance in the timings so it was difficult to accurately represent the run time of each program (the data values above are averages of 10 runs).
* The difference in time from processing 10 numbers versus processing 1000 numbers is small which implies that the cost of the actual work being done in these cases is insignicant compared to the overhead cost. This also implies that only the 100k integer input file can be used to compare the different programs running with different number of processes.