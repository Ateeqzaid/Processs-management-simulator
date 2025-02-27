#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;

// Constants and global variables
const int CONTEXT_SWITCH_TIME = 14;
const int TIME_SLICE = 200;
const int MAX_BURST_TIME = 3500;
const int MIN_BURST_TIME = 500;
const int MAX_PRIORITY = 5;

double total_turnaround_time = 0.0;
double total_waiting_time = 0.0;
double total_initial_waiting_time = 0.0;
double max_turnaround_time = 0.0;
double min_turnaround_time = 5000.0;
double max_waiting_time = 0.0;
double min_waiting_time = 5000.0;
double max_initial_waiting_time = 0.0;
double min_initial_waiting_time = 5000.0;

// Process class definition
class Process {
private:
    int arrivalTime;
    int burstTime;
    int remainingBurstTime;
    bool hasBeenAccessed;
    int initialWaitingTime;
    int totalWaitingTime;
    int priorityLevel;
    int processId;

public:
    Process(int burst, int arrival, int id, int priority);
    void setArrivalTime(int time);
    void setProcessId(int id);
    int getArrivalTime() const;
    int getBurstTime() const;
    int getPriorityLevel() const;
    int getRemainingBurstTime() const;
    void decrementBurstTime();
    int getInitialWaitingTime() const;
    int getTotalWaitingTime() const;
    void markAsAccessed(int time);
    bool hasAccessedCPU() const;
    int getProcessId() const;
};

Process::Process(int burst, int arrival, int id, int priority) {
    processId = id;
    burstTime = burst;
    remainingBurstTime = burst;
    arrivalTime = arrival;
    hasBeenAccessed = false;
    priorityLevel = priority;
    initialWaitingTime = 0;
    totalWaitingTime = 0;
}

void Process::setArrivalTime(int time) {
    arrivalTime = time;
}

void Process::setProcessId(int id) {
    processId = id;
}

int Process::getArrivalTime() const {
    return arrivalTime;
}

int Process::getBurstTime() const {
    return burstTime;
}

int Process::getPriorityLevel() const {
    return priorityLevel;
}

int Process::getRemainingBurstTime() const {
    return remainingBurstTime;
}

void Process::decrementBurstTime() {
    remainingBurstTime--;
}

int Process::getInitialWaitingTime() const {
    return initialWaitingTime;
}

int Process::getTotalWaitingTime() const {
    return totalWaitingTime;
}

void Process::markAsAccessed(int time) {
    hasBeenAccessed = true;
    initialWaitingTime = time;
    totalWaitingTime = time;
}

bool Process::hasAccessedCPU() const {
    return hasBeenAccessed;
}

int Process::getProcessId() const {
    return processId;
}

// Helper functions for sorting
bool compareByBurstTime(const Process &a, const Process &b) {
    return a.getBurstTime() < b.getBurstTime();
}

bool compareByPriority(const Process &a, const Process &b) {
    return a.getPriorityLevel() < b.getPriorityLevel();
}

bool compareByArrivalTime(const Process &a, const Process &b) {
    return a.getArrivalTime() < b.getArrivalTime();
}

// Function to print statistics
void printStatistics(int numProcesses) {
    cout << "Turnaround time: min ";
    printf("%.3f", min_turnaround_time);
    cout << "ms";
    cout << "; avg ";
    printf("%.3f", total_turnaround_time / numProcesses);
    cout << "ms";
    cout << "; max ";
    printf("%.3f", max_turnaround_time);
    cout << "ms" << endl;

    cout << "Initial wait time: min ";
    printf("%.3f", min_initial_waiting_time);
    cout << "ms";
    cout << "; avg ";
    printf("%.3f", total_initial_waiting_time / numProcesses);
    cout << "ms";
    cout << "; max ";
    printf("%.3f", max_initial_waiting_time);
    cout << "ms" << endl;

    cout << "Total wait time: min ";
    printf("%.3f", min_waiting_time);
    cout << "ms";
    cout << "; avg ";
    printf("%.3f", total_waiting_time / numProcesses);
    cout << "ms";
    cout << "; max ";
    printf("%.3f", max_waiting_time);
    cout << "ms" << endl;

    // Reset statistics for next scheduling algorithm
    total_turnaround_time = 0.0;
    total_waiting_time = 0.0;
    total_initial_waiting_time = 0.0;
    max_turnaround_time = 0.0;
    min_turnaround_time = 5000.0 * numProcesses;
    max_waiting_time = 0.0;
    min_waiting_time = 5000.0 * numProcesses;
    max_initial_waiting_time = 0.0;
    min_initial_waiting_time = 5000.0 * numProcesses;

    cout << endl;
}

