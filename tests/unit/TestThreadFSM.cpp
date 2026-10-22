#include <gtest/gtest.h>
#include "core/Thread.h"

TEST(ThreadFSMTest, InitialStateIsNew)
{
    core::Thread t(1, 10, 0);
    EXPECT_EQ(t.state, core::ThreadState::NEW);
}

TEST(ThreadFSMTest, ValidTransitions)
{
    core::Thread t(1, 10, 0);

    // NEW -> READY
    EXPECT_TRUE(t.setState(core::ThreadState::READY));
    EXPECT_EQ(t.state, core::ThreadState::READY);

    // READY -> RUNNING
    EXPECT_TRUE(t.setState(core::ThreadState::RUNNING));
    EXPECT_EQ(t.state, core::ThreadState::RUNNING);

    // RUNNING -> BLOCKED
    EXPECT_TRUE(t.setState(core::ThreadState::BLOCKED));
    EXPECT_EQ(t.state, core::ThreadState::BLOCKED);

    // BLOCKED -> READY
    EXPECT_TRUE(t.setState(core::ThreadState::READY));
    EXPECT_EQ(t.state, core::ThreadState::READY);

    // READY -> TERMINATED
    EXPECT_TRUE(t.setState(core::ThreadState::TERMINATED));
    EXPECT_EQ(t.state, core::ThreadState::TERMINATED);
}

TEST(ThreadFSMTest, InvalidTransitions)
{
    core::Thread t(1, 10, 0);

    EXPECT_FALSE(t.setState(core::ThreadState::RUNNING));
    EXPECT_EQ(t.state, core::ThreadState::NEW);

    t.state = core::ThreadState::TERMINATED;

    EXPECT_FALSE(t.setState(core::ThreadState::READY));
    EXPECT_FALSE(t.setState(core::ThreadState::RUNNING));
    EXPECT_EQ(t.state, core::ThreadState::TERMINATED);
}