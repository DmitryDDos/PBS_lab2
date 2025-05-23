#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <locale>
#include <windows.h>  // Для Windows

using namespace std;
using namespace std::chrono;

void initialize_arrays(vector<vector<double>>& arr1, vector<vector<double>>& arr2, int rows, int cols) {
    arr1.resize(rows, vector<double>(cols));
    arr2.resize(rows, vector<double>(cols));

#pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arr1[i][j] = i + j + 1;
            arr2[i][j] = (i + 1) * (j + 1);
        }
    }
}

void operations_sequential(const vector<vector<double>>& arr1, const vector<vector<double>>& arr2,
    vector<vector<double>>& add, vector<vector<double>>& sub,
    vector<vector<double>>& mul, vector<vector<double>>& div, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            add[i][j] = arr1[i][j] + arr2[i][j];
            sub[i][j] = arr1[i][j] - arr2[i][j];
            mul[i][j] = arr1[i][j] * arr2[i][j];
            div[i][j] = arr2[i][j] != 0 ? arr1[i][j] / arr2[i][j] : 0;
        }
    }
}

void operations_parallel(const vector<vector<double>>& arr1, const vector<vector<double>>& arr2,
    vector<vector<double>>& add, vector<vector<double>>& sub,
    vector<vector<double>>& mul, vector<vector<double>>& div, int rows, int cols, int threads) {
    omp_set_num_threads(threads);
#pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            add[i][j] = arr1[i][j] + arr2[i][j];
            sub[i][j] = arr1[i][j] - arr2[i][j];
            mul[i][j] = arr1[i][j] * arr2[i][j];
            div[i][j] = arr2[i][j] != 0 ? arr1[i][j] / arr2[i][j] : 0;
        }
    }
}

int main() {
    // Установка UTF-8 кодировки для консоли Windows
    SetConsoleOutputCP(CP_UTF8);
    // Установка локали для корректного вывода русского текста
    setlocale(LC_ALL, "Russian");

    int rows = 1000;
    int cols = 1000;
    int threads = 4;

    vector<vector<double>> arr1, arr2;
    initialize_arrays(arr1, arr2, rows, cols);

    vector<vector<double>> add(rows, vector<double>(cols));
    vector<vector<double>> sub(rows, vector<double>(cols));
    vector<vector<double>> mul(rows, vector<double>(cols));
    vector<vector<double>> div(rows, vector<double>(cols));

    // Последовательный запуск
    auto start_seq = high_resolution_clock::now();
    operations_sequential(arr1, arr2, add, sub, mul, div, rows, cols);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<microseconds>(end_seq - start_seq).count();

    // Параллельный запуск
    auto start_par = high_resolution_clock::now();
    operations_parallel(arr1, arr2, add, sub, mul, div, rows, cols, threads);
    auto end_par = high_resolution_clock::now();
    auto duration_par = duration_cast<microseconds>(end_par - start_par).count();

    cout << "Время последовательной версии: " << duration_seq << " мкс" << endl;
    cout << "Время параллельной версии (" << threads << " потоков): " << duration_par << " мкс" << endl;

    return 0;
}
