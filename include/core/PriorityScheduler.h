#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "core/Scheduler.h"
#include <vector>
#include <memory>

namespace core
{
    class PriorityScheduler : public Scheduler
    {
    public:
        explicit PriorityScheduler(bool preemptive = true);
        ~PriorityScheduler() override = default;

        void addThread(std::shared_ptr<Thread> thread) override;
        void tick() override;
        std::shared_ptr<Thread> getCurrentThread() const override;
        bool isFinished() const override;
        const std::vector<std::shared_ptr<Thread>>& getAllThreads() const override;

        bool isPreemptive() const { return m_preemptive; }
        void setPreemptive(bool preemptive) { m_preemptive = preemptive; }

        const std::vector<std::shared_ptr<Thread>>& getWaitingThreads() const { return m_waitingThreads; }

    private:
        bool m_preemptive{ true };
        std::vector<std::shared_ptr<Thread>> m_readyQueue;
        std::vector<std::shared_ptr<Thread>> m_waitingThreads;
        std::shared_ptr<Thread> m_currentThread{ nullptr };

        void sortReadyQueue();
    };
}

#endif // PRIORITY_SCHEDULER_H