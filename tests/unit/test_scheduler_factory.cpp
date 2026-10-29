#include <gtest/gtest.h>
#include "core/SchedulerFactory.h"
#include "core/SchedulerTypes.h"

TEST(SchedulerFactoryTest, CreatesCorrectSchedulerInstances)
{
    auto fcfs = core::SchedulerFactory::createScheduler(core::SchedulerType::FCFS);
    EXPECT_NE(fcfs, nullptr);

    auto rr = core::SchedulerFactory::createScheduler(core::SchedulerType::RoundRobin);
    EXPECT_NE(rr, nullptr);

    auto priority = core::SchedulerFactory::createScheduler(core::SchedulerType::Priority);
    EXPECT_NE(priority, nullptr);
}

TEST(SchedulerFactoryTest, ThrowsOnInvalidType)
{
    EXPECT_THROW(
        core::SchedulerFactory::createScheduler(static_cast<core::SchedulerType>(999)),
        std::invalid_argument
    );
}