#include "core/SchedulerFactory.h"
#include "core/FCFSScheduler.h"
#include "core/RRScheduler.h"
#include "core/PriorityScheduler.h"
#include <stdexcept>

namespace core
{
    std::unique_ptr<Scheduler> SchedulerFactory::createScheduler(SchedulerType type)
    {
        switch (type)
        {
        case SchedulerType::FCFS:
            return std::make_unique<FCFSScheduler>();
        case SchedulerType::RoundRobin:
            return std::make_unique<RRScheduler>();
        case SchedulerType::Priority:
            return std::make_unique<PriorityScheduler>();
        default:
            throw std::invalid_argument("Unknown or unsupported scheduler type in factory");
        }
    }
}