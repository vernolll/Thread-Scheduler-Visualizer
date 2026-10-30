#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QTimer>
#include <QThread>
#include <memory>
#include <vector>
#include "core/SimulationWorker.h"
#include "core/WorkloadScenario.h"

namespace gui
{
    class GUIController : public QObject
    {
        Q_OBJECT
            Q_PROPERTY(QString currentScheduler READ currentScheduler WRITE setCurrentScheduler NOTIFY currentSchedulerChanged)
            Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
            Q_PROPERTY(int timeQuantum READ timeQuantum WRITE setTimeQuantum NOTIFY timeQuantumChanged)
            Q_PROPERTY(QVariantList ganttBlocks READ ganttBlocks NOTIFY ganttBlocksChanged)
            Q_PROPERTY(QVariantList readyQueue READ readyQueue NOTIFY readyQueueChanged)
            Q_PROPERTY(QVariantList threadList READ threadList NOTIFY threadListChanged)
            Q_PROPERTY(QVariantList benchmarkResults READ benchmarkResults NOTIFY benchmarkResultsChanged)
            Q_PROPERTY(int currentTime READ currentTime NOTIFY currentTimeChanged)
            Q_PROPERTY(QStringList scenarioNames READ scenarioNames CONSTANT)

    public:
        explicit GUIController(QObject* parent = nullptr);
        ~GUIController() override;

        QString currentScheduler() const { return m_currentScheduler; }
        void setCurrentScheduler(const QString& scheduler);

        bool isRunning() const { return m_isRunning; }
        int timeQuantum() const { return m_timeQuantum; }
        void setTimeQuantum(int quantum);

        QVariantList ganttBlocks() const { return m_ganttBlocks; }
        QVariantList readyQueue() const { return m_readyQueue; }
        QVariantList threadList() const { return m_threadList; }
        QVariantList benchmarkResults() const { return m_benchmarkResults; }
        int currentTime() const { return m_currentTime; }

    public slots:
        void startSimulation();
        void pauseSimulation();
        void stepSimulation();
        void resetSimulation();
        void addThread(int id, int burstTime, int priority);
        void runAlgorithmBenchmark();

        Q_INVOKABLE void loadScenario(int index);
        Q_INVOKABLE void saveToJson(const QString& filePath);
        Q_INVOKABLE void loadFromJson(const QString& filePath);
        Q_INVOKABLE void exportCsv(const QString& filePath);

    signals:
        void currentSchedulerChanged();
        void isRunningChanged();
        void timeQuantumChanged();
        void simulationUpdated();
        void ganttBlocksChanged();
        void readyQueueChanged();
        void threadListChanged();
        void benchmarkResultsChanged();
        void currentTimeChanged();

    private slots:
        void onCurrentTimeChanged(int time);
        void onIsRunningChanged(bool running);
        void onReadyQueueUpdated(const QVariantList& queue);
        void onThreadListUpdated(const QVariantList& threads);
        void onGanttBlockAppended(int threadId, const QString& name, int startTime, int duration, const QString& color);
        void onGanttBlocksCleared();
        void onBenchmarkResultsUpdated(const QVariantList& results);

    private:
        QString m_currentScheduler{ "FCFS" };
        bool m_isRunning{ false };
        int m_timeQuantum{ 2 };
        int m_currentTime{ 0 };
        int m_nextThreadId{ 0 };
        QVariantList m_ganttBlocks;
        QVariantList m_readyQueue;
        QVariantList m_threadList;
        QVariantList m_benchmarkResults;
        std::vector<core::ScenarioThreadConfig> m_lastLoadedThreads;

        QThread m_workerThread;
        core::SimulationWorker* m_worker{ nullptr };
        QTimer* m_timer{ nullptr };

        QStringList scenarioNames() const;
    };
}

#endif // GUI_CONTROLLER_H