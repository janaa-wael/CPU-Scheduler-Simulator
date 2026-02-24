#include <iostream>
#include "Priority_Scheduler.h"

bool Priority_Scheduler::allProcessesComplete() const
{
    for(auto& p : allProcesses)
    {
        if(!p->getIsComplete()) return false;
    }
    return true;
}

int Priority_Scheduler::findNextArrivalTime() const
{
    int next_arrival = INT_MAX;
    for(auto& p : allProcesses)
    {
        next_arrival = min(p->getArrivalTime(),next_arrival);
    }
    return next_arrival;
}

Priority_Scheduler::Priority_Scheduler(bool isPreemptive = false)
: Scheduler([](shared_ptr<Process> p1, shared_ptr<Process>p2){
    return p1->getId() > p2->getId();
    }, isPreemptive, "Priority Scheduler")
{

}

void Priority_Scheduler::runStatic(int runUntilTime = -1) 
{
    bool isLiveMode = (runUntilTime < 0);
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
        
        if(!currentProcess || currentProcess->getRemainingTime()
            || (preemptive && !readyQueue.empty() && 
            readyQueue.top()->getPriority() < currentProcess->getPriority()))
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

        int executionTime = preemptive? 1 : currentProcess->getRemainingTime();

        if(!isLiveMode && timeCounter + executionTime > runUntilTime)
        {
            executionTime = runUntilTime - timeCounter;
        }
        timeCounter += executionTime;
        int remaining = currentProcess->getRemainingTime() - executionTime;
        currentProcess->setRemainingTime(remaining);

        if(remaining<=0)
        {
            currentProcess->setCompletionTime(timeCounter);
            currentProcess->setIsComplete(true);
            currentProcess->setTurnAroundTime(currentProcess->getCompletionTime() - currentProcess->getBurstTime());
            currentProcess->setWaitingTime(currentProcess->getTurnAroundTime() - currentProcess->getBurstTime());
            currentProcess = nullptr;
        }
        if(!isLiveMode && timeCounter >= runUntilTime) break;
    }
    calculateAvgTurnAroundTime();
    calculateAvgWaitingTime();
}    
bool Priority_Scheduler::runOneStep() 
{
        // Update ready queue with any newly arrived processes
    updateReadyQueue();

    // If current process just completed in the last step
    if (currentProcess && currentProcess->getRemainingTime() <= 0) {
        // Process already marked as complete, but we kept the reference
        // for UI display purposes. Now we'll actually handle the completion.
        currentProcess = nullptr;
    }

    // Check preemption if necessary
    if (currentProcess && preemptive && !readyQueue.empty() &&
        readyQueue.top()->getPriority() < currentProcess->getPriority()) {
        // Preempt current process
        readyQueue.push(currentProcess);
        currentProcess = nullptr;
    }

    // If no current process, get the next one from the queue
    if (!currentProcess) {
        if (!readyQueue.empty()) {
            currentProcess = selectNextProcess();
            readyQueue.pop();

            // Set start time if this is first execution
            if (currentProcess->getStartTime() < 0)
                currentProcess->setStartTime(timeCounter);
        } else {
            // No process available, check if simulation is complete
            if (isSimulationComplete())
                return true;

            // Advance time by just 1 unit if no processes are ready
            timeCounter++;
            updateReadyQueue(); // Check again after advancing time

            // Return but don't mark as complete - this is an idle step
            return false;
        }
    }

    // Execute the current process for one time unit
    timeCounter++;
    int remaining = currentProcess->getRemainingTime() - 1;
    currentProcess->setRemainingTime(remaining);

    // Check if the process is completed
    if (remaining <= 0) {
        currentProcess->setCompletionTime(timeCounter);
        currentProcess->setIsComplete(true);

        // Calculate metrics
        currentProcess->setTurnAroundTime(currentProcess->getCompletionTime() - currentProcess->getArrivalTime());
        currentProcess->setWaitingTime(currentProcess->getTurnAroundTime() - currentProcess->getBurstTime());
    }

    // Calculate metrics after each step
    calculateAvgWaitingTime();
    calculateAvgTurnAroundTime();

    return isSimulationComplete();
}

shared_ptr<Process> Priority_Scheduler::selectNextProcess() 
{
    if(readyQueue.empty()) return nullptr;
    return readyQueue.top();
}
