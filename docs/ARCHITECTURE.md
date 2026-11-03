# Architecture Documentation

This document provides a detailed overview of the system architecture, component interactions, and state machines for the **Thread Scheduler Visualizer** project.

---

## 1. Class Diagram (Core Engine & Data Models)

The following diagram illustrates the relationship between the polymorphic scheduler interfaces, concrete scheduling algorithm implementations, and underlying domain entities (`Thread`, `PCB`).

```mermaid
classDiagram
    class IScheduler {
        <<interface>>
        +addThread(Thread* thread)*
        +getNextThread()* Thread*
        +update(int ticks)*
        +reset()*
        +getReadyQueue()* vector~Thread*~
    }
    
    class FCFSScheduler {
        -queue~Thread*~ readyQueue
        +addThread(Thread* thread)
        +getNextThread() Thread*
        +update(int ticks)
    }
    
    class RRScheduler {
        -queue~Thread*~ readyQueue
        -int timeQuantum
        -int currentQuantumTicks
        +addThread(Thread* thread)
        +getNextThread() Thread*
        +update(int ticks)
    }

    class PriorityScheduler {
        -vector~Thread*~ readyQueue
        -bool enableAging
        -int agingThreshold
        +addThread(Thread* thread)
        +getNextThread() Thread*
        +update(int ticks)
    }

    class MLFQScheduler {
        -vector~queue~Thread*~~ queues
        -vector~int~ quantums
        +addThread(Thread* thread)
        +getNextThread() Thread*
        +update(int ticks)
    }

    IScheduler <|.. FCFSScheduler
    IScheduler <|.. RRScheduler
    IScheduler <|.. PriorityScheduler
    IScheduler <|.. MLFQScheduler

    class Thread {
        +int id
        +int priority
        +int burstTime
        +int remainingTime
        +int arrivalTime
        +ThreadState state
    }

    class PCB {
        +int processId
        +Thread* thread
        +int contextSwitchCount
        +int totalWaitTime
    }

    IScheduler --> Thread : manages
    Thread --> PCB : linked via metadata
```

---

## 2. Sequence Diagram (Simulation Execution Lifecycle)

The sequence diagram below highlights how the `GUIController` delegates execution ticks to the worker thread (`SimulationWorker`), updating scheduling mathematics and notifying QML view models.

```mermaid
sequenceDiagram
    autonumber
    participant UI as GUIController / QML
    participant Worker as SimulationWorker
    participant Scheduler as IScheduler Implementation
    participant Metrics as MetricsCalculator

    UI->>Worker: startSimulation()
    activate Worker
    loop For Every Discrete Simulation Tick
        Worker->>Scheduler: update(currentTick)
        Worker->>Scheduler: getNextThread()
        Scheduler-->>Worker: activeThread (or nullptr if IDLE)
        
        alt Active Thread Running
            Worker->>Worker: executeThreadTick(activeThread)
        end
        
        Worker->>Metrics: recordTickData(activeThread, readyQueueState)
        Worker-->>UI: emit tickCompleted(currentTick)
        UI->>UI: Refresh GanttChartModel & ReadyQueueModel
    end
    Worker-->>UI: emit simulationFinished()
    deactivate Worker
```

---

## 3. State Diagram (Thread Lifecycle FSM)

This diagram documents the formal Finite State Machine (FSM) transitions experienced by every thread managed within the scheduling system.

```mermaid
stateDiagram-v2
    [*] --> NEW : Thread Instantiation
    NEW --> READY : Arrives in Ready Queue
    READY --> RUNNING : Dispatched by Scheduler
    
    RUNNING --> READY : Time Quantum Expired / Preempted
    RUNNING --> WAITING : I/O Lock or Wait Requested
    WAITING --> READY : I/O Completion Signal
    
    RUNNING --> TERMINATED : Remaining Burst Reaches 0
    TERMINATED --> [*]
```

---

## 4. Layering & Separation of Concerns

The project adheres strictly to a clean separation between Core Engine logic and GUI View Models:

1. **Core Domain Layer (`include/core/`, `src/core/`)**: Independent of any Qt framework dependencies. Contains mathematical algorithms, scheduling policies, and metric collectors.
2. **Adapter & View Model Layer (`include/gui/`, `src/gui/`)**: Implements `QAbstractListModel` wrappers (`GanttChartModel`, `ReadyQueueModel`, `ThreadListModel`) and bridges C++ signals to QML slots.
3. **Declarative UI Layer (`src/qml/`)**: Reactive QML views and visual components for rendering comparative graphs, Gantt bars, and state diagrams.