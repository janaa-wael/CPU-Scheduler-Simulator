#include <iostream>

using namespace std;


class Process{
private:
    string name;
    int id;
    int burstTime;
    int priority;
    int arrivalTime;
    int completionTime;
    int startTime;
    int remainingTime;

public:
    string getName() const;
    int getId() const;
    int getBurstTime() const;
    int getPriority() const;
    int getArrivalTime() const;
    int getCompletionTime() const;
    int getStartTime() const;
    int getRemainingTime() const;
    void setName(string name);
    void setId(int id);
    void setBurstTime(int burstTime);
    void setPriority(int priority);
    void setArrivalTime(int arrivalTime);
    void setCompletionTime(int completionTime);
    void setStartTime(int startTime);
    void setRemainingTime(int remainingTime);
};