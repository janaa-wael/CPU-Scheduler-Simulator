#include <iostream>
#include "Process.h"

string Process::getName() const
{
    return name;
}

int Process::getId() const
{
    return id;
}

int Process::getBurstTime() const
{
    return burstTime;
}

int Process::getPriority() const
{
    return priority;
}

float Process::getProgress() const
{
    if(burstTime == 0) return 0.0f;
    if(remainingTime == 0) return 100.0f;

    return (static_cast<float>(burstTime-remainingTime)/burstTime) * 100.0f;
}


int Process::getArrivalTime() const
{
    return arrivalTime;
}

int Process::getCompletionTime() const
{
    return completionTime;
}

int Process::getStartTime() const
{
    return startTime;
}

int Process::getRemainingTime() const
{
    return remainingTime;
}

int Process::getWaitingTime() const
{
    return waitingTime;
}

int Process::getTurnAroundTime() const
{
    return turnAroundTime;
}

bool Process::getIsComplete() const
{
    return isComplete;
}


void Process::setName(string name) 
{
    this->name = name;
}

void Process::setId(int id) 
{
    this->id = id;
}

void Process::setBurstTime(int burstTime) 
{
    this->burstTime = burstTime;
}

void Process::setPriority(int priority) 
{
    this->priority = priority;
}

void Process::setArrivalTime(int arrivalTime) 
{
    this->arrivalTime = arrivalTime;
}

void Process::setCompletionTime(int completionTime) 
{
    this->completionTime = completionTime;
}

void Process::setStartTime(int startTime) 
{
    this->startTime = startTime;
}

void Process::setRemainingTime(int remainingTime) 
{
    this->remainingTime = remainingTime;
}

void Process::setTurnAroundTime(int turnaroundTime)
{
    this->turnAroundTime = turnaroundTime;
}

void Process::setIsComplete(bool status)
{
    this->isComplete = true;
}

void Process::setWaitingTime(int waitingTime)
{
    this->waitingTime = waitingTime;
}
