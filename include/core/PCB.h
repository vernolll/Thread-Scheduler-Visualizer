#ifndef PCB_H
#define PCB_H

#include "core/Thread.h"
#include <vector>
#include <string>

namespace core
{

    class ProcessControlBlock 
    {
    public:
        ProcessControlBlock() = default;
        ProcessControlBlock(uint32_t pid, std::string name);

        uint32_t getPid() const noexcept { return pid_; }
        const std::string& getName() const noexcept { return name_; }

        void addThread(const Thread& thread);
        std::vector<Thread>& getThreads() noexcept { return threads_; }
        const std::vector<Thread>& getThreads() const noexcept { return threads_; }

        bool isFinished() const;

    private:
        uint32_t pid_{ 0 };
        std::string name_;
        std::vector<Thread> threads_;
    };
}

#endif // PCB_H