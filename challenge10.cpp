#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "Scheduler.h"

int main() {
    srand(42);

    const int N = 1000;
    static SchedTask tasks[N];
    static SchedTask tasksCopy[N];

    for (int i = 0; i < N; ++i) {
        tasks[i].id            = i + 1;
        tasks[i].priority      = rand() % 10 + 1;
        tasks[i].arrivalTime   = i / 5;  // ~5 tasks arrive per time unit
        tasks[i].burstTime     = rand() % 5 + 1;
        tasks[i].deadline      = tasks[i].arrivalTime + tasks[i].burstTime + rand() % 4 + 1;
        tasks[i].remainingTime = tasks[i].burstTime;
        tasksCopy[i] = tasks[i];
    }

    std::cout << "=== Challenge 10: HeapForge Champion (1000 tasks) ===\n";

    static GanttEntry ganttEDF[10000];
    int ganttEDFSize = 0;
    Stats edfStats = runEDF(tasks, N, ganttEDF, ganttEDFSize);

    static GanttEntry ganttFP[10000];
    int ganttFPSize = 0;
    Stats fpStats = runFixedPriority(tasksCopy, N, ganttFP, ganttFPSize);

    // Print first 30 Gantt entries for each (full 1000-task chart is too long)
    std::cout << "\nEDF Gantt (first 30 slices):\n";
    printGantt(ganttEDF, ganttEDFSize < 30 ? ganttEDFSize : 30);

    std::cout << "\nFixed-Priority Gantt (first 30 slices):\n";
    printGantt(ganttFP, ganttFPSize < 30 ? ganttFPSize : 30);

    printStats(edfStats, "EDF");
    printStats(fpStats, "Fixed Priority");

    std::cout << "\n--- Comparison ---\n";
    std::cout << std::setw(20) << "" << std::setw(12) << "EDF" << std::setw(15) << "Fixed Pri" << "\n";
    std::cout << std::setw(20) << "Missed deadlines:"
              << std::setw(12) << edfStats.missedDeadlines
              << std::setw(15) << fpStats.missedDeadlines << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(20) << "Avg wait time:"
              << std::setw(12) << edfStats.avgWaitTime
              << std::setw(15) << fpStats.avgWaitTime << "\n";
    std::cout << std::setw(20) << "Avg turnaround:"
              << std::setw(12) << edfStats.avgTurnaround
              << std::setw(15) << fpStats.avgTurnaround << "\n";

    return 0;
}
