#ifndef METRICS_CALCULATOR_H
#define METRICS_CALCULATOR_H

#include "core/Thread.h"
#include "core/SimulationEvent.h"
#include <vector>
#include <memory>
#include <unordered_map>

namespace core
{
    struct ThreadMetrics
    {
        uint32_t threadId{ 0 };
        uint32_t turnaroundTime{ 0 };
        uint32_t waitingTime{ 0 };
        uint32_t responseTime{ 0 };
    };

    struct SimulationMetrics
    {
        std::vector<ThreadMetrics> perThreadMetrics;
        double averageTurnaroundTime{ 0.0 };
        double averageWaitingTime{ 0.0 };
        double averageResponseTime{ 0.0 };
        double cpuUtilization{ 0.0 };
        double throughput{ 0.0 };
        uint32_t contextSwitches{ 0 };
        double jainsFairnessIndex{ 1.0 };
    };

    class MetricsCalculator
    {
    public:
        static SimulationMetrics calculate(
            const std::vector<std::shared_ptr<Thread>>& threads,
            const std::vector<SimulationEvent>& events,
            uint64_t totalTicks
        );
    };
}

#endif // METRICS_CALCULATOR_H