#include <gtest/gtest.h>
#include "core/FCFSScheduler.h"
#include "core/Thread.h"

TEST(ContextSwitchTest, AddsOverheadTicksBetweenTransitions)
{
    core::FCFSScheduler scheduler;
    scheduler.setContextSwitchOverhead(2);

    auto t1 = std::make_shared<core::Thread>(1, 0, 3);
    auto t2 = std::make_shared<core::Thread>(2, 0, 3);

    scheduler.addThread(t1);
    scheduler.addThread(t2);

    for (int i = 0; i < 40; ++i)
    {
        scheduler.tick();
    }

    EXPECT_EQ(t1->state, core::ThreadState::TERMINATED);
    EXPECT_EQ(t2->state, core::ThreadState::TERMINATED);
}