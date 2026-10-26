#include <gtest/gtest.h>
#include "core/PriorityScheduler.h"
#include "core/Thread.h"

TEST(PrioritySchedulerTest, ExecutesThreadsInPriorityOrder)
{
    core::PriorityScheduler scheduler(/*preemptive=*/false);

    auto tLow = std::make_shared<core::Thread>(1u, 3u, 10u);
    auto tHigh = std::make_shared<core::Thread>(2u, 2u, 1u);

    scheduler.addThread(tLow);
    scheduler.addThread(tHigh);

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 2u);

    scheduler.tick();

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 1u);
}

TEST(PrioritySchedulerTest, PreemptsRunningThreadOnHigherPriority)
{
    core::PriorityScheduler scheduler(/*preemptive=*/true);

    auto tLow = std::make_shared<core::Thread>(1u, 5u, 10u);
    auto tHigh = std::make_shared<core::Thread>(2u, 2u, 1u);

    scheduler.addThread(tLow);
    scheduler.tick();
    ASSERT_EQ(scheduler.getCurrentThread()->id, 1u);
    EXPECT_EQ(tLow->state, core::ThreadState::RUNNING);

    scheduler.addThread(tHigh);

    EXPECT_EQ(tLow->state, core::ThreadState::READY);
    EXPECT_EQ(scheduler.getCurrentThread(), nullptr);

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 2u);
    EXPECT_EQ(tHigh->state, core::ThreadState::RUNNING);
}