#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "core/Scheduler.h"
#include <queue>
#include <vector>

namespace core
{

    class FCFSScheduler : public Scheduler
    {
    public:
        FCFSScheduler() = default;
        ~FCFSScheduler() override = default;

        void addThread(std::shared_ptr<Thread> thread) override;
        void tick() override;
        std::shared_ptr<Thread> getCurrentThread() const override;
        bool isFinished() const override;
        const std::vector<std::shared_ptr<Thread>>& getAllThreads() const override;
        const std::vector<std::shared_ptr<Thread>>& getWaitingThreads() const { return m_waitingThreads; }

    private:
        std::queue<std::shared_ptr<Thread>> m_readyQueue;
        std::vector<std::shared_ptr<Thread>> m_waitingThreads;
        std::shared_ptr<Thread> m_currentThread{ nullptr };
    };
}

#endif // FCFS_SCHEDULER_H