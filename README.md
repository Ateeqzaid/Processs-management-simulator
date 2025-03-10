            Process Management Simulator (Documentation)

I created a c++ program called process_simulator which simulates short term CPU scheduling in an operating system. The

following scheduling algorithms are used. There is a context switch time of 14 milliseconds.

 

   1) First-Come, First-Served (FCFS)

   2) Shortest-Job-First (SJF), with no preemption

   3) Preemptive Shortest-Job-First (SJF), with preemption

   4) Round-Robin (RR), with configurable time slice t initially set to 20 milliseconds    

  5) Preemptive Priority, with random priority levels 0-4 assigned to processes at the onset (low numbers indicate high priority); processes with the same priority are processed via RR; higher-priority processes entering the system preempt a running process

 

Each process has a randomely assigned arrival time and a random CPU burst time in the range of 500 - 4000 ms.

 

While I tested my project using multiple values for the number of my processes (variable num_processes), I did most testing with n = 20. 80% of the processes will NOT begin at time 0. Exponential Distribution was used to generate the arrival times

in order for the simulation to feel more natural.

 

Timeslices for round robin and priority scheduling can be modified by changing the variable int timeslice.

 

NOTE: No processes are actually being forked, this is purely just a simulation of the scheduling algorithms.
