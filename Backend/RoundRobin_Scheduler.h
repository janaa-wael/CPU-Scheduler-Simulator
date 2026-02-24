#include <iostream>
#include <queue>
#include <functional>
#include "Scheduler.h"
#include "process.h"

using namespace std;


class RoundRobin_Scheduler : Scheduler {
private:
    int quantumCounter;
    int timeQuantum;
    queue<shared_ptr<Process>> roundRobinQueue;
    struct RR_Comparator{
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
    RoundRobin_Scheduler(bool isPreemptive);
    void runStatic(int runUntilTime = -1) override;
    bool runOneStep() override;
    void updateReadyQueue() override;

    void addProcess(shared_ptr<Process> p) override;
    void deleteProcess(int pid) override;
    void updateProcess(shared_ptr<Process>p) override;

    int getTimeQuantum() const;
    void setTimeQuantum(int quantum);
    shared_ptr<Process> selectNextProcess() override;
};