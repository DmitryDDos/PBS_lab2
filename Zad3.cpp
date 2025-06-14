#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <iomanip> // Для форматирования вывода

void quickSort(std::vector<int>& arr, int left, int right) {
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

void parallelQuickSort(std::vector<int>& arr, int left, int right) {
    if (right - left < 1000) {
        quickSort(arr, left, right);
    }
    else {
        int i = left, j = right;
        int pivot = arr[(left + right) / 2];

        while (i <= j) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
            if (i <= j) {
                std::swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }

#pragma omp task shared(arr)
        if (left < j) parallelQuickSort(arr, left, j);
#pragma omp task shared(arr)
        if (i < right) parallelQuickSort(arr, i, right);
    }
}

int main(int argc, char* argv[]) {
    int numThreads;

    // Проверка, передано ли количество ядер в аргументах командной строки
    if (argc == 2) {
        numThreads = std::atoi(argv[1]);
    }
    else {
        std::cout << "Enter the number of threads: ";
        std::cin >> numThreads;
    }

    omp_set_num_threads(numThreads);
    std::cout << "Number of threads set to: " << numThreads << std::endl;

    const size_t ARRAY_SIZE = 1000000;

    // Создание двух массивов
    std::vector<int> array1(ARRAY_SIZE);
    std::vector<int> array2(ARRAY_SIZE);

    // Заполнение массивов случайными числами
    srand(static_cast<unsigned>(time(0)));
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        array1[i] = rand() % 100;
        array2[i] = rand() % 100;
    }

    // Сортировка первого массива (последовательная)
    double startTime1 = omp_get_wtime();
    quickSort(array1, 0, ARRAY_SIZE - 1);
    double elapsedTime1 = omp_get_wtime() - startTime1;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Sorting of the first array (sequential) completed in " << elapsedTime1 << " seconds." << std::endl;

    // Сортировка второго массива (последовательная)
    double startTime2 = omp_get_wtime();
    quickSort(array2, 0, ARRAY_SIZE - 1);
    double elapsedTime2 = omp_get_wtime() - startTime2;
    std::cout << "Sorting of the second array (sequential) completed in " << elapsedTime2 << " seconds." << std::endl;

    // Заполнение первого массива случайными числами снова для параллельной сортировки
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        array1[i] = rand() % 100;
    }

    // Сортировка первого массива (параллельная)
    double startTime3 = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp single
        parallelQuickSort(array1, 0, ARRAY_SIZE - 1);
    }
    double elapsedTime3 = omp_get_wtime() - startTime3;
    std::cout << "Sorting of the first array (parallel) completed in " << elapsedTime3 << " seconds." << std::endl;

    // Заполнение второго массива случайными числами снова для параллельной сортировки
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        array2[i] = rand() % 100;
    }

    // Сортировка второго массива (параллельная)
    double startTime4 = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp single
        parallelQuickSort(array2, 0, ARRAY_SIZE - 1);
    }
    double elapsedTime4 = omp_get_wtime() - startTime4;
    std::cout << "Sorting of the second array (parallel) completed in " << elapsedTime4 << " seconds." << std::endl;

    return 0;
}
