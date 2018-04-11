# CPU Scheduling

## Description
<p>
The program simulates the execution of processes of a tablet having multiple cores, 1 SSD and 1 display (input).
Time is simulated using an event based simulation system where each event denotes when a process has started and
ended depending on the number of requests completed. At each start/termination of a process, the number of
RUNNING, READY and BLOCKED processes are printed. At the end of the simulation, a summary of data is displayed.
The data includes: Number of processes completed, Total number of SSD accesses, Average SSD access time (rounded to 2 DP),
Total elapsed time, Core utilization (rounded to 2 DP), SSD utiliazation (rounded to 2 DP).
</p>
