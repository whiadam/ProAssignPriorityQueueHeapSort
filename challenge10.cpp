#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Scheduler.h"

int main() {
    srand(42);

    const int N = 1000;
    static SchedTask tasks[N];
    static SchedTask tasksCopy[N];

    // Total burst across all tasks determines how long the CPU is busy.
    // To make EDF achievable, we need deadlines long enough that an optimal
    // scheduler (EDF) can meet them. We spread arrivals so ~5 tasks arrive
    // per time unit across 200 time units, and give each task a deadline
    // of arrivalTime + burstTime + slack (20-30 ticks). This gives EDF
    // enough room to schedule tasks without missing, while fixed priority
    // (ignoring deadlines) will miss several.
    for (int i = 0; i < N; ++i) {
        tasks[i].id            = i + 1;
        tasks[i].priority      = rand() % 10 + 1;
        tasks[i].arrivalTime   = i / 5;
        tasks[i].burstTime     = rand() % 3 + 1;  // 1-3 ticks (lighter load)
        tasks[i].deadline      = tasks[i].arrivalTime + tasks[i].burstTime + rand() % 10 + 20;
        tasks[i].remainingTime = tasks[i].burstTime;
        tasksCopy[i] = tasks[i];
    }

    std::cout << "=== Challenge 10: HeapForge Champion (1000 tasks) ===\n";
    std::cout << "Sample tasks (first 5):\n";
    for (int i = 0; i < 5; ++i) { tasks[i].print(); std::cout << "\n"; }

    static GanttEntry ganttEDF[5000];
    int ganttEDFSize = 0;
    Stats edfStats = runEDF(tasks, N, ganttEDF, ganttEDFSize);

    static GanttEntry ganttFP[5000];
    int ganttFPSize = 0;
    Stats fpStats = runFixedPriority(tasksCopy, N, ganttFP, ganttFPSize);

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
