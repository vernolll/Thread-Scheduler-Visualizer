#include "core/MetricsCalculator.h"
#include <numeric>

namespace core
{
    SimulationMetrics MetricsCalculator::calculate(
        const std::vector<std::shared_ptr<Thread>>& threads,
        const std::vector<SimulationEvent>& events,
        uint64_t totalTicks)
    {
        SimulationMetrics metrics;
        if (threads.empty()) return metrics;

        std::unordered_map<uint32_t, uint32_t> turnaroundMap;
        std::unordered_map<uint32_t, uint32_t> waitingMap;
        std::unordered_map<uint32_t, uint32_t> responseMap;
        std::unordered_map<uint32_t, uint64_t> lastReadyTimestamp;
        std::unordered_map<uint32_t, bool> firstRunRecorded;

        for (const auto& event : events)
        {
            if (event.newState == ThreadState::READY)
            {
                lastReadyTimestamp[event.threadId] = event.tick;
            }
            else if (event.newState == ThreadState::RUNNING)
            {
                if (!firstRunRecorded[event.threadId])
                {
                    responseMap[event.threadId] = static_cast<uint32_t>(event.tick);
                    firstRunRecorded[event.threadId] = true;
                }
            }
            else if (event.newState == ThreadState::TERMINATED)
            {
                turnaroundMap[event.threadId] = static_cast<uint32_t>(event.tick);
            }
        }

        double totalTurnaround = 0;
        double totalWaiting = 0;
        double totalResponse = 0;

        for (const auto& thread : threads)
        {
            ThreadMetrics tm;
            tm.threadId = thread->id;
            tm.turnaroundTime = turnaroundMap.count(thread->id) ? turnaroundMap[thread->id] : 0;
            tm.waitingTime = thread->waitingTime;
            tm.responseTime = responseMap.count(thread->id) ? responseMap[thread->id] : 0;

            totalTurnaround += tm.turnaroundTime;
            totalWaiting += tm.waitingTime;
            totalResponse += tm.responseTime;

            metrics.perThreadMetrics.push_back(tm);
        }

        size_t count = threads.size();
        metrics.averageTurnaroundTime = count > 0 ? totalTurnaround / count : 0.0;
        metrics.averageWaitingTime = count > 0 ? totalWaiting / count : 0.0;
        metrics.averageResponseTime = count > 0 ? totalResponse / count : 0.0;

        return metrics;
    }
}