#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include "../PagedArray.h"

void insertionSort(PagedArray& arr, long long size) {
    for (long long i = 1; i < size; i++) {
        int32_t key = arr[i];
        long long j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

#endif