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

    void GUIController::appendGanttBlock(int threadId, const QString& name, int startTime, int duration, const QString& color)
    {
        QVariantMap block;
        block["threadId"] = threadId;
        block["name"] = name;
        block["startTime"] = startTime;
        block["duration"] = duration;
        block["color"] = color;

        m_ganttBlocks.append(block);
        emit ganttBlocksChanged();
    }

    void GUIController::updateReadyQueue()
    {
        m_readyQueue.clear();
        if (m_scheduler)
        {
            const auto& allThreads = m_scheduler->getAllThreads();
            auto currentTh = m_scheduler->getCurrentThread();

            static QStringList palette = { "#4caf50", "#2196f3", "#ff9800", "#e91e63", "#9c27b0", "#00bcd4" };

            for (const auto& th : allThreads)
            {
                if (th)
                {
                    bool isRunning = (currentTh && currentTh->id == th->id);
                    bool isReady = (th->state == core::ThreadState::READY) ||
                        (!isRunning && th->remainingTime > 0);

                    if (isReady)
                    {
                        QVariantMap item;
                        item["id"] = static_cast<int>(th->id);
                        item["burstTime"] = static_cast<int>(th->remainingTime);
                        item["priority"] = static_cast<int>(th->priority);
                        item["color"] = palette[th->id % palette.size()];
                        m_readyQueue.append(item);
                    }
                }
            }
        }
        emit readyQueueChanged();
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

            auto activeThread = m_scheduler->getCurrentThread();
            if (activeThread)
            {
                static QStringList palette = { "#4caf50", "#2196f3", "#ff9800", "#e91e63", "#9c27b0", "#00bcd4" };
                uint32_t threadId = activeThread->id;
                QString color = palette[threadId % palette.size()];

                appendGanttBlock(
                    static_cast<int>(threadId),
                    QString("T%1").arg(threadId),
                    m_currentTime,
                    1,
                    color
                );
            }

            m_currentTime++;
            updateReadyQueue(); // Обновляем Ready Queue после тика
            emit currentTimeChanged();
            emit simulationUpdated();
            qDebug() << "Simulation step executed, time:" << m_currentTime;
        }
    }

    void GUIController::resetSimulation()
    {
        m_isRunning = false;
        m_currentTime = 0;
        m_ganttBlocks.clear();

        if (m_currentScheduler == "FCFS")
        {
            m_scheduler = std::make_shared<core::FCFSScheduler>();
        }
        else
        {
            m_scheduler = std::make_shared<core::RRScheduler>(m_timeQuantum);
        }

        updateReadyQueue();
        emit isRunningChanged();
        emit currentTimeChanged();
        emit ganttBlocksChanged();
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
            updateReadyQueue(); // Сразу отображаем добавленный поток
            emit simulationUpdated();
            qDebug() << "Added thread ID:" << id << "Burst:" << burstTime << "Priority:" << priority;
        }
    }
}