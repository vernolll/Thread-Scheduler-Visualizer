#include <gtest/gtest.h>
#include "core/SchedulerFactory.h"
#include "core/SchedulerTypes.h"
#include "core/Thread.h"

class SchedulerParamTest : public ::testing::TestWithParam<core::SchedulerType> {};

TEST_P(SchedulerParamTest, ExecutesAndTerminatesAllThreads)
{
    core::SchedulerType type = GetParam();
    auto scheduler = core::SchedulerFactory::createScheduler(type);
    ASSERT_NE(scheduler, nullptr);

    scheduler->addThread(std::make_shared<core::Thread>(1u, 3u, 1u));
    scheduler->addThread(std::make_shared<core::Thread>(2u, 2u, 2u));

    int maxTicks = 100;
    int ticks = 0;
    while (!scheduler->isFinished() && ticks < maxTicks)
    {
        scheduler->tick();
        ticks++;
    }

    EXPECT_TRUE(scheduler->isFinished());

    for (const auto& th : scheduler->getAllThreads())
    {
        EXPECT_EQ(th->state, core::ThreadState::TERMINATED);
    }
}

INSTANTIATE_TEST_SUITE_P(
    AllSchedulersParameterized,
    SchedulerParamTest,
    ::testing::Values(
        core::SchedulerType::FCFS,
        core::SchedulerType::RoundRobin,
        core::SchedulerType::Priority,
        core::SchedulerType::SJF,
        core::SchedulerType::SRTF,
        core::SchedulerType::MLFQ
    )
);