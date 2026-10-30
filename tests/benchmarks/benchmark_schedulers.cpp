#include <benchmark/benchmark.h>
#include "core/SchedulerFactory.h"
#include "core/SchedulerTypes.h"
#include "core/Thread.h"
#include <memory>
#include <vector>

static void BM_SchedulerSimulation(benchmark::State& state, core::SchedulerType type) 
{
    const int threadCount = state.range(0);

    for (auto _ : state) 
    {
        state.PauseTiming();
        auto scheduler = core::SchedulerFactory::createScheduler(type);

        std::vector<std::shared_ptr<core::Thread>> threads;
        threads.reserve(threadCount);
        for (int i = 0; i < threadCount; ++i) 
        {
            uint32_t burst = 1 + (i % 50);
            uint32_t priority = i % 5;
            threads.push_back(std::make_shared<core::Thread>(static_cast<uint32_t>(i), burst, priority));
        }

        for (const auto& th : threads) 
        {
            scheduler->addThread(th);
        }

        state.ResumeTiming();

        int ticks = 0;

        const int maxTicks = threadCount * 5;
        while (ticks < maxTicks) 
        {
            scheduler->tick();
            ticks++;
        }
    }
}

static void CustomArguments(benchmark::internal::Benchmark* b) 
{
    for (int threads = 1000; threads <= 10000; threads += 3000) 
    {
        b->Args({ threads });
    }
}

BENCHMARK_CAPTURE(BM_SchedulerSimulation, FCFS, core::SchedulerType::FCFS)
->Apply(CustomArguments)
->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_SchedulerSimulation, RoundRobin, core::SchedulerType::RoundRobin)
->Apply(CustomArguments)
->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_SchedulerSimulation, Priority, core::SchedulerType::Priority)
->Apply(CustomArguments)
->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();