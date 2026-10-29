#ifndef BENCHMARK_RUNNER_H
#define BENCHMARK_RUNNER_H

#include <vector>
#include <memory>
#include <string>
#include "core/Thread.h"
#include "core/SchedulerTypes.h"
#include "core/MetricsCalculator.h"

namespace core
{
    struct BenchmarkResult
    {
        SchedulerType schedulerType;
        std::string schedulerName;
        SimulationMetrics metrics;
        uint64_t totalTicks{ 0 };
        bool success{ false };
    };

    class BenchmarkRunner
    {
    public:
        static std::vector<BenchmarkResult> runBenchmark(
            const std::vector<std::shared_ptr<Thread>>& baselineThreads
        );
    };
}

#endif // BENCHMARK_RUNNER_H