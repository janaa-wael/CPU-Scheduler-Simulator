#include <iostream>
#include "Process.h"
#include "Scheduler.h"
#include "FCFS_Scheduler.h"

using namespace std;


FCFS_Scheduler::FCFS_Scheduler(bool isPreemptive) 
: Scheduler(FirstArrivalComparator(), isPreemptive, "First-Come-First-Served Scheduler")
{

}

void FCFS_Scheduler::runStatic(int runUntilTime = -1)
{
    // This is the same as the current run method
    bool isLiveMode = (runUntilTime < 0);

    while ((isLiveMode || timeCounter < runUntilTime)) {
        updateReadyQueue();

        if (readyQueue.empty()) {
            if (allProcessesComplete())
                break;

            int nextArrival = findNextArrivalTime();
            if (nextArrival > timeCounter)
                timeCounter = nextArrival;

            continue;
        }

        auto p = selectNextProcess();
        readyQueue.pop();

        if (p->getStartTime() < 0)
            p->setStartTime(timeCounter);

        timeCounter += p->getRemainingTime();
        p->setCompletionTime(timeCounter);
        p->setRemainingTime(0);
        p->setIsComplete(true);

        p->setTurnAroundTime(p->getCompletionTime() - p->getArrivalTime());
        p->setWaitingTime(p->getTurnAroundTime() - p->getBurstTime());
    }

    calculateAvgWaitingTime();
    calculateAvgTurnAroundTime();
}

shared_ptr<Process> FCFS_Scheduler::selectNextProcess()
{
    if(!readyQueue.empty())
    {
        shared_ptr<Process> p = readyQueue.top();
        readyQueue.pop();
        return p;
    }
    return nullptr;
}

bool FCFS_Scheduler::allProcessesComplete() const
{

}

int FCFS_Scheduler::findNextArrivalTime() const
{
    int next_arrival = INT_MAX;
    for(const auto& p: allProcesses)
    {
        if(!p->getIsComplete() && p->getArrivalTime() > timeCounter)
        {
            next_arrival = min(next_arrival, p->getArrivalTime());
        }
    }
    return next_arrival;
}
