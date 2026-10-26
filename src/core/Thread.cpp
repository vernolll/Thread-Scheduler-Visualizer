#include "core/Thread.h"

namespace core
{
    std::string toString(ThreadState state)
    {
        switch (state)
        {
        case ThreadState::NEW:        return "NEW";
        case ThreadState::READY:      return "READY";
        case ThreadState::RUNNING:    return "RUNNING";
        case ThreadState::WAITING:    return "WAITING";
        case ThreadState::BLOCKED:    return "BLOCKED";
        case ThreadState::TERMINATED: return "TERMINATED";
        default:                      return "UNKNOWN";
        }
    }

    bool isValidStateTransition(ThreadState from, ThreadState to)
    {
        if (from == to) return true;

        switch (from)
        {
        case ThreadState::NEW:
            return (to == ThreadState::READY || to == ThreadState::TERMINATED);

        case ThreadState::READY:
            return (to == ThreadState::RUNNING || to == ThreadState::TERMINATED);

        case ThreadState::RUNNING:
            return (to == ThreadState::READY || to == ThreadState::WAITING || to == ThreadState::BLOCKED || to == ThreadState::TERMINATED);

        case ThreadState::WAITING:
            return (to == ThreadState::READY || to == ThreadState::TERMINATED);

        case ThreadState::BLOCKED:
            return (to == ThreadState::READY || to == ThreadState::TERMINATED);

        case ThreadState::TERMINATED:
            return false;
        }
        return false;
    }
}