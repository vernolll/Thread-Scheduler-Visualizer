#include "core/PCB.h"
#include <algorithm>

namespace core 
{
    ProcessControlBlock::ProcessControlBlock(uint32_t pid, std::string name)
        : pid_(pid), name_(std::move(name)) 
    {
    }

    void ProcessControlBlock::addThread(const Thread& thread) {
        threads_.push_back(thread);
    }

    bool ProcessControlBlock::isFinished() const 
    {
        if (threads_.empty()) 
        {
            return true;
        }
        return std::all_of(threads_.begin(), threads_.end(), [](const Thread& t) 
            {
            return t.state == ThreadState::TERMINATED;
            });
    }
} 