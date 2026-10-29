#ifndef SCHEDULER_TYPES_H
#define SCHEDULER_TYPES_H

namespace core
{
    enum class SchedulerType
    {
        FCFS,
        RoundRobin,
        Priority,
        SJF,
        SRTF,
        MLFQ
    };
}

#endif // SCHEDULER_TYPES_H