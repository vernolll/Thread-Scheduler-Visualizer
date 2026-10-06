#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "core/Thread.h"
#include <vector>
#include <memory>
#include <queue>

namespace core 
{

    enum class AlgorithmType 
    {
        FCFS,
        ROUND_ROBIN,
        PRIORITY
    };

    class Scheduler 
    {
    public:
        virtual ~Scheduler() = default;

        virtual void addThread(std::shared_ptr<Thread> thread) = 0;

        virtual void tick() = 0;

        virtual std::shared_ptr<Thread> getCurrentThread() const = 0;

        virtual bool isFinished() const = 0;

        virtual const std::vector<std::shared_ptr<Thread>>& getAllThreads() const = 0;

    protected:
        std::vector<std::shared_ptr<Thread>> m_allThreads;
    };

}

#endif // SCHEDULER_H