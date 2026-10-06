#include "gui/GUIController.h"
#include "core/FCFSScheduler.h"
#include "core/RRScheduler.h"
#include <QDebug>

namespace gui 
{
    GUIController::GUIController(QObject* parent) : QObject(parent) 
    {
        m_scheduler = std::make_shared<core::FCFSScheduler>();
    }

    void GUIController::setCurrentScheduler(const QString& scheduler) 
    {
        if (m_currentScheduler != scheduler) 
        {
            m_currentScheduler = scheduler;
            if (m_currentScheduler == "FCFS") 
            {
                m_scheduler = std::make_shared<core::FCFSScheduler>();
            }
            else if (m_currentScheduler == "Round Robin") 
            {
                m_scheduler = std::make_shared<core::RRScheduler>(m_timeQuantum);
            }
            emit currentSchedulerChanged();
            resetSimulation();
        }
    }

    void GUIController::setTimeQuantum(int quantum)
    {
        if (m_timeQuantum != quantum)
        {
            m_timeQuantum = quantum;
            if (m_currentScheduler == "Round Robin") 
            {
                m_scheduler = std::make_shared<core::RRScheduler>(m_timeQuantum);
            }
            emit timeQuantumChanged();
        }
    }

    void GUIController::startSimulation() 
    {
        m_isRunning = true;
        emit isRunningChanged();
        qDebug() << "Simulation started";
    }

    void GUIController::pauseSimulation()
    {
        m_isRunning = false;
        emit isRunningChanged();
        qDebug() << "Simulation paused";
    }

    void GUIController::stepSimulation() 
    {
        if (m_scheduler) 
        {
            m_scheduler->tick();
            emit simulationUpdated();
            qDebug() << "Simulation step executed";
        }
    }

    void GUIController::resetSimulation()
    {
        m_isRunning = false;
        emit isRunningChanged();
        if (m_currentScheduler == "FCFS") 
        {
            m_scheduler = std::make_shared<core::FCFSScheduler>();
        }
        else
        {
            m_scheduler = std::make_shared<core::RRScheduler>(m_timeQuantum);
        }
        emit simulationUpdated();
        qDebug() << "Simulation reset";
    }

    void GUIController::addThread(int id, int burstTime, int priority)
    {
        if (m_scheduler)
        {
            auto thread = std::make_shared<core::Thread>(
                static_cast<uint32_t>(id),
                static_cast<uint32_t>(burstTime),
                static_cast<uint32_t>(priority)
            );
            m_scheduler->addThread(thread);
            emit simulationUpdated();
            qDebug() << "Added thread ID:" << id << "Burst:" << burstTime << "Priority:" << priority;
        }
    }

}