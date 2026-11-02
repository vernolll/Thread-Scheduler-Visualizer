#ifndef GANTT_CHART_MODEL_H
#define GANTT_CHART_MODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QString>

namespace gui
{
    struct GanttBlockUIData
    {
        int threadId{ 0 };
        QString name;
        int startTime{ 0 };
        int duration{ 0 };
        QString color{ "#ffffff" };
    };

    class GanttChartModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum GanttRoles
        {
            ThreadIdRole = Qt::UserRole + 1,
            NameRole,
            StartTimeRole,
            DurationRole,
            ColorRole
        };

        explicit GanttChartModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        void setBlocks(const std::vector<GanttBlockUIData>& blocks);
        void appendBlock(int threadId, const QString& name, int startTime, int duration, const QString& color);
        void clear();

    private:
        std::vector<GanttBlockUIData> m_blocks;
    };
}

#endif // GANTT_CHART_MODEL_H