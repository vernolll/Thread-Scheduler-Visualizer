#include <iostream>
#include <memory>
#include "core/Thread.h"
#include "core/FCFSScheduler.h"

int main() 
{
    std::cout << "=== Thread Scheduler Visualizer - Core FCFS Test ===" << std::endl;

    core::FCFSScheduler scheduler;

    auto t1 = std::make_shared<core::Thread>(1u, 3u, 1u);
    auto t2 = std::make_shared<core::Thread>(2u, 5u, 2u);
    auto t3 = std::make_shared<core::Thread>(3u, 2u, 3u);

    scheduler.addThread(t1);
    scheduler.addThread(t2);
    scheduler.addThread(t3);

    std::cout << "\nStarting FCFS Simulation Steps...\n" << std::endl;

    int tickCount = 0;
    while (!scheduler.isFinished()) 
    {
        tickCount++;
        scheduler.tick();

        auto current = scheduler.getCurrentThread();
        if (current) 
        {
            std::cout << "[Tick " << tickCount << "] Running Thread ID="
                << current->id
                << " | Priority=" << current->priority
                << " | Remaining Time=" << current->remainingTime << std::endl;
        }
        else 
        {
            std::cout << "[Tick " << tickCount << "] CPU Idle" << std::endl;
        }
    }

    std::cout << "\nSimulation finished successfully in " << tickCount << " ticks!" << std::endl;

    return 0;
}