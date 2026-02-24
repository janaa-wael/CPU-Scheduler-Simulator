#include <iostream>
#include "Process.h"
#include <algorithm>
#include "RoundRobin_Scheduler.h"
using namespace std;

bool RoundRobin_Scheduler::allProcessesComplete() const
{

}

int RoundRobin_Scheduler::findNextArrivalTime() const
{
    int next_arrival = INT_MAX;
    for(auto&p : allProcesses)
    {
        next_arrival = min(next_arrival , p->getArrivalTime()); 
    }
}

RoundRobin_Scheduler::RoundRobin_Scheduler(bool isPreemptive)
: Scheduler(RR_Comparator(), true, "Round Robin Scheduler")
{

}

void RoundRobin_Scheduler::runStatic(int runUntilTime = -1) 
{
        timeCounter = 0;

    // Clear the queue and reset the current process
    std::queue<std::shared_ptr<Process>> emptyQueue;
    roundRobinQueue = emptyQueue;
    currentProcess = nullptr;
    quantumCounter = 0;

    // Reset process states
    for (auto& p : allProcesses) {
        p->setIsComplete(false);
        p->setRemainingTime(p->getBurstTime());
        p->setStartTime(-1);
        p->setCompletionTime(-1);
        p->setTurnAroundTime(0);
        p->setWaitingTime(0);
    }

    // Sort processes by arrival time for consistent processing
    std::sort(allProcesses.begin(), allProcesses.end(),
              [](const auto& a, const auto& b) { return a->getArrivalTime() < b->getArrivalTime(); });

    // Add processes that arrive at time 0
    for (auto& p : allProcesses) {
        if (p->getArrivalTime() == 0) {
            roundRobinQueue.push(p);
        }
    }

    bool isLiveMode = (runUntilTime < 0);
    std::vector<std::shared_ptr<Process>> completedProcesses;

    while (!allProcessesComplete()) {
        // Handle CPU idle time when no process is ready
        if (roundRobinQueue.empty() && !currentProcess) {
            // Find the next arrival time
            int nextArrival = findNextArrivalTime();
            if (nextArrival == INT_MAX) break; // No more processes will arrive

            // Jump to next arrival
            timeCounter = nextArrival;

            // Add newly arrived processes to the queue
            for (auto& p : allProcesses) {
                if (!p->getIsComplete() && p->getArrivalTime() <= timeCounter) 
                {
                    bool alreadyInQueue = false;
                    // Check if the process is already in the queue
                    std::queue<std::shared_ptr<Process>> tempQueue = roundRobinQueue;
                    while (!tempQueue.empty()) 
                    {
                        if (tempQueue.front()->getId() == p->getId()) 
                        {
                            alreadyInQueue = true;
                            break;
                        }
                        tempQueue.pop();
                    }

                    if (!alreadyInQueue) {
                        roundRobinQueue.push(p);
                    }
                }
            }
            continue;
        }

        if (!currentProcess) {
            currentProcess = roundRobinQueue.front();
            roundRobinQueue.pop();

            if (currentProcess->getStartTime() < 0) {
                currentProcess->setStartTime(timeCounter);
            }

            quantumCounter = 0;
        }

        int executeTime = min(quantumCounter, currentProcess->getRemainingTime());

        int endTime = timeCounter + executeTime;
        std::vector<std::shared_ptr<Process>> newArrivals;

        for (auto& p : allProcesses) {
            if (!p->getIsComplete() && p->getArrivalTime() > timeCounter && p->getArrivalTime() <= endTime) {
                newArrivals.push_back(p);
            }
        }

        std::sort(newArrivals.begin(), newArrivals.end(),
                  [](const auto& a, const auto& b) { return a->getArrivalTime() < b->getArrivalTime(); });

        currentProcess->setRemainingTime(currentProcess->getRemainingTime() - executeTime);
        timeCounter += executeTime;

        for (auto& p : newArrivals) {
            roundRobinQueue.push(p);
        }

        if (currentProcess->getRemainingTime() <= 0) {
            currentProcess->setCompletionTime(timeCounter);
            currentProcess->setIsComplete(true);
            currentProcess->setTurnAroundTime(currentProcess->getCompletionTime() - currentProcess->getArrivalTime());
            currentProcess->setWaitingTime(currentProcess->getTurnAroundTime() - currentProcess->getBurstTime());
            completedProcesses.push_back(currentProcess);
            currentProcess = nullptr;
        } else {
            roundRobinQueue.push(currentProcess);
            currentProcess = nullptr;
        }

        if (!isLiveMode && timeCounter >= runUntilTime) {
            break;
        }
    }

    calculateAvgWaitingTime();
    calculateAvgTurnAroundTime();

}
    
