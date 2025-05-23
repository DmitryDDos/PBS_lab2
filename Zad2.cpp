#include <iostream>
#include <algorithm>
#include <omp.h>
#include <cstdlib> // Для std::atoi

// Последовательная реализация partition
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

// Последовательная реализация быстрой сортировки
void quickSort_sequential(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort_sequential(arr, low, pi - 1);
        quickSort_sequential(arr, pi + 1, high);
    }
}

// Параллельная реализация быстрой сортировки с OpenMP
void quickSort_parallel(int* arr, int low, int high, int depth = 0) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Ограничиваем глубину рекурсии для параллелизма
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_elements>" << std::endl;
        return 1;
    }

    const size_t SIZE = std::atoi(argv[1]); // Число элементов передается в качестве аргумента
    int* dynamicArray_seq = new int[SIZE];
    int* dynamicArray_par = new int[SIZE];

    // Инициализация массива случайными числами
    for (size_t i = 0; i < SIZE; ++i) {
        int val = rand() % 10000;
        dynamicArray_seq[i] = val;
        dynamicArray_par[i] = val;
    }

    // Последовательная сортировка
    quickSort_sequential(dynamicArray_seq, 0, SIZE - 1);

    // Параллельная сортировка
#pragma omp parallel
    {
#pragma omp single
        quickSort_parallel(dynamicArray_par, 0, SIZE - 1);
    }

    // Удаление динамически выделенной памяти
    delete[] dynamicArray_seq;
    delete[] dynamicArray_par;
    return 0;
}
