#include <gtest/gtest.h>
#include "core/MLFQScheduler.h"
#include "core/SchedulerFactory.h"
#include "core/SchedulerTypes.h"
#include "core/Thread.h"

TEST(MLFQSchedulerTest, ExecutesAndDemotesOnQuantumExpiry)
{
    auto scheduler = core::SchedulerFactory::createScheduler(core::SchedulerType::MLFQ);
    ASSERT_NE(scheduler, nullptr);

    auto thread = std::make_shared<core::Thread>(1u, 5u);
    scheduler->addThread(thread);

    scheduler->tick();
    ASSERT_NE(scheduler->getCurrentThread(), nullptr);
    EXPECT_EQ(scheduler->getCurrentThread()->id, 1u);

    scheduler->tick();

    scheduler->tick();

    while (!scheduler->isFinished())
    {
        scheduler->tick();
    }

    EXPECT_EQ(thread->state, core::ThreadState::TERMINATED);
}