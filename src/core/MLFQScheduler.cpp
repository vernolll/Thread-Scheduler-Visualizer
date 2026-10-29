#include "core/MLFQScheduler.h"

namespace core
{
    MLFQScheduler::MLFQScheduler(size_t numLevels, uint32_t boostInterval)
        : m_boostInterval(boostInterval)
    {
        uint32_t quantum = 2;
        for (size_t i = 0; i < numLevels; ++i)
        {
            m_levels.push_back({ quantum, {} });
            quantum *= 2;
        }
    }

    void MLFQScheduler::addThread(std::shared_ptr<Thread> thread)
    {
        if (!thread) return;

        thread->state = ThreadState::READY;
        m_allThreads.push_back(thread);

        m_levels[0].queue.push_back(thread);
    }

    void MLFQScheduler::performBoost()
    {
        for (size_t i = 1; i < m_levels.size(); ++i)
        {
            for (auto& th : m_levels[i].queue)
            {
                m_levels[0].queue.push_back(th);
            }
            m_levels[i].queue.clear();
        }
        m_ticksSinceBoost = 0;
    }

    std::pair<size_t, std::shared_ptr<Thread>> MLFQScheduler::findNextThread()
    {
        for (size_t i = 0; i < m_levels.size(); ++i)
        {
            if (!m_levels[i].queue.empty())
            {
                auto th = m_levels[i].queue.front();
                m_levels[i].queue.erase(m_levels[i].queue.begin());
                return { i, th };
            }
        }
        return { 0, nullptr };
    }

    void MLFQScheduler::tick()
    {
        m_currentTick++;
        m_ticksSinceBoost++;

        if (m_ticksSinceBoost >= m_boostInterval)
        {
            performBoost();
        }

        if (m_remainingSwitchTicks > 0)
        {
            m_remainingSwitchTicks--;
            if (m_remainingSwitchTicks == 0 && !m_currentThread && !isFinished())
            {
                auto [lvl, th] = findNextThread();
                if (th)
                {
                    m_currentLevel = lvl;
                    m_currentThread = th;
                    m_currentQuantumUsed = 0;
                    changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU (MLFQ)");
                }
            }
            return;
        }

        if (!m_currentThread)
        {
            auto [lvl, th] = findNextThread();
            if (th)
            {
                m_currentLevel = lvl;
                m_currentThread = th;
                m_currentQuantumUsed = 0;
                changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU (MLFQ)");
            }
        }

        if (m_currentThread)
        {
            bool needSwitch = false;

            if (m_currentThread->currentBurstIndex < m_currentThread->ioBursts.size())
            {
                auto& burst = m_currentThread->ioBursts[m_currentThread->currentBurstIndex];
                if (burst.isCpuBurst && burst.remaining > 0)
                {
                    burst.remaining--;
                    m_currentQuantumUsed++;
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
                            }
                            else
                            {
                                changeThreadState(m_currentThread, ThreadState::READY, "CPU burst finished, moved to READY");
                                m_levels[0].queue.push_back(m_currentThread);
                            }
                        }
                        else
                        {
                            changeThreadState(m_currentThread, ThreadState::TERMINATED, "Thread completed");
                        }
                        m_currentThread = nullptr;
                        needSwitch = true;
                    }
                    else if (m_currentQuantumUsed >= m_levels[m_currentLevel].timeQuantum)
                    {
                        size_t nextLevel = (m_currentLevel + 1 < m_levels.size()) ? m_currentLevel + 1 : m_currentLevel;
                        changeThreadState(m_currentThread, ThreadState::READY, "Quantum expired, demoted to lower queue");
                        m_levels[nextLevel].queue.push_back(m_currentThread);
                        m_currentThread = nullptr;
                        needSwitch = true;
                    }
                }
            }

            if (needSwitch)
            {
                if (!isFinished())
                {
                    if (m_contextSwitchOverhead > 0)
                    {
                        m_remainingSwitchTicks = m_contextSwitchOverhead;
                    }
                    else
                    {
                        auto [lvl, th] = findNextThread();
                        if (th)
                        {
                            m_currentLevel = lvl;
                            m_currentThread = th;
                            m_currentQuantumUsed = 0;
                            changeThreadState(m_currentThread, ThreadState::RUNNING, "Scheduled on CPU (MLFQ)");
                        }
                    }
                }
            }
        }
    }

    std::shared_ptr<Thread> MLFQScheduler::getCurrentThread() const
    {
        return m_currentThread;
    }

    bool MLFQScheduler::isFinished() const
    {
        if (m_currentThread != nullptr) return false;
        for (const auto& lvl : m_levels)
        {
            if (!lvl.queue.empty()) return false;
        }
        for (const auto& th : m_allThreads)
        {
            if (th->state != ThreadState::TERMINATED) return false;
        }
        return true;
    }

    const std::vector<std::shared_ptr<Thread>>& MLFQScheduler::getAllThreads() const
    {
        return m_allThreads;
    }
}