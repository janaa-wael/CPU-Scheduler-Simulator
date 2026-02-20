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

bool FCFS_Scheduler::runOneStep() 
{
    updateReadyQueue();

    bool processCompleted = false;

    if(currentProcess && currentProcess->getRemainingTime() <= 0)
    {
        currentProcess = nullptr;
    }

    if(!currentProcess)
    {
        if(!readyQueue.empty())
        {
            currentProcess = selectNextProcess();
            readyQueue.pop();
        }
        if (currentProcess->getStartTime() < 0)
            currentProcess->setStartTime(timeCounter);
        else
        {
            if(isSimulationComplete())
            {
                return true;
            }

            timeCounter++;
            updateReadyQueue();
            return false;
        }
    }

    timeCounter++;
    int remaining = currentProcess->getRemainingTime();
    currentProcess->setRemainingTime(remaining);

    if(remaining <= 0)
    {
        currentProcess->setCompletionTime(timeCounter);
        currentProcess->setIsComplete(true);

        currentProcess->setTurnAroundTime(currentProcess->getCompletionTime() - currentProcess->getArrivalTime());
        currentProcess->setWaitingTime(currentProcess->getTurnAroundTime() - currentProcess->getBurstTime());
    }

    calculateAvgTurnAroundTime();
    calculateAvgWaitingTime();

    return isSimulationComplete();
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
    for(auto& p : allProcesses)
    {
        if(!p->getIsComplete()) return false;
    }
    return true;
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
