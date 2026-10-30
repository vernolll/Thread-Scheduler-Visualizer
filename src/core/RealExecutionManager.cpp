#include "core/RealExecutionManager.h"
#include <chrono>
#include <thread>

namespace core
{
    RealExecutionManager::RealExecutionManager() = default;

    RealExecutionManager::~RealExecutionManager()
    {
        stopExecution();
    }

    void RealExecutionManager::initialize(const std::vector<std::shared_ptr<Thread>>& threads)
    {
        stopExecution();
        m_managedThreads = threads;
    }

    void RealExecutionManager::startExecution()
    {
        if (m_isRunning) return;

        m_stopRequested = false;
        m_isPaused = false;
        m_isRunning = true;

        m_osThreads.clear();
        for (auto& th : m_managedThreads)
        {
            if (th)
            {
                m_osThreads.emplace_back(&RealExecutionManager::threadWorkerRoutine, this, th);
            }
        }
    }

    void RealExecutionManager::pauseExecution()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isPaused = true;
    }

    void RealExecutionManager::stopExecution()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopRequested = true;
            m_isPaused = false;
        }
        m_cv.notify_all();

        for (auto& t : m_osThreads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
        m_osThreads.clear();
        m_isRunning = false;
    }

    bool RealExecutionManager::isRunning() const
    {
        return m_isRunning;
    }

    void RealExecutionManager::setStateChangeCallback(StateChangeCallback callback)
    {
        m_stateCallback = callback;
    }

    void RealExecutionManager::threadWorkerRoutine(std::shared_ptr<Thread> threadObj)
    {
        if (!threadObj) return;

        threadObj->state = ThreadState::READY;
        if (m_stateCallback) 
        {
            m_stateCallback(threadObj->id, threadObj->state);
        }

        while (threadObj->remainingTime > 0 && !m_stopRequested)
        {
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cv.wait(lock, [this] { return !m_isPaused || m_stopRequested; });
            }

            if (m_stopRequested) break;

            threadObj->state = ThreadState::RUNNING;
            if (m_stateCallback) 
            {
                m_stateCallback(threadObj->id, threadObj->state);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            threadObj->remainingTime--;

            if (threadObj->remainingTime > 0)
            {
                threadObj->state = ThreadState::READY;
                if (m_stateCallback) 
                {
                    m_stateCallback(threadObj->id, threadObj->state);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

        if (threadObj->remainingTime == 0)
        {
            threadObj->state = ThreadState::TERMINATED;
            if (m_stateCallback) 
            {
                m_stateCallback(threadObj->id, threadObj->state);
            }
        }
    }
}