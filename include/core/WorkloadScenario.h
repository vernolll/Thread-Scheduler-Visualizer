#ifndef WORKLOAD_SCENARIO_H
#define WORKLOAD_SCENARIO_H

#include <vector>
#include <string>
#include <cstdint>

namespace core
{
    struct ScenarioThreadConfig
    {
        uint32_t id;
        uint32_t burstTime;
        uint32_t priority;
    };

    struct WorkloadScenario
    {
        std::string name;
        std::string description;
        std::vector<ScenarioThreadConfig> threads;
    };

    class WorkloadScenarioManager
    {
    public:
        static std::vector<WorkloadScenario> getPredefinedScenarios();
    };
}

#endif // WORKLOAD_SCENARIO_H