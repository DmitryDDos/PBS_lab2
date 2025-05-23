#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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

    // Инициализируем массив (например, последовательными значениями)
    for (int i = 0; i < array_size; i++) {
        array[i] = i + 1; // Можно заменить на любую другую инициализацию
    }

    // Параллельное вычисление суммы с OpenMP
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < array_size; i++) {
        sum += array[i];
    }

    // Выводим результат
    printf("Array size: %d\n", array_size);
    printf("Sum of array elements: %.2f\n", sum);

    // Освобождаем память
    free(array);

    return 0;
}
