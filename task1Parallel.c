#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    // Получаем размер массива из переменной окружения ARRAY_SIZE
    const char* array_size_str = getenv("ARRAY_SIZE");
    if (array_size_str == NULL) {
        fprintf(stderr, "Error: ARRAY_SIZE environment variable is not set\n");
        return 1;
    }

    int array_size = atoi(array_size_str);
    if (array_size <= 0) {
        fprintf(stderr, "Error: Invalid ARRAY_SIZE value (must be positive)\n");
        return 1;
    }

    // Выделяем память под массив
    double* array = (double*)malloc(array_size * sizeof(double));
    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    // Инициализируем массив
    for (int i = 0; i < array_size; i++) {
        array[i] = i + 1;
    }

    // Начинаем замер времени
    double start_time = omp_get_wtime();

    // Параллельное вычисление суммы
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < array_size; i++) {
        sum += array[i];
    }

    // Заканчиваем замер времени
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // Выводим только время выполнения
    printf("%.6f\n", elapsed_time);
    // Освобождаем память
    free(array);

    return 0;
}
