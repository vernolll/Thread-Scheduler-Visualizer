#include "core/FCFSScheduler.h"
#include <algorithm>

namespace core
{
    void FCFSScheduler::addThread(std::shared_ptr<Thread> thread)
    {
        if (!thread) return;

        thread->state = ThreadState::READY;
        m_allThreads.push_back(thread);
        m_readyQueue.push(thread);
    }

    void FCFSScheduler::tick()
    {
        m_currentTick++;

        for (auto it = m_waitingThreads.begin(); it != m_waitingThreads.end(); )
        {
            auto thread = *it;
            if (thread && thread->state == ThreadState::WAITING)
            {
                if (thread->currentBurstIndex < thread->ioBursts.size())
                {
                    auto& burst = thread->ioBursts[thread->currentBurstIndex];
                    if (!burst.isCpuBurst && burst.remaining > 0)
                    {
                        burst.remaining--;
                        if (burst.remaining == 0)
                        {
                            thread->currentBurstIndex++;
                            if (thread->currentBurstIndex < thread->ioBursts.size())
                            {
                                changeThreadState(thread, ThreadState::READY, "I/O burst finished, moved to READY");
                                m_readyQueue.push(thread);
                            }
                            else
                            {
                                changeThreadState(thread, ThreadState::TERMINATED, "Thread finished");
                            }
                            it = m_waitingThreads.erase(it);
                            continue;
                        }
                    }
                }
            }
            ++it;
        }

        if (m_remainingSwitchTicks > 0)
        {
            m_remainingSwitchTicks--;
            return;
        }

        if (!m_currentThread && !m_readyQueue.empty())
        {
            m_currentThread = m_readyQueue.front();
            m_readyQueue.pop();
            changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU");

            if (m_contextSwitchOverhead > 0)
            {
                m_remainingSwitchTicks = m_contextSwitchOverhead;
                return;
            }
        }

        if (m_currentThread)
        {
            if (m_currentThread->currentBurstIndex < m_currentThread->ioBursts.size())
            {
                auto& burst = m_currentThread->ioBursts[m_currentThread->currentBurstIndex];
                if (burst.isCpuBurst && burst.remaining > 0)
                {
                    burst.remaining--;
                    if (m_currentThread->remainingTime > 0)
                    {
                        m_currentThread->remainingTime--;
                    }

                    if (burst.remaining == 0)
                    {
                        m_currentThread->currentBurstIndex++;
                        if (m_currentThread->currentBurstIndex < m_currentThread->ioBursts.size())
                        {
                            auto& nextBurst = m_currentThread->ioBursts[m_currentThread->currentBurstIndex];
                            if (!nextBurst.isCpuBurst)
                            {
                                changeThreadState(m_currentThread, ThreadState::WAITING, "Entering I/O burst");
                                m_waitingThreads.push_back(m_currentThread);
                                m_currentThread = nullptr;
                            }
                            else
                            {
                                changeThreadState(m_currentThread, ThreadState::READY, "CPU burst finished, moved to READY");
                                m_readyQueue.push(m_currentThread);
                                m_currentThread = nullptr;
                            }
                        }
                        else
                        {
                            changeThreadState(m_currentThread, ThreadState::TERMINATED, "Thread completed");
                            m_currentThread = nullptr;
                        }

                        if (!m_currentThread && !m_readyQueue.empty() && m_contextSwitchOverhead > 0)
                        {
                            m_remainingSwitchTicks = m_contextSwitchOverhead;
                        }
                    }
                }
            }
            else if (m_currentThread->remainingTime > 0)
            {
                m_currentThread->remainingTime--;
                if (m_currentThread->remainingTime == 0)
                {
                    changeThreadState(m_currentThread, ThreadState::TERMINATED, "Thread completed");
                    m_currentThread = nullptr;

                    if (!m_readyQueue.empty() && m_contextSwitchOverhead > 0)
                    {
                        m_remainingSwitchTicks = m_contextSwitchOverhead;
                    }
                }
            }

            if (!m_currentThread && !m_readyQueue.empty() && m_remainingSwitchTicks == 0)
            {
                m_currentThread = m_readyQueue.front();
                m_readyQueue.pop();
                changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU");
                if (m_contextSwitchOverhead > 0)
                {
                    m_remainingSwitchTicks = m_contextSwitchOverhead;
                }
            }
        }
    }

    std::shared_ptr<Thread> FCFSScheduler::getCurrentThread() const
    {
        return m_currentThread;
    }

    bool FCFSScheduler::isFinished() const
    {
        return m_currentThread == nullptr && m_readyQueue.empty() && m_waitingThreads.empty();
    }

    const std::vector<std::shared_ptr<Thread>>& FCFSScheduler::getAllThreads() const
    {
        return m_allThreads;
    }
}