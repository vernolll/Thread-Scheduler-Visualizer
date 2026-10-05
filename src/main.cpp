#include "core/Thread.h"
#include "core/PCB.h"
#include <iostream>

int main() 
{
    core::ProcessControlBlock pcb(1, "MainProcess");
    pcb.addThread(core::Thread(1, 1, 5, 0, 10));

    std::cout << "Process " << pcb.getName() << " initialized with "
        << pcb.getThreads().size() << " thread(s).\n";
    std::cout << "Initial Thread State: " << core::toString(pcb.getThreads()[0].state) << "\n";

    return 0;
}