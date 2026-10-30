#ifndef SIMULATION_WORKER_H
#define SIMULATION_WORKER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <memory>
#include <vector>
#include "core/Scheduler.h"
#include "core/WorkloadScenario.h"
#include "core/RealExecutionManager.h"

namespace core
{
    enum class ExecutionMode
    {
        SIMULATION,
        REAL_THREADS
    };

    class SimulationWorker : public QObject
    {
        Q_OBJECT
    public:
        explicit SimulationWorker(QObject* parent = nullptr);
        ~SimulationWorker() override = default;

    public slots:
        void setExecutionMode(int mode); // 0 = SIMULATION, 1 = REAL_THREADS
        void setCurrentScheduler(const QString& scheduler, int timeQuantum);
        void setTimeQuantum(int quantum);
        void startSimulation();
        void pauseSimulation();
        void stepSimulation();
        void resetSimulation();
        void addThread(int id, int burstTime, int priority);
        void loadScenario(int index);
        void runAlgorithmBenchmark();

    signals:
        void currentTimeChanged(int time);
        void isRunningChanged(bool isRunning);
        void readyQueueUpdated(const QVariantList& queue);
        void threadListUpdated(const QVariantList& threads);
        void ganttBlockAppended(int threadId, const QString& name, int startTime, int duration, const QString& color);
        void ganttBlocksCleared();
        void benchmarkResultsUpdated(const QVariantList& results);

    private:
        QString m_currentScheduler{ "FCFS" };
        bool m_isRunning{ false };
        int m_timeQuantum{ 2 };
        int m_currentTime{ 0 };
        ExecutionMode m_executionMode{ ExecutionMode::SIMULATION };

        std::shared_ptr<core::Scheduler> m_scheduler;
        std::unique_ptr<core::RealExecutionManager> m_realExecutionManager;

        void updateReadyQueue();
        void updateThreadList();
        void handleRealThreadStateChange(uint32_t threadId, ThreadState newState);
    };
}

#endif // SIMULATION_WORKER_H