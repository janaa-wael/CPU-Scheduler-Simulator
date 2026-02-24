#include <iostream>
#include "Process.h"
#include "Scheduler.h"

class Priority_Scheduler : Scheduler {
private:
    bool allProcessesComplete() const;
    int findNextArrivalTime() const;

public:
    Priority_Scheduler(bool isPreemptive = false);
    void runStatic(int runUntilTime = -1) override;
    bool runOneStep() override;
    shared_ptr<Process> selectNextProcess() override;
};
