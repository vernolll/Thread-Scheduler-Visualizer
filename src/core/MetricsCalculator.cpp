#include "core/MetricsCalculator.h"
#include <numeric>
#include <cmath>
#include <algorithm>

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
        std::unordered_map<uint32_t, uint32_t> responseMap;
        std::unordered_map<uint32_t, bool> firstRunRecorded;

        uint32_t contextSwitchesCount = 0;
        uint32_t lastRunningThreadId = 0;
        bool hasRunningThread = false;

        for (const auto& event : events)
        {
            if (event.newState == ThreadState::RUNNING)
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

        std::vector<double> slowdownFactors;
        slowdownFactors.reserve(threads.size());

        for (const auto& thread : threads)
        {
            ThreadMetrics tm;
            tm.threadId = thread->id;

            uint32_t completionTick = turnaroundMap.count(thread->id) ? turnaroundMap[thread->id] : static_cast<uint32_t>(totalTicks);
            if (completionTick >= thread->arrivalTime) 
            {
                tm.turnaroundTime = completionTick - thread->arrivalTime;
            }
            else 
            {
                tm.turnaroundTime = completionTick;
            }

            int calculatedWaiting = static_cast<int>(tm.turnaroundTime) - static_cast<int>(thread->burstTime);
            tm.waitingTime = (calculatedWaiting > 0) ? static_cast<uint32_t>(calculatedWaiting) : 0U;

            if (responseMap.count(thread->id)) {
                uint32_t firstRunTick = responseMap[thread->id];
                tm.responseTime = (firstRunTick >= thread->arrivalTime) ? (firstRunTick - thread->arrivalTime) : firstRunTick;
            }
            else 
            {
                tm.responseTime = 0;
            }

            if (thread->state == ThreadState::TERMINATED || turnaroundMap.count(thread->id))
            {
                completedCount++;
            }

            totalTurnaround += tm.turnaroundTime;
            totalWaiting += tm.waitingTime;
            totalResponse += tm.responseTime;

            metrics.perThreadMetrics.push_back(tm);

            double burst = thread->burstTime > 0 ? static_cast<double>(thread->burstTime) : 1.0;
            double slowdown = static_cast<double>(tm.turnaroundTime) / burst;
            slowdownFactors.push_back(slowdown > 0.0 ? (1.0 / slowdown) : 1.0);
        }

        size_t count = threads.size();
        metrics.averageTurnaroundTime = count > 0 ? totalTurnaround / count : 0.0;
        metrics.averageWaitingTime = count > 0 ? totalWaiting / count : 0.0;
        metrics.averageResponseTime = count > 0 ? totalResponse / count : 0.0;

        if (totalTicks > 0)
        {
            metrics.throughput = static_cast<double>(completedCount) / static_cast<double>(totalTicks);
        }

        double sum = 0.0;
        double sumSq = 0.0;

        for (double val : slowdownFactors)
        {
            sum += val;
            sumSq += (val * val);
        }

        double n = static_cast<double>(slowdownFactors.size());
        if (n > 0.0 && sumSq > 0.0)
        {
            metrics.jainsFairnessIndex = (sum * sum) / (n * sumSq);
        }

        return metrics;
    }
}