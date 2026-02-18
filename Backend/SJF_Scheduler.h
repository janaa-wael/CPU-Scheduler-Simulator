#include <iostream>
#include "Scheduler.h"
#include "Process.h"
#include <functional>


class SJF_Scheduler : Scheduler{
private:
    struct ShortestJobFirstComparator{
        bool operator()(shared_ptr<Process> p1, shared_ptr<Process> p2)
        {
            if(p1->getBurstTime() == p1->getBurstTime())
            {
                return p1->getId() > p2->getId();
            }
            return p1->getBurstTime() > p2->getBurstTime();
        }
    };
public:
    SJF_Scheduler(bool preemptive);
    void run(int runUntilTime = -1);
    shared_ptr<Process> selectNextProcess();

};
