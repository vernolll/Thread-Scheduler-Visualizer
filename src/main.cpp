#include <iostream>
#include <memory>
#include "core/Thread.h"
#include "core/RRScheduler.h"

int main() 
{
    std::cout << "=== Thread Scheduler Visualizer - Core Round Robin (Quantum = 2) Test ===" << std::endl;

    core::RRScheduler scheduler(2); // Quantum = 2 ticks

    auto t1 = std::make_shared<core::Thread>(1u, 4u, 1u); // Burst = 4
    auto t2 = std::make_shared<core::Thread>(2u, 3u, 2u); // Burst = 3
    auto t3 = std::make_shared<core::Thread>(3u, 2u, 3u); // Burst = 2

    scheduler.addThread(t1);
    scheduler.addThread(t2);
    scheduler.addThread(t3);

    std::cout << "\nStarting Round Robin Simulation Steps...\n" << std::endl;

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