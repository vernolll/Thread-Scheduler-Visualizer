#include <gtest/gtest.h>
#include "core/SJFScheduler.h"
#include "core/SRTFScheduler.h"
#include "core/Thread.h"

TEST(SJFSchedulerTest, ExecutesShorterJobFirst)
{
    core::SJFScheduler scheduler;

    auto tLong = std::make_shared<core::Thread>(1u, 5u);
    auto tShort = std::make_shared<core::Thread>(2u, 2u);

    scheduler.addThread(tLong);
    scheduler.addThread(tShort);

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 2u);
}

TEST(SRTFSchedulerTest, PreemptsOnShorterRemainingTime)
{
    core::SRTFScheduler scheduler;

    auto tLong = std::make_shared<core::Thread>(1u, 10u);
    auto tShort = std::make_shared<core::Thread>(2u, 2u);

    scheduler.addThread(tLong);
    scheduler.tick();
    ASSERT_EQ(scheduler.getCurrentThread()->id, 1u);

    scheduler.addThread(tShort);
    EXPECT_EQ(tLong->state, core::ThreadState::READY);

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 2u);
}