#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <algorithm>
#include "../PagedArray.h"

void bubbleSort(PagedArray& arr, long long size) {
    for (long long i = 0; i < size - 1; i++) {
        for (long long j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

#endif