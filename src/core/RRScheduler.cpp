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
                                thread->setState(ThreadState::READY);
                                m_readyQueue.push(thread);
                            }
                            else
                            {
                                thread->setState(ThreadState::TERMINATED);
                            }
                            it = m_waitingThreads.erase(it);
                            continue;
                        }
                    }
                }
            }
            ++it;
        }

        if (!m_currentThread && !m_readyQueue.empty())
        {
            m_currentThread = m_readyQueue.front();
            m_readyQueue.pop();
            m_currentThread->setState(ThreadState::RUNNING);
            m_currentQuantumTicks = 0;
        }

        if (m_currentThread)
        {
            bool burstFinished = false;

            if (m_currentThread->currentBurstIndex < m_currentThread->ioBursts.size())
            {
                auto& burst = m_currentThread->ioBursts[m_currentThread->currentBurstIndex];
                if (burst.isCpuBurst && burst.remaining > 0)
                {
                    burst.remaining--;
                    m_currentQuantumTicks++;
                    if (m_currentThread->remainingTime > 0)
                    {
                        m_currentThread->remainingTime--;
                    }

                    if (burst.remaining == 0)
                    {
                        burstFinished = true;
                    }
                }
            }
            else if (m_currentThread->remainingTime > 0)
            {
                m_currentThread->remainingTime--;
                m_currentQuantumTicks++;
                if (m_currentThread->remainingTime == 0)
                {
                    burstFinished = true;
                }
            }

            if (burstFinished || (m_currentThread->remainingTime == 0))
            {
                m_currentThread->currentBurstIndex++;
                m_currentQuantumTicks = 0;

                if (m_currentThread->currentBurstIndex < m_currentThread->ioBursts.size() && m_currentThread->remainingTime > 0)
                {
                    auto& nextBurst = m_currentThread->ioBursts[m_currentThread->currentBurstIndex];
                    if (!nextBurst.isCpuBurst)
                    {
                        m_currentThread->setState(ThreadState::WAITING);
                        m_waitingThreads.push_back(m_currentThread);
                        m_currentThread = nullptr;
                    }
                    else
                    {
                        m_currentThread->setState(ThreadState::READY);
                        m_readyQueue.push(m_currentThread);
                        m_currentThread = nullptr;
                    }
                }
                else
                {
                    m_currentThread->setState(ThreadState::TERMINATED);
                    m_currentThread = nullptr;
                }
            }
            else if (m_currentQuantumTicks >= m_timeQuantum)
            {
                m_currentThread->setState(ThreadState::READY);
                m_readyQueue.push(m_currentThread);

                m_currentThread = nullptr;
                m_currentQuantumTicks = 0;
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