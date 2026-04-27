// HeapMaster.hpp (implementation file)
#ifndef HEAPMASTER_HPP
#define HEAPMASTER_HPP

#include "HeapMaster.h"

// TODO: implement ALL functions here

template<typename T, int MAX_SIZE, typename Compare>
void HeapMaster<T, MAX_SIZE, Compare>::heapifyUp(int i) {
    while (i > 0) {
        int p = parent(i);
        if (comp(data[p], data[i])) {
            std::swap(data[p], data[i]);
            i = p;
        } else {
            break;
        }
    }
}

template<typename T, int MAX_SIZE, typename Compare>
void HeapMaster<T, MAX_SIZE, Compare>::heapifyDown(int i) {
    while (true) {
        int best = i;
        int l = left(i);
        int r = right(i);

        if (l < sz && comp(data[best], data[l])) best = l;
        if (r < sz && comp(data[best], data[r])) best = r;

        if (best != i) {
            std::swap(data[i], data[best]);
            i = best;
        } else {
            break;
        }
    }
}

template<typename T, int MAX_SIZE, typename Compare>
const T& HeapMaster<T, MAX_SIZE, Compare>::top() const {
    return data[0];
}

template<typename T, int MAX_SIZE, typename Compare>
void HeapMaster<T, MAX_SIZE, Compare>::push(const T& val) {
    if (sz >= MAX_SIZE) return;
    data[sz] = val;
    heapifyUp(sz);
    sz++;
}

template<typename T, int MAX_SIZE, typename Compare>
void HeapMaster<T, MAX_SIZE, Compare>::pop() {
    if (sz == 0) return;
    data[0] = data[sz - 1];
    sz--;
    if (sz > 0) heapifyDown(0);
}

template<typename T, int MAX_SIZE, typename Compare>
bool HeapMaster<T, MAX_SIZE, Compare>::decreaseKey(int index, const T& newVal) {
    if (index < 0 || index >= sz) return false;
    T oldVal = data[index];
    if (!comp(oldVal, newVal)) return false;
    data[index] = newVal;
    heapifyUp(index);
    return true;
}

template<typename T, int MAX_SIZE, typename Compare>
bool HeapMaster<T, MAX_SIZE, Compare>::increaseKey(int index, const T& newVal) {
    if (index < 0 || index >= sz) return false;
    T oldVal = data[index];
    if (!comp(newVal, oldVal)) return false;
    data[index] = newVal;
    heapifyDown(index);
    return true;
}

template<typename T, int MAX_SIZE, typename Compare>
void HeapMaster<T, MAX_SIZE, Compare>::buildHeap(T arr[], int n) {
    if (n > MAX_SIZE) n = MAX_SIZE;
    for (int i = 0; i < n; ++i) {
        data[i] = arr[i];
    }
    sz = n;
    for (int i = sz / 2 - 1; i >= 0; --i) {
        heapifyDown(i);
    }
}

template<typename T, int MAX_SIZE, typename Compare>
void HeapMaster<T, MAX_SIZE, Compare>::printArray() const {
    for (int i = 0; i < sz; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "\n";
}

template<typename T, int MAX_SIZE, typename Compare>
void HeapMaster<T, MAX_SIZE, Compare>::printHeap() const {
    if (sz == 0) {
        std::cout << "(empty)\n";
        return;
    }
    int levelStart = 0;
    int levelSize = 1;
    int i = 0;
    while (i < sz) {
        for (int j = 0; j < levelSize && i < sz; ++j, ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
        levelStart = levelStart * 2 + 1;
        levelSize *= 2;
    }
}

// helper for heapSort on raw arrays

template<typename T, typename Compare>
void heapifyDownArray(T arr[], int n, int i, Compare comp) {
    while (true) {
        int best = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < n && comp(arr[best], arr[l])) best = l;
        if (r < n && comp(arr[best], arr[r])) best = r;
        if (best != i) {
            std::swap(arr[i], arr[best]);
            i = best;
        } else {
            break;
        }
    }
}

template<typename T>
void heapSort(T arr[], int n) {
    if (n <= 1) return;
    std::less<T> comp;
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapifyDownArray(arr, n, i, comp);
    }
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapifyDownArray(arr, i, 0, comp);
    }
}

#endif
