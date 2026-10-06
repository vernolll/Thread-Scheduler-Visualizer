#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include <QObject>
#include <QString>
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

    public:
        explicit GUIController(QObject* parent = nullptr);

        QString currentScheduler() const { return m_currentScheduler; }
        void setCurrentScheduler(const QString& scheduler);

        bool isRunning() const { return m_isRunning; }
        int timeQuantum() const { return m_timeQuantum; }
        void setTimeQuantum(int quantum);

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

    private:
        QString m_currentScheduler{ "FCFS" };
        bool m_isRunning{ false };
        int m_timeQuantum{ 2 };
        std::shared_ptr<core::Scheduler> m_scheduler;
    };

}

#endif // GUI_CONTROLLER_H