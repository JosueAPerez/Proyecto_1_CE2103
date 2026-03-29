#ifndef QUICK_SORT_M3_H
#define QUICK_SORT_M3_H

#include <algorithm>
#include "../PagedArray.h"

// elige la mediana entre tres posiciones y la coloca al final
long long medianOfThree(PagedArray& arr, long long low, long long high) {
    long long mid = low + (high - low) / 2;

    // ordenar los tres candidatos entre sí
    if (arr[low] > arr[mid])
        std::swap(arr[low], arr[mid]);
    if (arr[low] > arr[high])
        std::swap(arr[low], arr[high]);
    if (arr[mid] > arr[high])
        std::swap(arr[mid], arr[high]);

    // al llegar aquí: arr[low] <= arr[mid] <= arr[high]
    // colocar la mediana (mid) en high-1 para usarla como pivote
    std::swap(arr[mid], arr[high]);
    return arr[high];  // retornar el pivote
}

long long partitionM3(PagedArray& arr, long long low, long long high) {
    // obtener pivote usando mediana de tres
    int32_t pivot = medianOfThree(arr, low, high);
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

void quickSortM3(PagedArray& arr, long long low, long long high) {
    if (low < high) {
        long long pi = partitionM3(arr, low, high);
        quickSortM3(arr, low, pi - 1);
        quickSortM3(arr, pi + 1, high);
    }
}

#endif