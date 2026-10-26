#ifndef SIMULATION_EVENT_H
#define SIMULATION_EVENT_H

#include <cstdint>
#include <string>
#include "core/Thread.h"

namespace core
{
    struct SimulationEvent
    {
        uint64_t tick{ 0 };
        uint32_t threadId{ 0 };
        ThreadState prevState{ ThreadState::NEW };
        ThreadState newState{ ThreadState::NEW };
        std::string reason;

        SimulationEvent() = default;

        SimulationEvent(uint64_t t, uint32_t id, ThreadState prev, ThreadState next, std::string r)
            : tick(t), threadId(id), prevState(prev), newState(next), reason(std::move(r)) {
        }
    };
}

#endif // SIMULATION_EVENT_H