#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "HeapMaster.h"

struct PriorityCompare {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority < b.priority;
    }
};

struct DeadlineCompare {
    bool operator()(const Task& a, const Task& b) const {
        return a.deadline > b.deadline;
    }
};

struct SRTFTask {
    int id;
    int remainingTime;
    int arrivalTime;
    bool operator<(const SRTFTask& other) const {
        return remainingTime < other.remainingTime;
    }
};

// use a smaller MAX_SIZE for stack-allocated heaps in main
static HeapMaster<int, 100> maxHeap;
static HeapMaster<int, 100, std::greater<int>> minHeap;
static HeapMaster<int, 100> treeHeap;
static HeapMaster<Task, 10000, PriorityCompare> priHeap;
static HeapMaster<Task, 10000, DeadlineCompare> edfHeap;
static HeapMaster<SRTFTask, 1000, std::less<SRTFTask>> srtfHeap;

int main() {
    srand(42);
    std::cout << "HeapMaster Lab - Complete the challenges!\n\n";

    // Challenge 1: Insert 1..20 shuffled, verify valid max-heap
    std::cout << "=== Challenge 1: First Blood ===\n";
    int vals[20];
    for (int i = 0; i < 20; ++i) vals[i] = i + 1;
    for (int i = 19; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(vals[i], vals[j]);
    }
    for (int i = 0; i < 20; ++i) maxHeap.push(vals[i]);
    std::cout << "Array view: ";
    maxHeap.printArray();
    std::cout << "Max on top: " << maxHeap.top() << " (should be 20)\n";

    // Challenge 2: 10 random pops, remaining heap must stay valid
    std::cout << "\n=== Challenge 2: The Extractor ===\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << "Popped: " << maxHeap.top() << "\n";
        maxHeap.pop();
    }
    int prev = maxHeap.top();
    bool valid = true;
    HeapMaster<int, 100> tmp = maxHeap;
    while (!tmp.empty()) {
        int v = tmp.top(); tmp.pop();
        if (v > prev) { valid = false; break; }
        prev = v;
    }
    std::cout << "Remaining top: " << maxHeap.top() << "\n";
    std::cout << "Heap valid: " << (valid ? "YES" : "NO") << "\n";

    // Challenge 3: Min-heap with std::greater
    std::cout << "\n=== Challenge 3: Min vs Max Kingdom ===\n";
    minHeap.push(10); minHeap.push(4); minHeap.push(15); minHeap.push(1); minHeap.push(7);
    std::cout << "Min-heap top: " << minHeap.top() << " (should be 1)\n";
    minHeap.pop();
    std::cout << "After pop top: " << minHeap.top() << " (should be 4)\n";

    // Challenge 4: heapSort
    std::cout << "\n=== Challenge 4: HeapSort Conquest ===\n";
    int arr[6] = {5, 2, 9, 1, 7, 3};
    heapSort(arr, 6);
    std::cout << "heapSort result: ";
    for (int i = 0; i < 6; ++i) std::cout << arr[i] << " ";
    std::cout << "\n";

    // Challenge 5: Pretty tree print
    std::cout << "\n=== Challenge 5: Tree Vision ===\n";
    int treeVals[] = {45, 68, 60, 50, 33, 48, 40, 30, 72, 81, 62, 55, 85, 77, 99};
    for (int v : treeVals) treeHeap.push(v);
    treeHeap.printHeap();

    // Challenge 6: Task Scheduler - Priority
    std::cout << "\n=== Challenge 6: Task Scheduler - Priority ===\n";
    Task t1{1, 5, 0, 3, 10};
    Task t2{2, 10, 1, 4, 8};
    Task t3{3, 2, 2, 1, 5};
    priHeap.push(t1); priHeap.push(t2); priHeap.push(t3);
    std::cout << "Top priority task: "; priHeap.top().print(); std::cout << "\n";

    // Challenge 7: EDF
    std::cout << "\n=== Challenge 7: Earliest Deadline First ===\n";
    Task d1{4, 1, 0, 2, 7};
    Task d2{5, 1, 0, 2, 3};
    Task d3{6, 1, 0, 2, 5};
    edfHeap.push(d1); edfHeap.push(d2); edfHeap.push(d3);
    std::cout << "Earliest deadline task: "; edfHeap.top().print(); std::cout << "\n";

    // Challenge 8: SRTF simulation
    std::cout << "\n=== Challenge 8: SRTF Scheduling ===\n";
    srtfHeap.push({1, 10, 0});
    srtfHeap.push({2, 3,  1});
    srtfHeap.push({3, 5,  2});
    int time = 0;
    while (!srtfHeap.empty()) {
        SRTFTask task = srtfHeap.top(); srtfHeap.pop();
        std::cout << "Time " << time << ": Running Task " << task.id
                  << " (rem=" << task.remainingTime << ")\n";
        task.remainingTime -= 1;
        time++;
        if (task.remainingTime > 0) srtfHeap.push(task);
    }

    return 0;
}
