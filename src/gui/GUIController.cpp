#include "gui/GUIController.h"
#include "core/WorkloadScenario.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QQmlContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

namespace gui
{
    GUIController::GUIController(QObject* parent) : QObject(parent)
    {
        m_worker = new core::SimulationWorker();
        m_worker->moveToThread(&m_workerThread);

        connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);

        connect(m_worker, &core::SimulationWorker::currentTimeChanged, this, &GUIController::onCurrentTimeChanged, Qt::QueuedConnection);
        connect(m_worker, &core::SimulationWorker::isRunningChanged, this, &GUIController::onIsRunningChanged, Qt::QueuedConnection);
        connect(m_worker, &core::SimulationWorker::readyQueueUpdated, this, &GUIController::onReadyQueueUpdated, Qt::QueuedConnection);
        connect(m_worker, &core::SimulationWorker::threadListUpdated, this, &GUIController::onThreadListUpdated, Qt::QueuedConnection);
        connect(m_worker, &core::SimulationWorker::ganttBlockAppended, this, &GUIController::onGanttBlockAppended, Qt::QueuedConnection);
        connect(m_worker, &core::SimulationWorker::ganttBlocksCleared, this, &GUIController::onGanttBlocksCleared, Qt::QueuedConnection);
        connect(m_worker, &core::SimulationWorker::benchmarkResultsUpdated, this, &GUIController::onBenchmarkResultsUpdated, Qt::QueuedConnection);

        m_workerThread.start();

        m_timer = new QTimer(this);
        m_timer->setInterval(1000);
        connect(m_timer, &QTimer::timeout, this, &GUIController::stepSimulation);

