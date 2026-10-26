#include "core/EventLogger.h"
#include <algorithm>

namespace core
{
    void EventLogger::logEvent(uint64_t tick, uint32_t threadId, ThreadState prev, ThreadState next, const std::string& reason)
    {
        m_events.emplace_back(tick, threadId, prev, next, reason);
    }

    const std::vector<SimulationEvent>& EventLogger::getEvents() const
    {
        return m_events;
    }

    std::vector<SimulationEvent> EventLogger::getEventsForThread(uint32_t threadId) const
    {
        std::vector<SimulationEvent> filtered;
        for (const auto& event : m_events)
        {
            if (event.threadId == threadId)
            {
                filtered.push_back(event);
            }
        }
        return filtered;
    }

    void EventLogger::clear()
    {
        m_events.clear();
    }

    size_t EventLogger::size() const
    {
        return m_events.size();
    }
}