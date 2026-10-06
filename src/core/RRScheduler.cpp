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
        if (!m_currentThread && !m_readyQueue.empty())
        {
            m_currentThread = m_readyQueue.front();
            m_readyQueue.pop();
            m_currentThread->state = ThreadState::RUNNING;
            m_currentQuantumTicks = 0;
        }

        if (m_currentThread) 
        {
            m_currentThread->remainingTime--;
            m_currentQuantumTicks++;

            if (m_currentThread->remainingTime == 0) 
            {
                m_currentThread->state = ThreadState::TERMINATED;
                m_currentThread = nullptr;
                m_currentQuantumTicks = 0;

                if (!m_readyQueue.empty()) 
                {
                    m_currentThread = m_readyQueue.front();
                    m_readyQueue.pop();
                    m_currentThread->state = ThreadState::RUNNING;
                }
            }
            else if (m_currentQuantumTicks >= m_timeQuantum) 
            {
                m_currentThread->state = ThreadState::READY;
                m_readyQueue.push(m_currentThread);

                m_currentThread = m_readyQueue.front();
                m_readyQueue.pop();
                m_currentThread->state = ThreadState::RUNNING;
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
        return m_currentThread == nullptr && m_readyQueue.empty();
    }

    const std::vector<std::shared_ptr<Thread>>& RRScheduler::getAllThreads() const
    {
        return m_allThreads;
    }
}