#ifndef THREAD_H
#define THREAD_H

#include <string>

namespace core 
{
    enum class ThreadState 
    {
        NEW,
        READY,
        RUNNING,
        BLOCKED,
        TERMINATED
    };

    std::string toString(ThreadState state);

    struct Thread 
    {
        uint32_t id{ 0 };
        uint32_t processId{ 0 };
        uint32_t priority{ 0 };

        uint32_t arrivalTime{ 0 };
        uint32_t burstTime{ 0 };
        uint32_t remainingTime{ 0 };
        uint32_t waitingTime{ 0 };
        uint32_t turnaroundTime{ 0 };

        ThreadState state{ ThreadState::NEW };

        Thread() = default;

        Thread(uint32_t threadId, uint32_t burst, uint32_t prio = 0)
            : id(threadId), burstTime(burst), priority(prio), remainingTime(burst), state(ThreadState::NEW) 
        {
        }
    };

}

#endif // THREAD_H