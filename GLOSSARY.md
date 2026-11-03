# Glossary

This document outlines key technical terms, scheduling policies, and architectural concepts used throughout the **Thread Scheduler Visualizer** project[cite: 2].

---

## Core Concepts

### Thread
An execution context managed by the scheduler. Each thread contains an identifier, priority level, total burst duration, remaining execution time, and current lifecycle state[cite: 2].

### Process Control Block (PCB)
A data structure maintained by the operating system kernel that holds operational metadata for a specific process or thread, including CPU state, scheduling information, and resource handles[cite: 2].

### Thread State
The current phase of a thread in its execution lifecycle:
* **NEW**: Thread has been instantiated but not yet scheduled[cite: 2].
* **READY**: Thread is waiting in the ready queue to be assigned to a CPU core[cite: 2].
* **RUNNING**: Thread is actively executing instructions on the CPU core[cite: 2].
* **WAITING / BLOCKED**: Thread is suspended waiting for an I/O operation, event, or resource lock[cite: 2].
* **TERMINATED**: Thread execution is complete[cite: 2].

---

## Scheduling Algorithms & Policies

### Scheduling Algorithm
The policy used by the scheduler to decide which ready thread is allocated CPU execution time next[cite: 2].

### First-Come, First-Served (FCFS)
A non-preemptive scheduling algorithm that executes threads strictly in the order of their arrival in the ready queue (FIFO)[cite: 2].

### Round Robin (RR)
A preemptive scheduling algorithm that assigns a fixed time quantum to each thread[cite: 2]. Threads execute in a circular FIFO queue; if a thread's quantum expires before completion, it is preempted and moved to the back of the ready queue[cite: 2].

### Shortest Job First (SJF)
A non-preemptive scheduling algorithm that selects the waiting thread with the smallest total CPU burst time for execution next.

### Shortest Remaining Time First (SRTF)
The preemptive version of SJF. If a new thread arrives with a remaining burst time shorter than the currently running thread's remaining execution time, the current thread is preempted.

### Priority Scheduling
An algorithm where CPU execution time is allocated based on assigned thread priority ranks. Can operate in preemptive or non-preemptive mode.

### Aging
A technique used in priority scheduling to prevent resource starvation. The priority of waiting threads is incrementally increased the longer they stay in the ready queue, ensuring they eventually get CPU time.

### Multi-Level Feedback Queue (MLFQ)
An advanced scheduling algorithm that maintains multiple ready queues with different priority levels and quantum sizes. It allows threads to move between queues based on their CPU usage and behavior (e.g., demoting CPU-bound threads and promoting interactive I/O-bound threads).

---

## Performance & Execution Concepts

### Time Quantum (Time Slice)
The maximum continuous amount of execution time (measured in ticks) allocated to a thread before it is preempted in Round Robin or similar preemptive scheduling policies[cite: 2].

### Preemption
The act of temporarily interrupting an actively executing thread on the CPU without its cooperation to assign execution time to another thread[cite: 2].

### Tick
The fundamental unit of discrete simulation time. In each tick, the active scheduler advances the execution state of the currently running thread by decremental time steps[cite: 2].

### Context Switch Overhead
The time or resource cost consumed by the system when switching the CPU from executing one thread to another (saving and restoring registers and PCB states).

### Throughput
A performance metric representing the total number of completed threads or tasks finished per unit of simulation time.

### Jain's Fairness Index
A fairness metric used to evaluate how equitably CPU resources are distributed among all competing threads, ranging from $1/n$ (worst case, completely unfair) to $1.0$ (perfect fairness).

---

## Architecture & Frameworks

### Core Engine
The pure C++17 domain backend containing data models (`Thread`, `PCB`) and scheduling algorithms (`Scheduler`, `FCFSScheduler`, `MLFQScheduler`). Independent of UI components[cite: 2].

### Qt Quick / QML
Declarative UI technology used for rendering animated components, control panels, comparison tables, and Gantt charts bound directly to C++ models[cite: 2].