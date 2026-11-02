#ifndef THREAD_LIST_MODEL_H
#define THREAD_LIST_MODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QString>

namespace gui
{
    struct ThreadUIData
    {
        int id{ 0 };
        int priority{ 0 };
        int burstTime{ 0 };
        int remainingTime{ 0 };
        QString state{ "NEW" };
        QString color{ "#ffffff" };
    };

    class ThreadListModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum ThreadRoles
        {
            IdRole = Qt::UserRole + 1,
            PriorityRole,
            BurstTimeRole,
            RemainingTimeRole,
            StateRole,
            ColorRole
        };

        explicit ThreadListModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        void setThreads(const std::vector<ThreadUIData>& threads);
        void updateThreads(const QVariantList& variantList);
        void clear();

    private:
        std::vector<ThreadUIData> m_threads;
    };
}

#endif // THREAD_LIST_MODEL_H