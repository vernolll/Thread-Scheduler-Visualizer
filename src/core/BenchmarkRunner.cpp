#include "core/BenchmarkRunner.h"
#include "core/SchedulerFactory.h"
#include "core/SimulationEvent.h"
#include <unordered_map>

namespace core
{
    static std::string getSchedulerName(SchedulerType type)
    {
        switch (type)
        {
        case SchedulerType::FCFS: return "FCFS";
        case SchedulerType::RoundRobin: return "Round Robin";
        case SchedulerType::Priority: return "Priority";
        case SchedulerType::SJF: return "SJF";
        case SchedulerType::SRTF: return "SRTF";
        case SchedulerType::MLFQ: return "MLFQ";
        default: return "Unknown";
        }
    }

    static std::vector<std::shared_ptr<Thread>> cloneThreads(const std::vector<std::shared_ptr<Thread>>& original)
    {
        std::vector<std::shared_ptr<Thread>> copies;
        copies.reserve(original.size());
        for (const auto& th : original)
        {
            if (!th) continue;
            auto copy = std::make_shared<Thread>(th->id, th->burstTime, th->priority);
            copy->arrivalTime = th->arrivalTime;
            copy->state = ThreadState::NEW; // Сбрасываем в начальное состояние
            copies.push_back(copy);
        }
        return copies;
    }

    std::vector<BenchmarkResult> BenchmarkRunner::runBenchmark(
        const std::vector<std::shared_ptr<Thread>>& baselineThreads)
    {
        std::vector<BenchmarkResult> results;

        std::vector<SchedulerType> types = {
            SchedulerType::FCFS,
            SchedulerType::RoundRobin,
            SchedulerType::Priority,
            SchedulerType::SJF,
            SchedulerType::SRTF,
            SchedulerType::MLFQ
        };

        for (SchedulerType type : types)
        {
            BenchmarkResult res;
            res.schedulerType = type;
            res.schedulerName = getSchedulerName(type);

            auto threadsCopy = cloneThreads(baselineThreads);
            auto scheduler = SchedulerFactory::createScheduler(type);

            if (!scheduler)
            {
                res.success = false;
                results.push_back(res);
                continue;
            }

            for (const auto& th : threadsCopy)
            {
                scheduler->addThread(th);
            }

            std::vector<SimulationEvent> events;
            uint64_t ticks = 0;
            const uint64_t maxTicks = 10000;

            uint32_t lastRunningThreadId = 0;
            bool hasRunningBefore = false;

            while (!scheduler->isFinished() && ticks < maxTicks)
            {
                scheduler->tick();
                ticks++;

                auto currThread = scheduler->getCurrentThread();
                if (currThread)
                {
                    if (hasRunningBefore && lastRunningThreadId != currThread->id)
                    {
                        events.push_back(SimulationEvent(ticks, currThread->id, ThreadState::RUNNING, ThreadState::RUNNING, "Context Switch"));
                    }
                    lastRunningThreadId = currThread->id;
                    hasRunningBefore = true;
                }

                for (const auto& th : scheduler->getAllThreads())
                {
                    if (th->state == ThreadState::TERMINATED)
                    {
                        bool alreadyTerminated = false;
                        for (const auto& ev : events) {
                            if (ev.threadId == th->id && ev.newState == ThreadState::TERMINATED) 
                            {
                                alreadyTerminated = true;
                                break;
                            }
                        }
                        if (!alreadyTerminated) {
                            events.push_back(SimulationEvent(ticks, th->id, ThreadState::RUNNING, ThreadState::TERMINATED, "Terminated"));
                        }
                    }
                }
            }

            res.totalTicks = ticks;
            res.success = scheduler->isFinished();

            // Пересчитываем метрики с собранными событиями
            res.metrics = MetricsCalculator::calculate(threadsCopy, events, ticks);

            results.push_back(res);
        }

        return results;
    }
}