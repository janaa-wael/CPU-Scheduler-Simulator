#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include "Process.h"

using namespace std;

class Scheduler{
private:
    float avgTurnAroundTime;
    float avgWaitingTime;
    bool preemptive = false;
    string name;
    function<bool(std::shared_ptr<Process>, std::shared_ptr<Process>)> processComparator;
    vector<shared_ptr<Process>> allProcesses;
protected:
    priority_queue<
        shared_ptr<Process>, 
        vector<shared_ptr<Process>>, 
        function<bool(shared_ptr<Process>, shared_ptr<Process>)>
    > readyQueue;


public: 
    Scheduler(std::function<bool(std::shared_ptr<Process>, std::shared_ptr<Process>)> comp, 
        bool isPreemptive, const std::string& schedulerName);
    virtual ~Scheduler() = default;
    float getAvgTurnAroundTime() const;
    float getAvgWaitingTime() const;
    bool getPreemptive() const;
    const string& getName() const;

    virtual void addProcess(shared_ptr<Process> p);
    virtual void deleteProcess(int pid);
    virtual void updateProcess(shared_ptr<Process> p);

    virtual void updateReadyQueue();
    virtual void run(int runUntilTime = -1) = 0;
    virtual shared_ptr<Process> selectNextProcess() = 0;

    void calculateAvgWaitingTime();
    void calculateAvgTurnAroundTime();


};