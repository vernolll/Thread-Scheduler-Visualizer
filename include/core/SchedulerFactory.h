#ifndef SCHEDULER_FACTORY_H
#define SCHEDULER_FACTORY_H

#include "core/Scheduler.h"
#include "core/SchedulerTypes.h"
#include <memory>

namespace core
{
    class SchedulerFactory
    {
    public:
        static std::unique_ptr<Scheduler> createScheduler(SchedulerType type);
    };
}

#endif // SCHEDULER_FACTORY_H