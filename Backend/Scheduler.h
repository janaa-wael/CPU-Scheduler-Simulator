#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include "Process.h"

using namespace std;

class Scheduler{
protected:
    float avgTurnAroundTime;
    float avgWaitingTime;
    bool preemptive = false;
    string name;
    shared_ptr<Process> currentProcess = nullptr;
    function<bool(std::shared_ptr<Process>, std::shared_ptr<Process>)> processComparator;
    vector<shared_ptr<Process>> allProcesses;

    int timeCounter=0;
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
    int getCurrentTime() const;
    bool getPreemptive() const;
    const string& getName() const;
    shared_ptr<Process> getCurrentProcess() const;
    virtual void addProcess(shared_ptr<Process> p);
    virtual void deleteProcess(int pid);
    virtual void updateProcess(shared_ptr<Process> p);

    virtual void updateReadyQueue();
    virtual void runStatic(int runUntilTime = -1) = 0;
    virtual shared_ptr<Process> selectNextProcess() = 0;

    virtual bool runOneStep();

    virtual void addNewProcesses(const vector<shared_ptr<Process>>& newProcesses);
    virtual bool isSimulationComplete() const;

    void calculateAvgWaitingTime();
    void calculateAvgTurnAroundTime();


};