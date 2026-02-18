#include <iostream>
#include "Process.h"
#include "Scheduler.h"
#include "FCFS_Scheduler.h"

using namespace std;

int ticks=0;

FCFS_Scheduler::FCFS_Scheduler(bool isPreemptive) 
: Scheduler(FirstArrivalComparator(), isPreemptive, "First-Come-First-Served Scheduler")
{

}

void FCFS_Scheduler::run(int runUntilTime = -1)
{
    while(readyQueue.empty()) ticks++;

    auto currentProcess = readyQueue.top();
    readyQueue.pop();
    currentProcess->setArrivalTime(ticks);

    while(currentProcess->getRemainingTime() != 0)
    {
        currentProcess->setRemainingTime(currentProcess->getRemainingTime() - 1);
        ticks++;
    }

    currentProcess->setCompletionTime(ticks);
    currentProcess->setTurnAroundTime(currentProcess->getCompletionTime() - currentProcess->getArrivalTime());
    currentProcess->setIsComplete(true);
    currentProcess->setWaitingTime(currentProcess->getTurnAroundTime() - currentProcess->getBurstTime());


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