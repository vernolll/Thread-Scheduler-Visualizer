#include "core/WorkloadScenario.h"

namespace core
{
    std::vector<WorkloadScenario> WorkloadScenarioManager::getPredefinedScenarios()
    {
        return {
            {
                "Convoy Effect",
                "One long CPU-bound thread followed by several short interactive threads.",
                {
                    {1, 18, 1},
                    {2, 2, 1},
                    {3, 2, 1},
                    {4, 3, 1}
                }
            },
            {
                "Starvation",
                "High-priority streams continuously arriving to starve low-priority threads.",
                {
                    {1, 10, 5},
                    {2, 8, 1},
                    {3, 6, 1},
                    {4, 12, 4}
                }
            },
            {
                "CPU-bound vs I/O-bound",
                "A mix of heavy computational tasks and short burst tasks.",
                {
                    {1, 15, 2},
                    {2, 2, 3},
                    {3, 14, 2},
                    {4, 3, 3},
                    {5, 1, 3}
                }
            },
            {
                "Mixed Load",
                "A balanced, diverse set of threads with varying burst times and priorities.",
                {
                    {1, 8, 2},
                    {2, 4, 1},
                    {3, 10, 3},
                    {4, 5, 2},
                    {5, 7, 1}
                }
            }
        };
    }
}