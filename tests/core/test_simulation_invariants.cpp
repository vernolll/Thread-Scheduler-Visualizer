#include <gtest/gtest.h>
#include "core/SchedulerFactory.h"
#include "core/SchedulerTypes.h"
#include "core/Thread.h"
#include <memory>
#include <vector>
#include <algorithm>

class SimulationInvariantTest : public ::testing::TestWithParam<core::SchedulerType> {};

TEST_P(SimulationInvariantTest, CoreInvariantsHold)
{
    core::SchedulerType type = GetParam();
    auto scheduler = core::SchedulerFactory::createScheduler(type);
    ASSERT_NE(scheduler, nullptr);

    std::vector<std::shared_ptr<core::Thread>> threads =
    {
        std::make_shared<core::Thread>(1u, 4u, 2u),
        std::make_shared<core::Thread>(2u, 2u, 1u),
        std::make_shared<core::Thread>(3u, 6u, 3u),
        std::make_shared<core::Thread>(4u, 3u, 0u)
    };

    uint32_t totalExpectedBurst = 0;
    for (const auto& th : threads)
    {
        totalExpectedBurst += th->burstTime;
        scheduler->addThread(th);
    }

    int ticks = 0;
    const int maxTicks = 1000;

    while (!scheduler->isFinished() && ticks < maxTicks)
    {
        auto currentThread = scheduler->getCurrentThread();
        if (currentThread != nullptr)
        {
            EXPECT_EQ(currentThread->state, core::ThreadState::RUNNING);
        }

        scheduler->tick();
        ticks++;
    }

    EXPECT_TRUE(scheduler->isFinished()) << "Simulation timed out for scheduler type: " << static_cast<int>(type);

    for (const auto& th : scheduler->getAllThreads())
    {
        EXPECT_EQ(th->state, core::ThreadState::TERMINATED);
    }

    EXPECT_GE(ticks, totalExpectedBurst);
}

INSTANTIATE_TEST_SUITE_P(
    InvariantTestAllSchedulers,
    SimulationInvariantTest,
    ::testing::Values(
        core::SchedulerType::FCFS,
        core::SchedulerType::RoundRobin,
        core::SchedulerType::Priority,
        core::SchedulerType::SJF,
        core::SchedulerType::SRTF,
        core::SchedulerType::MLFQ
    )
);