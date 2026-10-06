# Glossary

This document outlines key technical terms and concepts used throughout the **Thread Scheduler Visualizer** project.

---

## Core Concepts

### Thread
An execution context managed by the scheduler. Each thread contains an identifier, priority level, total burst duration, remaining execution time, and current lifecycle state.

### Process Control Block (PCB)
A data structure maintained by the operating system kernel that holds operational metadata for a specific process or thread, including CPU state, scheduling information, and resource handles.

### Thread State
The current phase of a thread in its execution lifecycle:
* **NEW**: Thread has been instantiated but not yet scheduled.
* **READY**: Thread is waiting in the queue to be assigned to a CPU core.
* **RUNNING**: Thread is actively executing instructions on the CPU core.
* **BLOCKED**: Thread is suspended waiting for an I/O operation or resource lock.
* **TERMINATED**: Thread execution is complete.

---

## Scheduling Concepts

### Scheduling Algorithm
The policy used by the scheduler to decide which ready thread is allocated CPU execution time next.

### First-Come, First-Served (FCFS)
A non-preemptive scheduling algorithm that executes threads strictly in the order of their arrival in the ready queue (FIFO).

### Round Robin (RR)
A preemptive algorithm where each thread is assigned a fixed time slice (time quantum) in cyclic order.

### Priority Scheduling
An algorithm where CPU execution time is allocated based on assigned thread priority ranks. Can operate in preemptive or non-preemptive mode.

### Time Quantum (Time Slice)
The maximum continuous amount of execution time allocated to a thread before it is preempted in Round Robin scheduling.

### Preemption
The act of temporarily interrupting an actively executing thread on the CPU without its cooperation to assign execution time to another thread.

### Tick
The fundamental unit of discrete simulation time. In each tick, the active scheduler advances the execution state of the currently running thread by decremental time steps.

### Time Quantum (Time Slice)
The maximum continuous amount of execution time (measured in ticks) allocated to a thread before it is preempted in Round Robin scheduling.

### Round Robin (RR)
A preemptive scheduling algorithm that assigns a fixed time quantum to each thread. Threads execute in a circular FIFO queue; if a thread's quantum expires before completion, it is preempted and moved to the back of the ready queue.

---

## Architecture & Frameworks

### Core Engine
The pure C++17 domain backend containing data models (`Thread`, `PCB`) and scheduling algorithms (`Scheduler`, `FCFSScheduler`). Independent of UI components.

### Qt Quick / QML
Declarative UI technology used for rendering animated components, control panels, tables, and Gantt charts bound directly to C++ models.