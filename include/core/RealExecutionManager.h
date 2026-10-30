#ifndef REAL_EXECUTION_MANAGER_H
#define REAL_EXECUTION_MANAGER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <memory>
#include <functional>
#include "core/Thread.h"

namespace core
{
    class RealExecutionManager
    {
    public:
        RealExecutionManager();
        ~RealExecutionManager();

        void initialize(const std::vector<std::shared_ptr<Thread>>& threads);

        void startExecution();
        void pauseExecution();
        void stopExecution();
        bool isRunning() const;

        using StateChangeCallback = std::function<void(uint32_t threadId, ThreadState newState)>;
        void setStateChangeCallback(StateChangeCallback callback);

    private:
        void threadWorkerRoutine(std::shared_ptr<Thread> threadObj);

        std::vector<std::thread> m_osThreads;
        std::vector<std::shared_ptr<Thread>> m_managedThreads;

        std::mutex m_mutex;
        std::condition_variable m_cv;

        std::atomic<bool> m_isRunning{ false };
        std::atomic<bool> m_isPaused{ false };
        std::atomic<bool> m_stopRequested{ false };

        StateChangeCallback m_stateCallback;
    };
}

#endif // REAL_EXECUTION_MANAGER_H