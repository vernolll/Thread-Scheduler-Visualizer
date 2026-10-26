#include <gtest/gtest.h>
#include "core/MetricsCalculator.h"
#include "core/Thread.h"
#include "core/SimulationEvent.h"

TEST(MetricsCalculatorTest, CalculatesBasicMetricsCorrectly)
{
    auto t1 = std::make_shared<core::Thread>(1u, 3u, 0u);
    auto t2 = std::make_shared<core::Thread>(2u, 5u, 0u);

    std::vector<std::shared_ptr<core::Thread>> threads = { t1, t2 };

    std::vector<core::SimulationEvent> events = {
        core::SimulationEvent(0, 1, core::ThreadState::NEW, core::ThreadState::READY, "Added"),
        core::SimulationEvent(0, 2, core::ThreadState::NEW, core::ThreadState::READY, "Added"),
        core::SimulationEvent(1, 1, core::ThreadState::READY, core::ThreadState::RUNNING, "Scheduled"),
        core::SimulationEvent(4, 1, core::ThreadState::RUNNING, core::ThreadState::TERMINATED, "Completed"),
        core::SimulationEvent(5, 2, core::ThreadState::READY, core::ThreadState::RUNNING, "Scheduled"),
        core::SimulationEvent(10, 2, core::ThreadState::RUNNING, core::ThreadState::TERMINATED, "Completed")
    };

    t1->waitingTime = 1;
    t2->waitingTime = 5;

    core::SimulationMetrics metrics = core::MetricsCalculator::calculate(threads, events, 10);

    ASSERT_EQ(metrics.perThreadMetrics.size(), 2);

    EXPECT_EQ(metrics.perThreadMetrics[0].threadId, 1u);
    EXPECT_EQ(metrics.perThreadMetrics[0].turnaroundTime, 4u);
    EXPECT_EQ(metrics.perThreadMetrics[0].waitingTime, 1u);
    EXPECT_EQ(metrics.perThreadMetrics[0].responseTime, 1u);

    EXPECT_EQ(metrics.perThreadMetrics[1].threadId, 2u);
    EXPECT_EQ(metrics.perThreadMetrics[1].turnaroundTime, 10u);
    EXPECT_EQ(metrics.perThreadMetrics[1].waitingTime, 5u);
    EXPECT_EQ(metrics.perThreadMetrics[1].responseTime, 5u);

    EXPECT_DOUBLE_EQ(metrics.averageTurnaroundTime, 7.0);
    EXPECT_DOUBLE_EQ(metrics.averageWaitingTime, 3.0);
}