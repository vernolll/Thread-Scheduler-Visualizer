#include "gui/BenchmarkResultsModel.h"
#include <QVariantMap>

namespace gui
{
    BenchmarkResultsModel::BenchmarkResultsModel(QObject* parent) : QAbstractListModel(parent) {}

    int BenchmarkResultsModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(m_results.size());
    }

    QVariant BenchmarkResultsModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_results.size()))
            return QVariant();

        const auto& item = m_results[index.row()];

        switch (role)
        {
        case SchedulerNameRole:
            return item.schedulerName;
        case TotalTicksRole:
            return static_cast<qulonglong>(item.totalTicks);
        case ThroughputRole:
            return item.throughput;
        case ContextSwitchesRole:
            return item.contextSwitches;
        case JainsFairnessRole:
            return item.jainsFairness;
        case AvgWaitingRole:
            return item.avgWaiting;
        case AvgTurnaroundRole:
            return item.avgTurnaround;
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> BenchmarkResultsModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[SchedulerNameRole] = "schedulerName";
        roles[TotalTicksRole] = "totalTicks";
        roles[ThroughputRole] = "throughput";
        roles[ContextSwitchesRole] = "contextSwitches";
        roles[JainsFairnessRole] = "jainsFairness";
        roles[AvgWaitingRole] = "avgWaiting";
        roles[AvgTurnaroundRole] = "avgTurnaround";
        return roles;
    }

    void BenchmarkResultsModel::setResults(const std::vector<BenchmarkResultUIData>& results)
    {
        beginResetModel();
        m_results = results;
        endResetModel();
    }

    void BenchmarkResultsModel::updateResults(const QVariantList& variantList)
    {
        std::vector<BenchmarkResultUIData> newResults;
        newResults.reserve(variantList.size());

        for (const auto& var : variantList)
        {
            QVariantMap map = var.toMap();
            BenchmarkResultUIData data;
            data.schedulerName = map["schedulerName"].toString();
            data.totalTicks = map["totalTicks"].toULongLong();
            data.throughput = map["throughput"].toDouble();
            data.contextSwitches = map["contextSwitches"].toInt();
            data.jainsFairness = map["jainsFairness"].toDouble();
            data.avgWaiting = map["avgWaiting"].toDouble();
            data.avgTurnaround = map["avgTurnaround"].toDouble();
            newResults.push_back(data);
        }

        setResults(newResults);
    }

    void BenchmarkResultsModel::clear()
    {
        beginResetModel();
        m_results.clear();
        endResetModel();
    }
}