        QMetaObject::invokeMethod(m_worker, "setCurrentScheduler", Qt::QueuedConnection,
            Q_ARG(QString, m_currentScheduler), Q_ARG(int, m_timeQuantum));
    }

    GUIController::~GUIController()
    {
        if (m_timer->isActive())
        {
            m_timer->stop();
        }
        m_workerThread.quit();
        m_workerThread.wait();
    }

    void GUIController::setCurrentScheduler(const QString& scheduler)
    {
        if (m_currentScheduler != scheduler)
        {
            m_currentScheduler = scheduler;
            emit currentSchedulerChanged();

            QMetaObject::invokeMethod(m_worker, "setCurrentScheduler", Qt::QueuedConnection,
                Q_ARG(QString, m_currentScheduler), Q_ARG(int, m_timeQuantum));
        }
    }

    void GUIController::setTimeQuantum(int quantum)
    {
        if (m_timeQuantum != quantum)
        {
            m_timeQuantum = quantum;
            emit timeQuantumChanged();

            QMetaObject::invokeMethod(m_worker, "setTimeQuantum", Qt::QueuedConnection,
                Q_ARG(int, m_timeQuantum));
        }
    }

    void GUIController::startSimulation()
    {
        if (!m_isRunning)
        {
            m_timer->start();
            QMetaObject::invokeMethod(m_worker, "startSimulation", Qt::QueuedConnection);
        }
    }

    void GUIController::pauseSimulation()
    {
        if (m_isRunning)
        {
            m_timer->stop();
            QMetaObject::invokeMethod(m_worker, "pauseSimulation", Qt::QueuedConnection);
        }
    }

    void GUIController::stepSimulation()
    {
        QMetaObject::invokeMethod(m_worker, "stepSimulation", Qt::QueuedConnection);
    }

    void GUIController::resetSimulation()
    {
        if (m_timer->isActive())
        {
            m_timer->stop();
        }
        m_nextThreadId = 0;
        m_lastLoadedThreads.clear();

        QMetaObject::invokeMethod(m_worker, "resetSimulation", Qt::QueuedConnection);
    }

    void GUIController::addThread(int id, int burstTime, int priority)
    {
        m_lastLoadedThreads.clear();
        QMetaObject::invokeMethod(m_worker, "addThread", Qt::QueuedConnection,
            Q_ARG(int, id), Q_ARG(int, burstTime), Q_ARG(int, priority));
    }

    void GUIController::runAlgorithmBenchmark()
    {
        QMetaObject::invokeMethod(m_worker, "runAlgorithmBenchmark", Qt::QueuedConnection);
    }

    QStringList GUIController::scenarioNames() const
    {
        QStringList list;
        auto scenarios = core::WorkloadScenarioManager::getPredefinedScenarios();
        for (const auto& s : scenarios)
        {
            list.append(QString::fromStdString(s.name));
        }
        return list;
    }

    void GUIController::loadScenario(int index)
    {
        auto scenarios = core::WorkloadScenarioManager::getPredefinedScenarios();
        if (index < 0 || index >= static_cast<int>(scenarios.size()))
            return;

        const auto& scenario = scenarios[index];
        m_lastLoadedThreads = scenario.threads;

        if (m_timer->isActive())
        {
            m_timer->stop();
        }

        QMetaObject::invokeMethod(m_worker, "loadScenario", Qt::QueuedConnection, Q_ARG(int, index));
    }

    void GUIController::onCurrentTimeChanged(int time)
    {
        m_currentTime = time;
        emit currentTimeChanged();
        emit simulationUpdated();
    }

    void GUIController::onIsRunningChanged(bool running)
    {
        if (m_isRunning != running)
        {
            m_isRunning = running;
            emit isRunningChanged();
        }
    }

    void GUIController::onReadyQueueUpdated(const QVariantList& queue)
    {
        m_readyQueue = queue;
        emit readyQueueChanged();
    }

    void GUIController::onThreadListUpdated(const QVariantList& threads)
    {
        m_threadList = threads;
        emit threadListChanged();
    }

    void GUIController::onGanttBlockAppended(int threadId, const QString& name, int startTime, int duration, const QString& color)
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

    void GUIController::onGanttBlocksCleared()
    {
        m_ganttBlocks.clear();
        emit ganttBlocksChanged();
    }

    void GUIController::onBenchmarkResultsUpdated(const QVariantList& results)
    {
        m_benchmarkResults = results;
        emit benchmarkResultsChanged();
    }

    void GUIController::saveToJson(const QString& filePath)
    {
        QString path = filePath;
        if (path.startsWith("file:///"))
        {
            path = path.mid(8);
        }
        else if (path.startsWith("file://"))
        {
            path = path.mid(7);
        }

        QJsonObject rootObj;
        rootObj["scheduler"] = m_currentScheduler;
        rootObj["timeQuantum"] = m_timeQuantum;

        QJsonArray threadsArray;
        for (const auto& itemVar : m_threadList)
        {
            QVariantMap item = itemVar.toMap();
            QJsonObject thObj;
            thObj["id"] = item["id"].toInt();
            thObj["burstTime"] = item["burstTime"].toInt();
            thObj["priority"] = item["priority"].toInt();
            threadsArray.append(thObj);
        }
        rootObj["threads"] = threadsArray;

        QJsonDocument doc(rootObj);
        QFile file(path);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(doc.toJson());
            file.close();
        }
    }

    void GUIController::loadFromJson(const QString& filePath)
    {
        QString path = filePath;
        if (path.startsWith("file:///"))
        {
            path = path.mid(8);
        }
        else if (path.startsWith("file://"))
        {
            path = path.mid(7);
        }

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || !doc.isObject())
        {
            return;
        }

        QJsonObject rootObj = doc.object();

        if (rootObj.contains("scheduler"))
        {
            setCurrentScheduler(rootObj["scheduler"].toString());
        }
        if (rootObj.contains("timeQuantum"))
        {
            setTimeQuantum(rootObj["timeQuantum"].toInt());
        }

        resetSimulation();

        if (rootObj.contains("threads") && rootObj["threads"].isArray())
        {
            QJsonArray threadsArray = rootObj["threads"].toArray();
            for (const auto& val : threadsArray)
            {
                QJsonObject thObj = val.toObject();
                int id = thObj["id"].toInt();
                int burst = thObj["burstTime"].toInt();
                int priority = thObj["priority"].toInt();
                addThread(id, burst, priority);
            }
        }
    }

    void GUIController::exportCsv(const QString& filePath)
    {
        QString path = filePath;
        if (path.startsWith("file:///"))
        {
            path = path.mid(8);
        }
        else if (path.startsWith("file://"))
        {
            path = path.mid(7);
        }

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }

        QTextStream out(&file);
        out << "Algorithm,TotalTicks,Throughput,ContextSwitches,JainsFairness,AvgWaiting,AvgTurnaround\n";

        for (const auto& itemVar : m_benchmarkResults) 
        {
            QVariantMap item = itemVar.toMap();
            out << item["schedulerName"].toString() << ","
                << item["totalTicks"].toInt() << ","
                << item["throughput"].toDouble() << ","
                << item["contextSwitches"].toInt() << ","
                << item["jainsFairness"].toDouble() << ","
                << item["avgWaiting"].toDouble() << ","
                << item["avgTurnaround"].toDouble() << "\n";
        }

        file.close();
    }
}