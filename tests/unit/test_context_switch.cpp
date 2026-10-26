#include <gtest/gtest.h>
#include "core/FCFSScheduler.h"
#include "core/Thread.h"

TEST(ContextSwitchTest, AddsOverheadTicksBetweenTransitions)
{
    core::FCFSScheduler scheduler;
    scheduler.setContextSwitchOverhead(2);

    auto t1 = std::make_shared<core::Thread>(1, 0, 5); // id=1, priority=0, time=5
    auto t2 = std::make_shared<core::Thread>(2, 0, 5); // id=2, priority=0, time=5

    scheduler.addThread(t1);
    scheduler.addThread(t2);

    scheduler.tick();
    EXPECT_EQ(scheduler.getCurrentThread(), t1);
    EXPECT_EQ(t1->state, core::ThreadState::RUNNING);

    for (int i = 0; i < 4; ++i)
    {
        scheduler.tick();
    }
    EXPECT_EQ(t1->state, core::ThreadState::TERMINATED);

    scheduler.tick();
}