// HeapMaster.h
#ifndef HEAPMASTER_H
#define HEAPMASTER_H

#include <iostream>
#include <iomanip>
#include <cstring>

// ====================== TASK STRUCT (for challenges 6–10) =======================
struct Task {
    int id;
    int priority;      // higher number = higher priority
    int arrivalTime;
    int burstTime;
    int deadline;

    void print() const {
        std::cout << "Task#" << id
                  << " pri=" << priority
                  << " arrive=" << arrivalTime
                  << " burst=" << burstTime
                  << " dead=" << deadline;
    }
};

// ============================= HEAP MASTER CLASS ===============================
#include <functional>

template<typename T, int MAX_SIZE = 1000005, typename Compare = std::less<T>>
class HeapMaster {
private:
    T data[MAX_SIZE];
    int sz;                    // current size
    Compare comp;              // comparator (less = max-heap, greater = min-heap)

    int parent(int i) const { return (i - 1) / 2; }
    int left(int i) const   { return 2 * i + 1; }
    int right(int i) const  { return 2 * i + 2; }

    void heapifyUp(int i);
    void heapifyDown(int i);

public:
    HeapMaster() : sz(0) {}

    bool empty() const { return sz == 0; }
    int size() const   { return sz; }
    const T& top() const;

    void push(const T& val);
    void pop();
    void clear() { sz = 0; }

    bool decreaseKey(int index, const T& newVal);
    bool increaseKey(int index, const T& newVal);

    void buildHeap(T arr[], int n);

    void printHeap() const;
    void printArray() const;
};

template<typename T>
void heapSort(T arr[], int n);

#include "HeapMaster.hpp"

#endif
