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
    bool allProcessesComplete() const;
    int findNextArrivalTime() const;

public:
    SJF_Scheduler(bool preemptive);
    void runStatic(int runUntilTime = -1) override;
    bool runOneStep() override;
    shared_ptr<Process> selectNextProcess();

};
