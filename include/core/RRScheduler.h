#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include "core/Scheduler.h"
#include <queue>

namespace core 
{
    class RRScheduler : public Scheduler 
    {
    public:
        explicit RRScheduler(uint32_t timeQuantum = 2);
        ~RRScheduler() override = default;

        void addThread(std::shared_ptr<Thread> thread) override;
        void tick() override;
        std::shared_ptr<Thread> getCurrentThread() const override;
        bool isFinished() const override;
        const std::vector<std::shared_ptr<Thread>>& getAllThreads() const override;

        uint32_t getTimeQuantum() const { return m_timeQuantum; }
        void setTimeQuantum(uint32_t quantum) { m_timeQuantum = quantum; }

    private:
        uint32_t m_timeQuantum{ 2 };
        uint32_t m_currentQuantumTicks{ 0 };
        std::queue<std::shared_ptr<Thread>> m_readyQueue;
        std::shared_ptr<Thread> m_currentThread{ nullptr };
    };
}

#endif // RR_SCHEDULER_H