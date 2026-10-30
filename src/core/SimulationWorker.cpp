#include "core/SimulationWorker.h"
#include "core/SchedulerFactory.h"
#include "core/SchedulerTypes.h"
#include "core/RRScheduler.h"
#include "core/BenchmarkRunner.h"
#include <QStringList>

namespace core
{
    SimulationWorker::SimulationWorker(QObject* parent) : QObject(parent)
    {
        m_scheduler = SchedulerFactory::createScheduler(SchedulerType::FCFS);
    }

    void SimulationWorker::setCurrentScheduler(const QString& scheduler, int timeQuantum)
    {
        m_currentScheduler = scheduler;
        m_timeQuantum = timeQuantum;

        if (m_currentScheduler == "FCFS")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::FCFS);
        }
        else if (m_currentScheduler == "Round Robin")
        {
            m_scheduler = std::make_shared<core::RRScheduler>(m_timeQuantum);
        }
        else if (m_currentScheduler == "Priority")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::Priority);
        }
        else if (m_currentScheduler == "SJF")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::SJF);
        }
        else if (m_currentScheduler == "SRTF")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::SRTF);
        }
        else if (m_currentScheduler == "MLFQ")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::MLFQ);
        }

        resetSimulation();
    }

    void SimulationWorker::setTimeQuantum(int quantum)
    {
        m_timeQuantum = quantum;
        if (m_currentScheduler == "Round Robin")
        {
            m_scheduler = std::make_shared<core::RRScheduler>(m_timeQuantum);
        }
    }

    void SimulationWorker::startSimulation()
    {
        if (!m_isRunning)
        {
            m_isRunning = true;
            emit isRunningChanged(true);
        }
    }

    void SimulationWorker::pauseSimulation()
    {
        if (m_isRunning)
        {
            m_isRunning = false;
            emit isRunningChanged(false);
        }
    }

    void SimulationWorker::stepSimulation()
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

                emit ganttBlockAppended(
                    static_cast<int>(threadId),
                    QString("T%1").arg(threadId),
                    m_currentTime,
                    1,
                    color
                );
            }

            m_currentTime++;
            updateReadyQueue();
            updateThreadList();
            emit currentTimeChanged(m_currentTime);
        }
    }

    void SimulationWorker::resetSimulation()
    {
        m_isRunning = false;
        emit isRunningChanged(false);

        m_currentTime = 0;
        emit currentTimeChanged(m_currentTime);

        emit ganttBlocksCleared();

        if (m_currentScheduler == "FCFS")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::FCFS);
        }
        else if (m_currentScheduler == "Round Robin")
        {
            m_scheduler = std::make_shared<core::RRScheduler>(m_timeQuantum);
        }
        else if (m_currentScheduler == "Priority")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::Priority);
        }
        else if (m_currentScheduler == "SJF")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::SJF);
        }
        else if (m_currentScheduler == "SRTF")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::SRTF);
        }
        else if (m_currentScheduler == "MLFQ")
        {
            m_scheduler = SchedulerFactory::createScheduler(SchedulerType::MLFQ);
        }

        updateReadyQueue();
        updateThreadList();
    }

    void SimulationWorker::addThread(int id, int burstTime, int priority)
    {
        if (m_scheduler)
        {
            auto thread = std::make_shared<core::Thread>(
                static_cast<uint32_t>(id),
                static_cast<uint32_t>(burstTime),
                static_cast<uint32_t>(priority)
            );
            m_scheduler->addThread(thread);
            updateReadyQueue();
            updateThreadList();
        }
    }

    void SimulationWorker::loadScenario(int index)
    {
        auto scenarios = WorkloadScenarioManager::getPredefinedScenarios();
        if (index < 0 || index >= static_cast<int>(scenarios.size()))
            return;

        resetSimulation();

        const auto& scenario = scenarios[index];
        for (const auto& t : scenario.threads)
        {
            auto thread = std::make_shared<core::Thread>(
                static_cast<uint32_t>(t.id),
                static_cast<uint32_t>(t.burstTime),
                static_cast<uint32_t>(t.priority)
            );
            m_scheduler->addThread(thread);
        }

        updateReadyQueue();
        updateThreadList();
    }

    void SimulationWorker::runAlgorithmBenchmark()
    {
        if (!m_scheduler) return;

        auto baselineThreads = m_scheduler->getAllThreads();
        auto benchmarkResults = BenchmarkRunner::runBenchmark(baselineThreads);

        QVariantList resultsList;
        for (const auto& res : benchmarkResults)
        {
            QVariantMap item;
            item["schedulerName"] = QString::fromStdString(res.schedulerName);
            item["totalTicks"] = static_cast<int>(res.totalTicks);
            item["success"] = res.success;
            item["throughput"] = res.metrics.throughput;
            item["contextSwitches"] = static_cast<int>(res.metrics.contextSwitches);
            item["jainsFairness"] = res.metrics.jainsFairnessIndex;
            item["avgTurnaround"] = res.metrics.averageTurnaroundTime;
            item["avgWaiting"] = res.metrics.averageWaitingTime;

            resultsList.append(item);
        }

        emit benchmarkResultsUpdated(resultsList);
    }

    void SimulationWorker::updateReadyQueue()
    {
        QVariantList readyQueue;
        if (m_scheduler)
        {
            const auto& allThreads = m_scheduler->getAllThreads();
            static QStringList palette = { "#4caf50", "#2196f3", "#ff9800", "#e91e63", "#9c27b0", "#00bcd4" };

            for (const auto& th : allThreads)
            {
                if (th && th->state == ThreadState::READY)
                {
                    QVariantMap item;
                    item["id"] = static_cast<int>(th->id);
                    item["burstTime"] = static_cast<int>(th->remainingTime);
                    item["priority"] = static_cast<int>(th->priority);
                    item["color"] = palette[th->id % palette.size()];
                    readyQueue.append(item);
                }
            }
        }
        emit readyQueueUpdated(readyQueue);
    }

    void SimulationWorker::updateThreadList()
    {
        QVariantList threadList;
        if (m_scheduler)
        {
            const auto& allThreads = m_scheduler->getAllThreads();
            static QStringList palette = { "#4caf50", "#2196f3", "#ff9800", "#e91e63", "#9c27b0", "#00bcd4" };

            for (const auto& th : allThreads)
            {
                if (th)
                {
                    QVariantMap item;
                    item["id"] = static_cast<int>(th->id);
                    item["priority"] = static_cast<int>(th->priority);
                    item["burstTime"] = static_cast<int>(th->burstTime);
                    item["remainingTime"] = static_cast<int>(th->remainingTime);
                    item["state"] = QString::fromStdString(toString(th->state));
                    item["color"] = palette[th->id % palette.size()];

                    threadList.append(item);
                }
            }
        }
        emit threadListUpdated(threadList);
    }
}