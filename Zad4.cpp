
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <algorithm> // Для std::sort
#include <string>    // Для std::stoi

using namespace std;
using namespace std::chrono;

// Функция для инициализации двумерного массива случайными числами
void initializeArray(vector<vector<double>>& arr, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arr[i][j] = static_cast<double>(rand()) / RAND_MAX * 10.0; // Числа от 0 до 10
        }
    }
}

// Функция для последовательной сортировки подмассивов
void sequentialSortSubarrays(vector<vector<double>>& arr, int rows, int cols, int num_parts) {
    int rows_per_part = rows / num_parts;
    if (rows_per_part == 0) rows_per_part = 1; // Защита от деления на 0 или слишком малого числа частей

    for (int part = 0; part < num_parts; ++part) {
        int start_row = part * rows_per_part;
        int end_row = (part == num_parts - 1) ? rows : (part + 1) * rows_per_part;
        // Преобразуем подмассив в одномерный вектор для сортировки
        vector<double> subarray;
        for (int i = start_row; i < end_row && i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                subarray.push_back(arr[i][j]);
            }
        }
        // Сортируем подмассив
        sort(subarray.begin(), subarray.end());
        // Записываем отсортированные данные обратно
        size_t idx = 0;
        for (int i = start_row; i < end_row && i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (idx < subarray.size()) {
                    arr[i][j] = subarray[idx++];
                }
            }
        }
    }
}

// Функция для параллельной сортировки подмассивов с OpenMP
void parallelSortSubarrays(vector<vector<double>>& arr, int rows, int cols, int num_threads) {
    int rows_per_part = rows / num_threads;
    if (rows_per_part == 0) rows_per_part = 1; // Защита от деления на 0

    omp_set_num_threads(num_threads);
#pragma omp parallel for
    for (int part = 0; part < num_threads; ++part) {
        int start_row = part * rows_per_part;
        int end_row = (part == num_threads - 1) ? rows : (part + 1) * rows_per_part;
        // Преобразуем подмассив в одномерный вектор для сортировки
        vector<double> subarray;
        for (int i = start_row; i < end_row && i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                subarray.push_back(arr[i][j]);
            }
        }
        // Сортируем подмассив
        sort(subarray.begin(), subarray.end());
        // Записываем отсортированные данные обратно
        size_t idx = 0;
        for (int i = start_row; i < end_row && i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (idx < subarray.size()) {
                    arr[i][j] = subarray[idx++];
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Параметры для двумерных массивов
    const int ROWS = 1000;
    const int COLS = 1000; // 400*400 = 160000 элементов (>100000)
    vector<vector<double>> A(ROWS, vector<double>(COLS));
    vector<vector<double>> B(ROWS, vector<double>(COLS));

    // Инициализация данных
    srand(static_cast<unsigned>(time(0)));
    initializeArray(A, ROWS, COLS);
    initializeArray(B, ROWS, COLS);

    // Определение количества ядер (и потоков)
    int num_cores;
    if (argc > 1) {
        // Если передан аргумент командной строки (для суперкомпьютера)
        num_cores = stoi(argv[1]);
        cout << "Number of cores (from command line): " << num_cores << endl;
    }
    else {
        // Иначе запрашиваем у пользователя (для локального компьютера)
        cout << "Enter the number of cores (threads) to use: ";
        cin >> num_cores;
        while (num_cores <= 0) {
            cout << "Please enter a positive number of cores: ";
            cin >> num_cores;
        }
        cout << "Number of cores set to: " << num_cores << endl;
    }

    // --- Сортировка подмассивов для массива A ---
    cout << "=== Sorting Subarrays for Array A ===\n";
    // Последовательное выполнение
    vector<vector<double>> A_seq = A; // Копия для последовательной обработки
    auto start = high_resolution_clock::now();
    sequentialSortSubarrays(A_seq, ROWS, COLS, num_cores); // Делим на num_cores частей
    auto end = high_resolution_clock::now();
    duration<double> seq_duration = duration_cast<duration<double>>(end - start);
    cout << "Sequential Sorting Subarrays (A) time: " << seq_duration.count() << " seconds\n";

    // Параллельное выполнение с заданным количеством потоков
    vector<vector<double>> A_par = A; // Копия для параллельной обработки
    start = high_resolution_clock::now();
    parallelSortSubarrays(A_par, ROWS, COLS, num_cores);
    end = high_resolution_clock::now();
    duration<double> par_duration = duration_cast<duration<double>>(end - start);
    cout << "Parallel Sorting Subarrays (A) time with " << num_cores << " threads: " << par_duration.count() << " seconds\n";

    // --- Сортировка подмассивов для массива B ---
    cout << "\n=== Sorting Subarrays for Array B ===\n";
    // Последовательное выполнение
    vector<vector<double>> B_seq = B; // Копия для последовательной обработки
    start = high_resolution_clock::now();
    sequentialSortSubarrays(B_seq, ROWS, COLS, num_cores); // Делим на num_cores частей
    end = high_resolution_clock::now();
    seq_duration = duration_cast<duration<double>>(end - start);
    cout << "Sequential Sorting Subarrays (B) time: " << seq_duration.count() << " seconds\n";

    // Параллельное выполнение с заданным количеством потоков
    vector<vector<double>> B_par = B; // Копия для параллельной обработки
    start = high_resolution_clock::now();
    parallelSortSubarrays(B_par, ROWS, COLS, num_cores);
    end = high_resolution_clock::now();
    par_duration = duration_cast<duration<double>>(end - start);
    cout << "Parallel Sorting Subarrays (B) time with " << num_cores << " threads: " << par_duration.count() << " seconds\n";

    return 0;
}
