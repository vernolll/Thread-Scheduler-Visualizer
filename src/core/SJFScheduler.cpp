#include "core/SJFScheduler.h"
#include <algorithm>

namespace core
{
    void SJFScheduler::sortReadyQueue()
    {
        std::sort(m_readyQueue.begin(), m_readyQueue.end(), [](const auto& a, const auto& b) 
            {
            uint32_t burstA = 0;
            if (a->currentBurstIndex < a->ioBursts.size())
            {
                burstA = a->ioBursts[a->currentBurstIndex].remaining;
            }
            uint32_t burstB = 0;
            if (b->currentBurstIndex < b->ioBursts.size())
            {
                burstB = b->ioBursts[b->currentBurstIndex].remaining;
            }

            if (burstA != burstB)
            {
                return burstA < burstB;
            }
            return a->id < b->id;
            });
    }

    void SJFScheduler::addThread(std::shared_ptr<Thread> thread)
    {
        if (!thread) return;

        thread->state = ThreadState::READY;
        m_allThreads.push_back(thread);
        m_readyQueue.push_back(thread);
        sortReadyQueue();
    }

    void SJFScheduler::tick()
    {
        m_currentTick++;

        if (m_remainingSwitchTicks > 0)
        {
            m_remainingSwitchTicks--;
            if (m_remainingSwitchTicks == 0 && !m_currentThread && !m_readyQueue.empty())
            {
                m_currentThread = m_readyQueue.front();
                m_readyQueue.erase(m_readyQueue.begin());
                changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU");
            }
            return;
        }

        if (!m_currentThread && !m_readyQueue.empty())
        {
            m_currentThread = m_readyQueue.front();
            m_readyQueue.erase(m_readyQueue.begin());
            changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU");
        }

        if (m_currentThread)
        {
            bool threadFinished = false;

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
                                m_currentThread = nullptr;
                                threadFinished = true;
                            }
                            else
                            {
                                changeThreadState(m_currentThread, ThreadState::READY, "CPU burst finished, moved to READY");
                                m_readyQueue.push_back(m_currentThread);
                                sortReadyQueue();
                                m_currentThread = nullptr;
                                threadFinished = true;
                            }
                        }
                        else
                        {
                            changeThreadState(m_currentThread, ThreadState::TERMINATED, "Thread completed");
                            m_currentThread = nullptr;
                            threadFinished = true;
                        }
                    }
                }
            }

            if (threadFinished && !m_readyQueue.empty())
            {
                if (m_contextSwitchOverhead > 0)
                {
                    m_remainingSwitchTicks = m_contextSwitchOverhead;
                }
                else
                {
                    m_currentThread = m_readyQueue.front();
                    m_readyQueue.erase(m_readyQueue.begin());
                    changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU");
                }
            }
        }
    }

    std::shared_ptr<Thread> SJFScheduler::getCurrentThread() const
    {
        return m_currentThread;
    }

    bool SJFScheduler::isFinished() const
    {
        return m_currentThread == nullptr && m_readyQueue.empty();
    }

    const std::vector<std::shared_ptr<Thread>>& SJFScheduler::getAllThreads() const
    {
        return m_allThreads;
    }
}