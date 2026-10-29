#include "core/MetricsCalculator.h"
#include <numeric>
#include <cmath>

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

        uint32_t contextSwitchesCount = 0;
        uint32_t lastRunningThreadId = 0;
        bool hasRunningThread = false;

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

                if (hasRunningThread && lastRunningThreadId != event.threadId)
                {
                    contextSwitchesCount++;
                }
                lastRunningThreadId = event.threadId;
                hasRunningThread = true;
            }
            else if (event.newState == ThreadState::TERMINATED)
            {
                turnaroundMap[event.threadId] = static_cast<uint32_t>(event.tick);
                if (lastRunningThreadId == event.threadId)
                {
                    hasRunningThread = false;
                }
            }
        }

        metrics.contextSwitches = contextSwitchesCount;

        double totalTurnaround = 0;
        double totalWaiting = 0;
        double totalResponse = 0;
        size_t completedCount = 0;

        std::vector<double> cpuAllocations;
        cpuAllocations.reserve(threads.size());

        for (const auto& thread : threads)
        {
            ThreadMetrics tm;
            tm.threadId = thread->id;
            tm.turnaroundTime = turnaroundMap.count(thread->id) ? turnaroundMap[thread->id] : 0;
            tm.waitingTime = thread->waitingTime;
            tm.responseTime = responseMap.count(thread->id) ? responseMap[thread->id] : 0;

            if (thread->state == ThreadState::TERMINATED || turnaroundMap.count(thread->id))
            {
                completedCount++;
            }

            totalTurnaround += tm.turnaroundTime;
            totalWaiting += tm.waitingTime;
            totalResponse += tm.responseTime;

            metrics.perThreadMetrics.push_back(tm);
            cpuAllocations.push_back(static_cast<double>(thread->burstTime));
        }

        size_t count = threads.size();
        metrics.averageTurnaroundTime = count > 0 ? totalTurnaround / count : 0.0;
        metrics.averageWaitingTime = count > 0 ? totalWaiting / count : 0.0;
        metrics.averageResponseTime = count > 0 ? totalResponse / count : 0.0;

        if (totalTicks > 0)
        {
            metrics.throughput = static_cast<double>(completedCount) / static_cast<double>(totalTicks);
        }

        double sumAllocations = 0.0;
        double sumSquaredAllocations = 0.0;

        for (double x : cpuAllocations)
        {
            sumAllocations += x;
            sumSquaredAllocations += (x * x);
        }

        double n = static_cast<double>(cpuAllocations.size());
        if (n > 0.0 && sumSquaredAllocations > 0.0)
        {
            metrics.jainsFairnessIndex = (sumAllocations * sumAllocations) / (n * sumSquaredAllocations);
        }

        return metrics;
    }
}