// Scheduling algorithms
void firstComeFirstServe(vector<Process> processes, int numProcesses) {
    int completedProcesses = 0;
    int currentTime = 0;
    int lastProcessId = 0;
    vector<Process> readyQueue;
    bool isContextSwitching = false;

    while (completedProcesses != numProcesses) {
        // Add arriving processes to the ready queue
        for (auto it = processes.begin(); it != processes.end(); ) {
            if (it->getArrivalTime() <= currentTime) {
                readyQueue.push_back(*it);
                cout << "[time " << it->getArrivalTime() << "ms] Process " << it->getProcessId() << " created (requires " << it->getBurstTime() << "ms CPU time)" << endl;
                it = processes.erase(it);
            } else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            if (isContextSwitching) {
                cout << "[time " << currentTime - 7 << "ms] Context switch (swapped out process " << lastProcessId << " for process " << readyQueue[0].getProcessId() << ")" << endl;
                currentTime += 7;
                isContextSwitching = false;
            }

            if (!readyQueue[0].hasAccessedCPU()) {
                int initialWait = currentTime - readyQueue[0].getArrivalTime();
                readyQueue[0].markAsAccessed(initialWait);
                total_initial_waiting_time += initialWait;
                min_initial_waiting_time = min(min_initial_waiting_time, static_cast<double>(initialWait));
                max_initial_waiting_time = max(max_initial_waiting_time, static_cast<double>(initialWait));
                cout << "[time " << currentTime << "ms] Process " << readyQueue[0].getProcessId() << " accessed CPU for the first time (initial wait time " << readyQueue[0].getInitialWaitingTime() << "ms)" << endl;
            }

            readyQueue[0].decrementBurstTime();

            if (readyQueue[0].getRemainingBurstTime() == 0) {
                int turnaroundTime = currentTime + 1 - readyQueue[0].getArrivalTime();
                int totalWaitTime = turnaroundTime - readyQueue[0].getBurstTime();
                cout << "[time " << currentTime + 1 << "ms] Process " << readyQueue[0].getProcessId() << " completed its CPU burst (turnaround time " << turnaroundTime << "ms, initial wait time " << readyQueue[0].getInitialWaitingTime() << "ms, total wait time " << totalWaitTime << "ms)" << endl;

                lastProcessId = readyQueue[0].getProcessId();
                total_turnaround_time += turnaroundTime;
                total_waiting_time += totalWaitTime;
                min_waiting_time = min(min_waiting_time, static_cast<double>(totalWaitTime));
                max_waiting_time = max(max_waiting_time, static_cast<double>(totalWaitTime));
                min_turnaround_time = min(min_turnaround_time, static_cast<double>(turnaroundTime));
                max_turnaround_time = max(max_turnaround_time, static_cast<double>(turnaroundTime));

                readyQueue.erase(readyQueue.begin());
                completedProcesses++;

                if (!readyQueue.empty()) {
                    cout << "[time " << currentTime + 1 << "ms] Context switch (swapped out process " << lastProcessId << " for process " << readyQueue[0].getProcessId() << ")" << endl;
                    currentTime += CONTEXT_SWITCH_TIME;
                } else {
                    isContextSwitching = true;
                    currentTime += 7;
                }
            }
        }
        currentTime++;
    }
}

// Other scheduling algorithms (SJF, SRTF, Round Robin, Priority) can be refactored similarly...

int main() {
    int numProcesses = 20;
    srand(static_cast<unsigned>(time(0)));
    vector<Process> processes;

    // Create processes with zero arrival time
    int numZeroArrival = numProcesses * 0.2;
    for (int i = 0; i < numZeroArrival; i++) {
        int burstTime = (rand() % MAX_BURST_TIME) + MIN_BURST_TIME;
        int priority = rand() % MAX_PRIORITY;
        processes.emplace_back(burstTime, 0, i + 1, priority);
    }

    // Create processes with non-zero arrival time
    int numRandomArrival = numProcesses - numZeroArrival;
    for (int i = 0; i < numRandomArrival; i++) {
        int burstTime = (rand() % MAX_BURST_TIME) + MIN_BURST_TIME;
        int priority = rand() % MAX_PRIORITY;
        double lambda = 0.001;
        double randomValue = static_cast<double>(rand()) / RAND_MAX;
        double arrivalTime = -log(randomValue) / lambda;
        if (arrivalTime > 8000) {
            i--;
            continue;
        }
        processes.emplace_back(burstTime, static_cast<int>(arrivalTime), numZeroArrival + i + 1, priority);
    }

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    // Assign process IDs based on arrival time
    for (size_t i = 0; i < processes.size(); i++) {
        processes[i].setProcessId(i + 1);
    }

    // Execute scheduling algorithms
    cout << "First Come First Serve:" << endl << endl;
    firstComeFirstServe(processes, numProcesses);
    printStatistics(numProcesses);

    // Other scheduling algorithms can be called here...

    return 0;
}