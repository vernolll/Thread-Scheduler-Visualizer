#include <gtest/gtest.h>
#include "core/EventLogger.h"
#include "core/SimulationEvent.h"

TEST(EventLoggerTest, LogsAndRetrievesEvents)
{
    core::EventLogger logger;

    EXPECT_EQ(logger.size(), 0u);

    logger.logEvent(1, 101, core::ThreadState::NEW, core::ThreadState::READY, "Thread added");
    logger.logEvent(2, 101, core::ThreadState::READY, core::ThreadState::RUNNING, "Scheduled");
    logger.logEvent(3, 102, core::ThreadState::NEW, core::ThreadState::READY, "Thread added");

    EXPECT_EQ(logger.size(), 3u);

    auto allEvents = logger.getEvents();
    EXPECT_EQ(allEvents[0].tick, 1u);
    EXPECT_EQ(allEvents[0].threadId, 101u);
    EXPECT_EQ(allEvents[0].reason, "Thread added");

    auto thread101Events = logger.getEventsForThread(101);
    EXPECT_EQ(thread101Events.size(), 2u);

    auto thread102Events = logger.getEventsForThread(102);
    EXPECT_EQ(thread102Events.size(), 1u);
    EXPECT_EQ(thread102Events[0].threadId, 102u);

    logger.clear();
    EXPECT_EQ(logger.size(), 0u);
}