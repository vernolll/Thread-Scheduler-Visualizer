#include "core/FCFSScheduler.h"

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
        if (!m_currentThread && !m_readyQueue.empty()) 
        {
            m_currentThread = m_readyQueue.front();
            m_readyQueue.pop();
            m_currentThread->state = ThreadState::RUNNING;
        }

        if (m_currentThread) 
        {
            m_currentThread->remainingTime--;

            if (m_currentThread->remainingTime <= 0) 
            {
                m_currentThread->state = ThreadState::TERMINATED;
                m_currentThread = nullptr;

                if (!m_readyQueue.empty()) 
                {
                    m_currentThread = m_readyQueue.front();
                    m_readyQueue.pop();
                    m_currentThread->state = ThreadState::RUNNING;
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
        return m_currentThread == nullptr && m_readyQueue.empty();
    }

    const std::vector<std::shared_ptr<Thread>>& FCFSScheduler::getAllThreads() const
    {
        return m_allThreads;
    }

}