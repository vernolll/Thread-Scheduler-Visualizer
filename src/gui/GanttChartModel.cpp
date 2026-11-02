#include "gui/GanttChartModel.h"

namespace gui
{
    GanttChartModel::GanttChartModel(QObject* parent) : QAbstractListModel(parent) {}

    int GanttChartModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(m_blocks.size());
    }

    QVariant GanttChartModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_blocks.size()))
            return QVariant();

        const auto& block = m_blocks[index.row()];

        switch (role)
        {
        case ThreadIdRole:
            return block.threadId;
        case NameRole:
            return block.name;
        case StartTimeRole:
            return block.startTime;
        case DurationRole:
            return block.duration;
        case ColorRole:
            return block.color;
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> GanttChartModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[ThreadIdRole] = "threadId";
        roles[NameRole] = "name";
        roles[StartTimeRole] = "startTime";
        roles[DurationRole] = "duration";
        roles[ColorRole] = "color";
        return roles;
    }

    void GanttChartModel::setBlocks(const std::vector<GanttBlockUIData>& blocks)
    {
        beginResetModel();
        m_blocks = blocks;
        endResetModel();
    }

    void GanttChartModel::appendBlock(int threadId, const QString& name, int startTime, int duration, const QString& color)
    {
        beginInsertRows(QModelIndex(), static_cast<int>(m_blocks.size()), static_cast<int>(m_blocks.size()));
        m_blocks.push_back({ threadId, name, startTime, duration, color });
        endInsertRows();
    }

    void GanttChartModel::clear()
    {
        beginResetModel();
        m_blocks.clear();
        endResetModel();
    }
}