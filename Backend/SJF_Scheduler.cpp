#include <iostream>
#include <functional>
#include "Scheduler.h"
#include "SJF_Scheduler.h"


SJF_Scheduler::SJF_Scheduler(bool preemptive)
: Scheduler(ShortestJobFirstComparator(), preemptive, "Shortest-Job-First Scheduler")
{

}

void SJF_Scheduler::run(int runUntilTime = -1)
{
    
}

shared_ptr<Process> SJF_Scheduler::selectNextProcess()
{
    if(!readyQueue.empty())   
    {
        auto next_process = readyQueue.top();
        readyQueue.pop();
        return next_process;
    }
    return nullptr;
}