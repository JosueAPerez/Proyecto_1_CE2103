#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <algorithm>
#include "../PagedArray.h"

long long partition(PagedArray& arr, long long low, long long high) {
    int32_t pivot = arr[high];
    long long i = low - 1;

    for (long long j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(PagedArray& arr, long long low, long long high) {
    if (low < high) {
        long long pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

#endif