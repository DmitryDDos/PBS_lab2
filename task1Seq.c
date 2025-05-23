#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Получаем размер массива из переменной окружения
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

    // Выделяем память и инициализируем массив
    double* array = (double*)malloc(array_size * sizeof(double));
    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < array_size; i++) {
        array[i] = i + 1;  // Простая инициализация (можно заменить)
    }

    // Замер времени начала
    clock_t start = clock();

    // Последовательное вычисление суммы
    double sum = 0.0;
    for (int i = 0; i < array_size; i++) {
        sum += array[i];
    }

    // Замер времени окончания
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Выводим только время выполнения
    printf("%.6f\n", elapsed_time);

    free(array);
    return 0;
}
