#include "core/RRScheduler.h"

namespace core
{
    RRScheduler::RRScheduler(uint32_t timeQuantum)
        : m_timeQuantum(timeQuantum)
    {
    }

    void RRScheduler::addThread(std::shared_ptr<Thread> thread)
    {
        if (!thread) return;

        thread->state = ThreadState::READY;
        m_allThreads.push_back(thread);
        m_readyQueue.push(thread);
    }

    void RRScheduler::tick()
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
            m_currentQuantumTicks = 0;
            changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU (RR)");

            if (m_contextSwitchOverhead > 0)
            {
                m_remainingSwitchTicks = m_contextSwitchOverhead;
                return;
            }
        }

        if (m_currentThread)
        {
            m_currentQuantumTicks++;

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

            if (m_currentThread && m_currentQuantumTicks >= m_timeQuantum && m_currentThread->state == ThreadState::RUNNING)
            {
                if (!m_readyQueue.empty())
                {
                    changeThreadState(m_currentThread, ThreadState::READY, "Time quantum expired, moved to READY");
                    m_readyQueue.push(m_currentThread);
                    m_currentThread = nullptr;

                    if (m_contextSwitchOverhead > 0)
                    {
                        m_remainingSwitchTicks = m_contextSwitchOverhead;
                    }
                }
                else
                {
                    m_currentQuantumTicks = 0;
                }
            }

            if (!m_currentThread && !m_readyQueue.empty() && m_remainingSwitchTicks == 0)
            {
                m_currentThread = m_readyQueue.front();
                m_readyQueue.pop();
                m_currentQuantumTicks = 0;
                changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU (RR)");
                if (m_contextSwitchOverhead > 0)
                {
                    m_remainingSwitchTicks = m_contextSwitchOverhead;
                }
            }
        }
    }

    std::shared_ptr<Thread> RRScheduler::getCurrentThread() const
    {
        return m_currentThread;
    }

    bool RRScheduler::isFinished() const
    {
        return m_currentThread == nullptr && m_readyQueue.empty() && m_waitingThreads.empty();
    }

    const std::vector<std::shared_ptr<Thread>>& RRScheduler::getAllThreads() const
    {
        return m_allThreads;
    }
}