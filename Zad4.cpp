#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <stdexcept>
#include <iomanip>

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

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <rows> <cols> <mode> [threads]" << endl;
        cerr << "Modes: 1 - sequential, 2 - parallel" << endl;
        return 1;
    }

    int rows = stoi(argv[1]);
    int cols = stoi(argv[2]);
    int mode = stoi(argv[3]);
    int threads = mode == 2 ? (argc > 4 ? stoi(argv[4]) : omp_get_max_threads()) : 1;

    vector<vector<double>> arr1, arr2;
    initialize_arrays(arr1, arr2, rows, cols);

    vector<vector<double>> add(rows, vector<double>(cols));
    vector<vector<double>> sub(rows, vector<double>(cols));
    vector<vector<double>> mul(rows, vector<double>(cols));
    vector<vector<double>> div(rows, vector<double>(cols));

    auto start = high_resolution_clock::now();

    if (mode == 1) {
        operations_sequential(arr1, arr2, add, sub, mul, div, rows, cols);
    }
    else {
        operations_parallel(arr1, arr2, add, sub, mul, div, rows, cols, threads);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << duration << endl; // Выводим только время в микросекундах

    return 0;
}
