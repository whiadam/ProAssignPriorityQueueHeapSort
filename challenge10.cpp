#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "Scheduler.h"

int main() {
    srand(42);

    const int N = 10;
    SchedTask tasks[N];
    SchedTask tasksCopy[N];

    for (int i = 0; i < N; ++i) {
        tasks[i].id           = i + 1;
        tasks[i].priority     = rand() % 10 + 1;
        tasks[i].arrivalTime  = i * 2;
        tasks[i].burstTime    = rand() % 5 + 1;
        tasks[i].deadline     = tasks[i].arrivalTime + tasks[i].burstTime + rand() % 6 + 1;
        tasks[i].remainingTime = tasks[i].burstTime;
        tasksCopy[i] = tasks[i];
    }

    std::cout << "=== Challenge 10: HeapForge Champion ===\n";
    std::cout << "Tasks:\n";
    for (int i = 0; i < N; ++i) {
        tasks[i].print();
        std::cout << "\n";
    }

    GanttEntry ganttEDF[5000];
    int ganttEDFSize = 0;
    Stats edfStats = runEDF(tasks, N, ganttEDF, ganttEDFSize);
    printGantt(ganttEDF, ganttEDFSize);
    printStats(edfStats, "EDF");

    GanttEntry ganttFP[5000];
    int ganttFPSize = 0;
    Stats fpStats = runFixedPriority(tasksCopy, N, ganttFP, ganttFPSize);
    printGantt(ganttFP, ganttFPSize);
    printStats(fpStats, "Fixed Priority");

    std::cout << "\n--- Comparison ---\n";
    std::cout << std::setw(20) << "" << std::setw(10) << "EDF" << std::setw(15) << "Fixed Pri" << "\n";
    std::cout << std::setw(20) << "Missed deadlines:"
              << std::setw(10) << edfStats.missedDeadlines
              << std::setw(15) << fpStats.missedDeadlines << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(20) << "Avg wait time:"
              << std::setw(10) << edfStats.avgWaitTime
              << std::setw(15) << fpStats.avgWaitTime << "\n";
    std::cout << std::setw(20) << "Avg turnaround:"
              << std::setw(10) << edfStats.avgTurnaround
              << std::setw(15) << fpStats.avgTurnaround << "\n";

    return 0;
}
