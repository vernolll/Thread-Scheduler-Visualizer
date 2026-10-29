#ifndef ISCHEDULER_H
#define ISCHEDULER_H

#include "core/Thread.h"
#include <memory>
#include <vector>

namespace core
{
    class IScheduler
    {
    public:
        virtual ~IScheduler() = default;

        virtual void addThread(std::shared_ptr<Thread> thread) = 0;
        virtual void tick() = 0;
        virtual std::shared_ptr<Thread> getCurrentThread() const = 0;
        virtual bool isFinished() const = 0;
        virtual const std::vector<std::shared_ptr<Thread>>& getAllThreads() const = 0;

        virtual void setContextSwitchOverhead(uint64_t ticks) = 0;
        virtual uint64_t getContextSwitchOverhead() const = 0;
    };
}

#endif // ISCHEDULER_H