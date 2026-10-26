#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <vector>
#include <string>
#include "core/SimulationEvent.h"

namespace core
{
    class EventLogger
    {
    public:
        EventLogger() = default;
        ~EventLogger() = default;

        void logEvent(uint64_t tick, uint32_t threadId, ThreadState prev, ThreadState next, const std::string& reason = "");

        const std::vector<SimulationEvent>& getEvents() const;

        std::vector<SimulationEvent> getEventsForThread(uint32_t threadId) const;

        void clear();

        size_t size() const;

    private:
        std::vector<SimulationEvent> m_events;
    };
}

#endif // EVENT_LOGGER_H