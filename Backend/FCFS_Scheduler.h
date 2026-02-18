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

public:
    FCFS_Scheduler(bool isPreemptive);
    void run(int runUntilTime = -1);
    shared_ptr<Process> selectNextProcess();

};