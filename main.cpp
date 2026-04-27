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

int main() {
    std::cout << "HeapMaster Lab – Complete the challenges!\n";

    HeapMaster<int> maxHeap;
    maxHeap.push(10);
    maxHeap.push(4);
    maxHeap.push(15);
    maxHeap.push(1);

    std::cout << "Max-heap array view: ";
    maxHeap.printArray();

    int arr[6] = {5, 2, 9, 1, 7, 3};
    heapSort(arr, 6);
    std::cout << "heapSort result: ";
    for (int i = 0; i < 6; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    HeapMaster<Task, 10000, PriorityCompare> priHeap;
    Task t1{1, 5, 0, 3, 10};
    Task t2{2, 10, 1, 4, 8};
    Task t3{3, 2, 2, 1, 5};

    priHeap.push(t1);
    priHeap.push(t2);
    priHeap.push(t3);

    std::cout << "Top priority task: ";
    priHeap.top().print();
    std::cout << "\n";

    HeapMaster<Task, 10000, DeadlineCompare> edfHeap;
    Task d1{4, 1, 0, 2, 7};
    Task d2{5, 1, 0, 2, 3};
    Task d3{6, 1, 0, 2, 5};

    edfHeap.push(d1);
    edfHeap.push(d2);
    edfHeap.push(d3);

    std::cout << "Earliest deadline task: ";
    edfHeap.top().print();
    std::cout << "\n";

    return 0;
}
