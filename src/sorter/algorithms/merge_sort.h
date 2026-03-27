#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include "../PagedArray.h"

void merge(PagedArray& arr, long long left, long long mid, long long right) {
    long long leftSize = mid - left + 1;
    long long rightSize = right - mid;

    // arreglos temporales
    int32_t* leftArr = new int32_t[leftSize];
    int32_t* rightArr = new int32_t[rightSize];

    // copiar datos a los arreglos temporales
    for (long long i = 0; i < leftSize; i++)
        leftArr[i] = arr[left + i];
    for (long long j = 0; j < rightSize; j++)
        rightArr[j] = arr[mid + 1 + j];

    // mezclar los arreglos temporales de vuelta
    long long i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // copiar los elementos restantes
    while (i < leftSize) { arr[k] = leftArr[i]; i++; k++; }
    while (j < rightSize) { arr[k] = rightArr[j]; j++; k++; }

    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(PagedArray& arr, long long left, long long right) {
    if (left < right) {
        long long mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

#endif