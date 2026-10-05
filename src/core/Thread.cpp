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
        case ThreadState::BLOCKED:    return "BLOCKED";
        case ThreadState::TERMINATED: return "TERMINATED";
        default:                      return "UNKNOWN";
        }
    }

}