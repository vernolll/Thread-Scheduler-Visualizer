#include <gtest/gtest.h>
#include "core/BenchmarkRunner.h"
#include "core/Thread.h"

TEST(BenchmarkRunnerTest, RunsAllSchedulersSuccessfully)
{
    std::vector<std::shared_ptr<core::Thread>> baseline;

    auto th1 = std::make_shared<core::Thread>(1u, 5u, 1u);
    th1->arrivalTime = 0u;
    baseline.push_back(th1);

    auto th2 = std::make_shared<core::Thread>(2u, 3u, 2u);
    th2->arrivalTime = 0u;
    baseline.push_back(th2);

    auto results = core::BenchmarkRunner::runBenchmark(baseline);

    EXPECT_EQ(results.size(), 6u);

    for (const auto& res : results)
    {
        EXPECT_TRUE(res.success);
        EXPECT_GT(res.totalTicks, 0u);
    }
}