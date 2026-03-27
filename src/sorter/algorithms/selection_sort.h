#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <algorithm>
#include "../PagedArray.h"

void selectionSort(PagedArray& arr, long long size) {
    for (long long i = 0; i < size - 1; i++) {
        long long minIndex = i;
        for (long long j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}

#endif