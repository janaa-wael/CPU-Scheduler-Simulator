#include <iostream>
#include <functional>
#include <memory>
#include "Scheduler.h"
#include "Process.h"

using namespace std;


Scheduler::Scheduler(std::function<bool(std::shared_ptr<Process>, std::shared_ptr<Process>)> comp, bool isPreemptive, const std::string& schedulerName)
: processComparator(comp), preemptive(isPreemptive), name(schedulerName)
{

}


float Scheduler::getAvgTurnAroundTime() const
{
    return avgTurnAroundTime;
}

float Scheduler::getAvgWaitingTime() const
{
    return avgWaitingTime;
}

bool Scheduler::getPreemptive() const
{
    return preemptive;
}

const string& Scheduler::getName() const
{
    return name;
}

void Scheduler::calculateAvgWaitingTime()
{
    
}

void Scheduler::calculateAvgTurnAroundTime()
{
    float result = 0;
    int sz = allProcesses.size();
    for(auto p : allProcesses)
    {
        result += (p->getCompletionTime() - p->getStartTime());
    }
    avgTurnAroundTime = result/sz;
}

void Scheduler::addProcess(shared_ptr<Process> p)
{
    allProcesses.push_back(p);
    readyQueue.push(p);
}

void Scheduler::deleteProcess(int pid)
{
    allProcesses.erase(
        remove_if(allProcesses.begin(), allProcesses.end(), 
            [pid](auto& p){
                return p->getId() == pid;
            }),
        allProcesses.end()
    );
    updateReadyQueue();
}

void Scheduler::updateProcess(shared_ptr<Process> p)
{
    for(auto& process : allProcesses)
    {
        if(p->getId() == process->getId())
        {
            process = p;
            break;
        }
    }
    updateReadyQueue();
}

shared_ptr<Process> Scheduler::getCurrentProcess() const
{
    return currentProcess;
}

void Scheduler::addNewProcesses(const vector<shared_ptr<Process>>& newProcesses)
{
    for(const auto& p : newProcesses)
    {
        addProcess(p);
    }
}
