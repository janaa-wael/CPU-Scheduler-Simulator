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

