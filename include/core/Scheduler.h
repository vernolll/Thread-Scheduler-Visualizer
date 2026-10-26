#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "core/Thread.h"
#include "core/EventLogger.h"
#include <vector>
#include <memory>

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

        const EventLogger& getEventLogger() const { return m_eventLogger; }

        uint64_t getCurrentTick() const { return m_currentTick; }

    protected:
        std::vector<std::shared_ptr<Thread>> m_allThreads;
        EventLogger m_eventLogger;
        uint64_t m_currentTick{ 0 };

        void changeThreadState(std::shared_ptr<Thread>& thread, ThreadState newState, const std::string& reason = "")
        {
            if (!thread) return;
            ThreadState oldState = thread->state;
            if (oldState != newState)
            {
                if (thread->setState(newState))
                {
                    m_eventLogger.logEvent(m_currentTick, thread->id, oldState, newState, reason);
                }
            }
        }
    };
}

#endif // SCHEDULER_H