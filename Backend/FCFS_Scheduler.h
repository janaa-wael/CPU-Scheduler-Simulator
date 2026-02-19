#include <iostream>
#include "Scheduler.h"
#include "Process.h"

using namespace std;

class FCFS_Scheduler : Scheduler{
private:
    struct FirstArrivalComparator{
        bool operator()(shared_ptr<Process> p1, shared_ptr<Process> p2)
        {
            if(p1->getArrivalTime() == p2->getArrivalTime())
            {
                return p1->getId() > p2->getId();
            }
            return p1->getArrivalTime() > p2->getArrivalTime();
        }
    };
    bool allProcessesComplete() const;
    int findNextArrivalTime() const;
public:
    FCFS_Scheduler(bool isPreemptive);
    void runStatic(int runUntilTime = -1) override;
    bool runOneStep() override;
    shared_ptr<Process> selectNextProcess() override;

};