bool RoundRobin_Scheduler::runOneStep() 
{
    // First, clear any completed process from being current
    if (currentProcess && currentProcess->getRemainingTime() <= 0) {
        currentProcess = nullptr;
    }

    // Add arriving processes to the queue
    for (auto& p : allProcesses) {
        if (p->getArrivalTime() == timeCounter && !p->getIsComplete()) {
            bool inQueue = false;
            std::queue<std::shared_ptr<Process>> temp = roundRobinQueue;
            while (!temp.empty()) {
                if (temp.front()->getId() == p->getId()) {
                    inQueue = true;
                    break;
                }
                temp.pop();
            }
            if (!inQueue) {
                roundRobinQueue.push(p);
            }
        }
    }

    // Check process state changes
    bool needNewProcess = false;

    if (currentProcess) {
        if (currentProcess->getRemainingTime() <= 0) {
            // Process is completed - we should have cleared it above
            needNewProcess = true;
        } else if (quantumCounter >= timeQuantum) {
            // Process used up its quantum
            roundRobinQueue.push(currentProcess);
            currentProcess = nullptr; // Clear the current process
            needNewProcess = true;
        }
    } else {
        // No current process, need a new one if available
        needNewProcess = true;
    }

    // If we need a new process, get one from the queue
    if (needNewProcess) {
        if (!roundRobinQueue.empty()) {
            currentProcess = roundRobinQueue.front();
            roundRobinQueue.pop();

            if (currentProcess->getStartTime() < 0) {
                currentProcess->setStartTime(timeCounter);
            }
            quantumCounter = 0;
        } else {
            // No process available, check if simulation is complete
            if (allProcessesComplete())
                return true;

            // Advance time by just 1 unit if no processes are ready
            timeCounter++;

            // Check for any processes arriving at the new time
            for (auto& p : allProcesses) {
                if (p->getArrivalTime() == timeCounter && !p->getIsComplete()) {
                    roundRobinQueue.push(p);
                }
            }

            // Calculate metrics
            calculateAvgWaitingTime();
            calculateAvgTurnAroundTime();

            // Return but don't mark as complete - this is an idle step
            return false;
        }
    }
}
shared_ptr<Process> RoundRobin_Scheduler::selectNextProcess() 
{
    if(roundRobinQueue.empty()) return nullptr;
    auto& p = roundRobinQueue.front();
    roundRobinQueue.pop();
    return p;
}

void RoundRobin_Scheduler::addProcess(shared_ptr<Process> p) 
{
    allProcesses.push_back(p);
    if(p->getArrivalTime() <= timeCounter) roundRobinQueue.push(p);
}

void RoundRobin_Scheduler::deleteProcess(int pid) 
{
    allProcesses.erase(
        remove_if(
            allProcesses.begin(),
            allProcesses.end(),
            [pid](auto& p)
            {
                return pid == p->getId();
            }
        )
    );
}

void RoundRobin_Scheduler::updateProcess(shared_ptr<Process>p) 
{
    for(auto& proc : allProcesses)
    {
        if(proc->getId() == p->getId())
        {
            proc = move(p);
            return;
        }
    }
}


int RoundRobin_Scheduler::getTimeQuantum() const
{
    return this->timeQuantum;
}

void RoundRobin_Scheduler::setTimeQuantum(int quantum)
{
    this->timeQuantum = quantum;
}

