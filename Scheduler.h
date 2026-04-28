#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <iomanip>
#include <string>
#include "HeapMaster.h"

struct SchedTask {
    int id;
    int priority;
    int arrivalTime;
    int burstTime;
    int deadline;
    int remainingTime;

    void print() const {
        std::cout << "Task#" << std::setw(3) << id
                  << " pri=" << std::setw(3) << priority
                  << " arrive=" << std::setw(3) << arrivalTime
                  << " burst=" << std::setw(3) << burstTime
                  << " dead=" << std::setw(3) << deadline;
    }
};

struct EDFCompare {
    bool operator()(const SchedTask& a, const SchedTask& b) const {
        return a.deadline > b.deadline;
    }
};

struct FixedPriCompare {
    bool operator()(const SchedTask& a, const SchedTask& b) const {
        return a.priority < b.priority;
    }
};

struct GanttEntry {
    int taskId;
    int start;
    int end;
};

struct Stats {
    int missedDeadlines;
    double avgWaitTime;
    double avgTurnaround;
    int totalTasks;
};

void printGantt(GanttEntry gantt[], int n) {
    std::cout << "\nGantt Chart:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "|T" << std::setw(2) << gantt[i].taskId
                  << "[" << gantt[i].start << "-" << gantt[i].end << "]";
    }
    std::cout << "|\n";
}

void printStats(const Stats& s, const std::string& name) {
    std::cout << "\n--- " << name << " Statistics ---\n";
    std::cout << "Total tasks:      " << s.totalTasks << "\n";
    std::cout << "Missed deadlines: " << s.missedDeadlines << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Avg wait time:    " << s.avgWaitTime << "\n";
    std::cout << "Avg turnaround:   " << s.avgTurnaround << "\n";
}

Stats runEDF(SchedTask tasks[], int n, GanttEntry gantt[], int& ganttSize) {
    HeapMaster<SchedTask, 2000, EDFCompare> pq;
    int time = 0;
    int idx = 0;
    int waitSum = 0;
    int turnaroundSum = 0;
    int missed = 0;
    ganttSize = 0;

    int waitStart[2000];
    for (int i = 0; i < n; ++i) {
        tasks[i].remainingTime = tasks[i].burstTime;
        waitStart[i] = tasks[i].arrivalTime;
    }

    while (idx < n || !pq.empty()) {
        while (idx < n && tasks[idx].arrivalTime <= time) {
            pq.push(tasks[idx]);
            idx++;
        }
        if (pq.empty()) {
            time++;
            continue;
        }
        SchedTask cur = pq.top(); pq.pop();
        int startTime = time;
        cur.remainingTime--;
        time++;

        while (idx < n && tasks[idx].arrivalTime <= time) {
            pq.push(tasks[idx]);
            idx++;
        }

        if (cur.remainingTime > 0) {
            if (!pq.empty()) {
                SchedTask next = pq.top();
                if (next.deadline < cur.deadline) {
                    pq.push(cur);
                } else {
                    pq.push(cur);
                }
            } else {
                pq.push(cur);
            }
        } else {
            int turnaround = time - cur.arrivalTime;
            int wait = turnaround - cur.burstTime;
            waitSum += wait;
            turnaroundSum += turnaround;
            if (time > cur.deadline) missed++;
        }

        gantt[ganttSize++] = {cur.id, startTime, time};
    }

    Stats s;
    s.totalTasks = n;
    s.missedDeadlines = missed;
    s.avgWaitTime = n > 0 ? (double)waitSum / n : 0;
    s.avgTurnaround = n > 0 ? (double)turnaroundSum / n : 0;
    return s;
}

Stats runFixedPriority(SchedTask tasks[], int n, GanttEntry gantt[], int& ganttSize) {
    HeapMaster<SchedTask, 2000, FixedPriCompare> pq;
    int time = 0;
    int idx = 0;
    int waitSum = 0;
    int turnaroundSum = 0;
    int missed = 0;
    ganttSize = 0;

    for (int i = 0; i < n; ++i) {
        tasks[i].remainingTime = tasks[i].burstTime;
    }

    while (idx < n || !pq.empty()) {
        while (idx < n && tasks[idx].arrivalTime <= time) {
            pq.push(tasks[idx]);
            idx++;
        }
        if (pq.empty()) {
            time++;
            continue;
        }
        SchedTask cur = pq.top(); pq.pop();
        int startTime = time;
        cur.remainingTime--;
        time++;

        while (idx < n && tasks[idx].arrivalTime <= time) {
            pq.push(tasks[idx]);
            idx++;
        }

        if (cur.remainingTime > 0) {
            pq.push(cur);
        } else {
            int turnaround = time - cur.arrivalTime;
            int wait = turnaround - cur.burstTime;
            waitSum += wait;
            turnaroundSum += turnaround;
            if (time > cur.deadline) missed++;
        }

        gantt[ganttSize++] = {cur.id, startTime, time};
    }

    Stats s;
    s.totalTasks = n;
    s.missedDeadlines = missed;
    s.avgWaitTime = n > 0 ? (double)waitSum / n : 0;
    s.avgTurnaround = n > 0 ? (double)turnaroundSum / n : 0;
    return s;
}

#endif
