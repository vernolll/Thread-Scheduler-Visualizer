#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QTimer>
#include <memory>
#include "core/Scheduler.h"

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
            Q_PROPERTY(int currentTime READ currentTime NOTIFY currentTimeChanged)

    public:
        explicit GUIController(QObject* parent = nullptr);
        ~GUIController() override = default;

        QString currentScheduler() const { return m_currentScheduler; }
        void setCurrentScheduler(const QString& scheduler);

        bool isRunning() const { return m_isRunning; }
        int timeQuantum() const { return m_timeQuantum; }
        void setTimeQuantum(int quantum);

        QVariantList ganttBlocks() const { return m_ganttBlocks; }
        QVariantList readyQueue() const { return m_readyQueue; }
        QVariantList threadList() const { return m_threadList; }
        int currentTime() const { return m_currentTime; }

    public slots:
        void startSimulation();
        void pauseSimulation();
        void stepSimulation();
        void resetSimulation();
        void addThread(int id, int burstTime, int priority);

    signals:
        void currentSchedulerChanged();
        void isRunningChanged();
        void timeQuantumChanged();
        void simulationUpdated();
        void ganttBlocksChanged();
        void readyQueueChanged();
        void threadListChanged();
        void currentTimeChanged();

    private:
        QString m_currentScheduler{ "FCFS" };
        bool m_isRunning{ false };
        int m_timeQuantum{ 2 };
        int m_currentTime{ 0 };
        int m_nextThreadId{ 0 };
        QVariantList m_ganttBlocks;
        QVariantList m_readyQueue;
        QVariantList m_threadList;
        std::shared_ptr<core::Scheduler> m_scheduler;
        QTimer* m_timer{ nullptr };

        void updateReadyQueue();
        void updateThreadList();
        void appendGanttBlock(int threadId, const QString& name, int startTime, int duration, const QString& color);
    };
}

#endif // GUI_CONTROLLER_H