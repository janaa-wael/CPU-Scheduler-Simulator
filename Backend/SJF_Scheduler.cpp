#include <iostream>
#include <functional>
#include "Scheduler.h"
#include "SJF_Scheduler.h"

int ticks;

bool SJF_Scheduler::allProcessesComplete() const
{
    for(auto& p : allProcesses)
    {
        if(!p->getIsComplete()) return false;
    }
    return true;
}

int SJF_Scheduler::findNextArrivalTime() const
{
    int next_arrival = INT_MAX;
    for(auto&p : allProcesses)
    {
        if(!p->getIsComplete() && p->getArrivalTime() > timeCounter)
        {
            next_arrival = min(next_arrival, p->getArrivalTime());
        }
        return next_arrival;
    }
}


SJF_Scheduler::SJF_Scheduler(bool preemptive)
: Scheduler(ShortestJobFirstComparator(), preemptive, "Shortest-Job-First Scheduler")
{

}
bool SJF_Scheduler::runOneStep()
{
    // 1) Bring in any processes that arrived at the current time
    updateReadyQueue();

    // 2) If the current process is already finished, clear it
    // (metrics should have been set at the moment it hit 0)
    if (currentProcess && currentProcess->getRemainingTime() <= 0)
    {
        currentProcess = nullptr;
    }

    // 3) Preemption (SRTF): if a shorter remaining-time process is ready, preempt
    if (preemptive && currentProcess && !readyQueue.empty())
    {
        auto shortestReady = readyQueue.top(); // should be the smallest remaining time
        if (shortestReady->getRemainingTime() < currentProcess->getRemainingTime())
        {
            readyQueue.push(currentProcess);
            currentProcess = nullptr;
        }
    }

    // 4) If CPU is idle, try to pick the next process; otherwise advance time by 1 (idle tick)
    if (!currentProcess)
    {
        if (!readyQueue.empty())
        {
            // IMPORTANT: pick ONE pop strategy. Here: pop outside.
            currentProcess = readyQueue.top();
            readyQueue.pop();

            if (currentProcess->getStartTime() < 0)
                currentProcess->setStartTime(timeCounter);
        }
        else
        {
            // Nothing ready: either we are done, or this step is CPU idle
            if (isSimulationComplete())
                return true;

            timeCounter++;          // consume one time unit as idle
            updateReadyQueue();     // optional: catch arrivals at the new time
            return false;
        }
    }

    // 5) Execute exactly one time unit
    timeCounter++;
    int remaining = currentProcess->getRemainingTime() - 1;
    currentProcess->setRemainingTime(remaining);

    // 6) If finished now, finalize metrics
    if (remaining <= 0)
    {
        currentProcess->setCompletionTime(timeCounter);
        currentProcess->setIsComplete(true);

        currentProcess->setTurnAroundTime(
            currentProcess->getCompletionTime() - currentProcess->getArrivalTime()
        );
        currentProcess->setWaitingTime(
            currentProcess->getTurnAroundTime() - currentProcess->getBurstTime()
        );

        // Optional: keep it for UI for 1 tick, or clear immediately.
        // If you want it cleared immediately, uncomment:
        // currentProcess = nullptr;
    }

    // 7) Update averages (could be done only at the end, but OK for live UI)
    calculateAvgWaitingTime();
    calculateAvgTurnAroundTime();

    return isSimulationComplete();
}


void SJF_Scheduler::runStatic(int runUntilTime = -1)
{
    bool isLiveMode = runUntilTime < 0;
    shared_ptr<Process> currentProcess = nullptr;

    while(isLiveMode || timeCounter < runUntilTime)
    {
        updateReadyQueue();

        if(readyQueue.empty() && !currentProcess)
        {
            if(allProcessesComplete()) break;

            int nextArrival = findNextArrivalTime();
            if(nextArrival > timeCounter) timeCounter = nextArrival;

            continue;
        }

        if(!currentProcess || currentProcess->getRemainingTime() == 0 || (preemptive && !readyQueue.empty() && readyQueue.top()->getRemainingTime() < currentProcess->getRemainingTime())) 
        {
            if(currentProcess && currentProcess->getRemainingTime() > 0)
            {
                readyQueue.push(currentProcess);
            }
            currentProcess = selectNextProcess();
            readyQueue.pop();
            if(currentProcess->getStartTime() < 0)
                currentProcess->setStartTime(timeCounter);
        }

        int executionTime = preemptive ? 1 : currentProcess->getRemainingTime();

        if(!isLiveMode && timeCounter + executionTime > runUntilTime)
        {
            executionTime = runUntilTime - timeCounter;
        }
        timeCounter += executionTime;
        int remaining = currentProcess->getRemainingTime();
        currentProcess->setRemainingTime(remaining);

        if(remaining <= 0)
        {
            currentProcess->setCompletionTime(timeCounter);
            currentProcess->setIsComplete(true);
            currentProcess->setTurnAroundTime(currentProcess->getCompletionTime() - currentProcess->getArrivalTime());
            currentProcess->setWaitingTime(currentProcess->getTurnAroundTime() - currentProcess->getBurstTime());
            currentProcess = nullptr;
        }
        if(!isLiveMode && timeCounter >= runUntilTime) break;
    }
    calculateAvgTurnAroundTime();
    calculateAvgWaitingTime();
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