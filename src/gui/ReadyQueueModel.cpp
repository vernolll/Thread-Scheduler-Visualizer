#include "gui/ReadyQueueModel.h"
#include <QVariantMap>

namespace gui
{
    ReadyQueueModel::ReadyQueueModel(QObject* parent) : QAbstractListModel(parent) {}

    int ReadyQueueModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(m_items.size());
    }

    QVariant ReadyQueueModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_items.size()))
            return QVariant();

        const auto& item = m_items[index.row()];

        switch (role)
        {
        case IdRole:
            return item.id;
        case PriorityRole:
            return item.priority;
        case BurstTimeRole:
            return item.burstTime;
        case LevelRole:
            return item.level;
        case ColorRole:
            return item.color;
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> ReadyQueueModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[IdRole] = "threadId";
        roles[PriorityRole] = "priority";
        roles[BurstTimeRole] = "burstTime";
        roles[LevelRole] = "queueLevel";
        roles[ColorRole] = "color";
        return roles;
    }

    void ReadyQueueModel::setItems(const std::vector<ReadyQueueItemUIData>& items)
    {
        beginResetModel();
        m_items = items;
        endResetModel();
    }

    void ReadyQueueModel::updateItems(const QVariantList& variantList)
    {
        std::vector<ReadyQueueItemUIData> newItems;
        newItems.reserve(variantList.size());

        for (const auto& var : variantList)
        {
            QVariantMap map = var.toMap();
            ReadyQueueItemUIData data;
            data.id = map["id"].toInt();
            data.priority = map["priority"].toInt();
            data.burstTime = map["burstTime"].toInt();
            data.level = map.contains("level") ? map["level"].toInt() : 0;
            data.color = map["color"].toString();
            newItems.push_back(data);
        }

        setItems(newItems);
    }

    void ReadyQueueModel::clear()
    {
        beginResetModel();
        m_items.clear();
        endResetModel();
    }
}