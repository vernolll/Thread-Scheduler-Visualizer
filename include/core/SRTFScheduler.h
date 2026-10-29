#ifndef SRTF_SCHEDULER_H
#define SRTF_SCHEDULER_H

#include "core/Scheduler.h"
#include <vector>
#include <memory>

namespace core
{
    class SRTFScheduler : public Scheduler
    {
    public:
        SRTFScheduler() = default;
        ~SRTFScheduler() override = default;

        void addThread(std::shared_ptr<Thread> thread) override;
        void tick() override;
        std::shared_ptr<Thread> getCurrentThread() const override;
        bool isFinished() const override;
        const std::vector<std::shared_ptr<Thread>>& getAllThreads() const override;

    private:
        std::vector<std::shared_ptr<Thread>> m_readyQueue;
        std::shared_ptr<Thread> m_currentThread{ nullptr };

        void sortReadyQueue();
    };
}

#endif // SRTF_SCHEDULER_H