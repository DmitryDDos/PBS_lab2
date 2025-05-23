#include <iostream>
#include <algorithm>
#include <omp.h>
#include <cstdlib>
#include <chrono>
#include <locale>

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort_sequential(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort_sequential(arr, low, pi - 1);
        quickSort_sequential(arr, pi + 1, high);
    }
}

void quickSort_parallel(int* arr, int low, int high, int depth = 0) {
    if (low < high) {
        int pi = partition(arr, low, high);
        if (depth < omp_get_max_threads()) {
#pragma omp task
            quickSort_parallel(arr, low, pi - 1, depth + 1);
#pragma omp task
            quickSort_parallel(arr, pi + 1, high, depth + 1);
#pragma omp taskwait
        }
        else {
            quickSort_parallel(arr, low, pi - 1, depth);
            quickSort_parallel(arr, pi + 1, high, depth);
        }
    }
}

int main() {
    // Установка локали (системная локаль, обычно UTF-8)
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    const size_t SIZE = 1000000;

    int* dynamicArray_seq = new int[SIZE];
    int* dynamicArray_par = new int[SIZE];

    for (size_t i = 0; i < SIZE; ++i) {
        int val = rand() % 10000;
        dynamicArray_seq[i] = val;
        dynamicArray_par[i] = val;
    }

    auto start_seq = std::chrono::high_resolution_clock::now();
    quickSort_sequential(dynamicArray_seq, 0, SIZE - 1);
    auto end_seq = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seq = end_seq - start_seq;

    auto start_par = std::chrono::high_resolution_clock::now();
#pragma omp parallel
    {
#pragma omp single
        quickSort_parallel(dynamicArray_par, 0, SIZE - 1);
    }
    auto end_par = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_par = end_par - start_par;

    std::wcout << L"Время выполнения последовательной сортировки: " << elapsed_seq.count() << L" секунд." << std::endl;
    std::wcout << L"Время выполнения параллельной сортировки: " << elapsed_par.count() << L" секунд." << std::endl;

    delete[] dynamicArray_seq;
    delete[] dynamicArray_par;

    return 0;
}
