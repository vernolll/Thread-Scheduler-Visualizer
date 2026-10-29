#ifndef MLFQ_SCHEDULER_H
#define MLFQ_SCHEDULER_H

#include "core/Scheduler.h"
#include <vector>
#include <memory>

namespace core
{
    struct MLFQLevel
    {
        uint32_t timeQuantum;
        std::vector<std::shared_ptr<Thread>> queue;
    };

    class MLFQScheduler : public Scheduler
    {
    public:
        MLFQScheduler(size_t numLevels = 3, uint32_t boostInterval = 50);
        ~MLFQScheduler() override = default;

        void addThread(std::shared_ptr<Thread> thread) override;
        void tick() override;
        std::shared_ptr<Thread> getCurrentThread() const override;
        bool isFinished() const override;
        const std::vector<std::shared_ptr<Thread>>& getAllThreads() const override;

    private:
        std::vector<MLFQLevel> m_levels;
        std::shared_ptr<Thread> m_currentThread{ nullptr };
        size_t m_currentLevel{ 0 };
        uint32_t m_currentQuantumUsed{ 0 };
        uint32_t m_boostInterval;
        uint32_t m_ticksSinceBoost{ 0 };

        void performBoost();
        std::pair<size_t, std::shared_ptr<Thread>> findNextThread();
    };
}

#endif // MLFQ_SCHEDULER_H