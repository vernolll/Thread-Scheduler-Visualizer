#ifndef READY_QUEUE_MODEL_H
#define READY_QUEUE_MODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QString>

namespace gui
{
    struct ReadyQueueItemUIData
    {
        int id{ 0 };
        int priority{ 0 };
        int burstTime{ 0 };
        int level{ 0 };
        QString color;
    };

    class ReadyQueueModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum ReadyQueueRoles
        {
            IdRole = Qt::UserRole + 1,
            PriorityRole,
            BurstTimeRole,
            LevelRole,
            ColorRole
        };

        explicit ReadyQueueModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        void setItems(const std::vector<ReadyQueueItemUIData>& items);
        void updateItems(const QVariantList& variantList);
        void clear();

    private:
        std::vector<ReadyQueueItemUIData> m_items;
    };
}

#endif // READY_QUEUE_MODEL_H