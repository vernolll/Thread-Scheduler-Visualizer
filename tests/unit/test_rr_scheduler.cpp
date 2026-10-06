#include <gtest/gtest.h>
#include "core/Thread.h"
#include "core/RRScheduler.h"

TEST(RRSchedulerTest, PreemptsThreadWhenQuantumExpires) 
{
    core::RRScheduler scheduler(2); // Quantum = 2

    auto t1 = std::make_shared<core::Thread>(1u, 3u, 1u); // burst = 3
    auto t2 = std::make_shared<core::Thread>(2u, 1u, 1u); // burst = 1

    scheduler.addThread(t1);
    scheduler.addThread(t2);

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 1u);

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 2u);

    scheduler.tick();
    ASSERT_NE(scheduler.getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler.getCurrentThread()->id, 1u);

    scheduler.tick();
    EXPECT_EQ(scheduler.getCurrentThread(), nullptr);
    EXPECT_TRUE(scheduler.isFinished());
}