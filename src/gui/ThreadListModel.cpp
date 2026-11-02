#include "gui/ThreadListModel.h"
#include <QVariantMap>

namespace gui
{
    ThreadListModel::ThreadListModel(QObject* parent) : QAbstractListModel(parent) {}

    int ThreadListModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(m_threads.size());
    }

    QVariant ThreadListModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_threads.size()))
            return QVariant();

        const auto& item = m_threads[index.row()];

        switch (role)
        {
        case IdRole:
            return item.id;
        case PriorityRole:
            return item.priority;
        case BurstTimeRole:
            return item.burstTime;
        case RemainingTimeRole:
            return item.remainingTime;
        case StateRole:
            return item.state;
        case ColorRole:
            return item.color;
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> ThreadListModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[IdRole] = "threadId";
        roles[PriorityRole] = "priority";
        roles[BurstTimeRole] = "burstTime";
        roles[RemainingTimeRole] = "remainingTime";
        roles[StateRole] = "state";
        roles[ColorRole] = "color";
        return roles;
    }

    void ThreadListModel::setThreads(const std::vector<ThreadUIData>& threads)
    {
        beginResetModel();
        m_threads = threads;
        endResetModel();
    }

    void ThreadListModel::updateThreads(const QVariantList& variantList)
    {
        std::vector<ThreadUIData> newThreads;
        newThreads.reserve(variantList.size());

        for (const auto& var : variantList)
        {
            QVariantMap map = var.toMap();
            ThreadUIData data;
            data.id = map["id"].toInt();
            data.priority = map["priority"].toInt();
            data.burstTime = map["burstTime"].toInt();
            data.remainingTime = map["remainingTime"].toInt();
            data.state = map["state"].toString();
            data.color = map["color"].toString();
            newThreads.push_back(data);
        }

        setThreads(newThreads);
    }

    void ThreadListModel::clear()
    {
        beginResetModel();
        m_threads.clear();
        endResetModel();
    }
}