#ifndef BENCHMARK_RESULTS_MODEL_H
#define BENCHMARK_RESULTS_MODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QString>

namespace gui
{
    struct BenchmarkResultUIData
    {
        QString schedulerName;
        uint64_t totalTicks{ 0 };
        double throughput{ 0.0 };
        int contextSwitches{ 0 };
        double jainsFairness{ 0.0 };
        double avgWaiting{ 0.0 };
        double avgTurnaround{ 0.0 };
    };

    class BenchmarkResultsModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum BenchmarkRoles
        {
            SchedulerNameRole = Qt::UserRole + 1,
            TotalTicksRole,
            ThroughputRole,
            ContextSwitchesRole,
            JainsFairnessRole,
            AvgWaitingRole,
            AvgTurnaroundRole
        };

        explicit BenchmarkResultsModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        void setResults(const std::vector<BenchmarkResultUIData>& results);
        void updateResults(const QVariantList& variantList);
        void clear();

    private:
        std::vector<BenchmarkResultUIData> m_results;
    };
}

#endif // BENCHMARK_RESULTS_MODEL_H