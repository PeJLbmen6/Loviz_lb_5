#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    int N, i, j;

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    printf("Введите количество вершин графа: ");
    scanf_s("%d", &N);

    // Создание динамической матрицы
    int** G = (int**)malloc(N * sizeof(int*));
    for (i = 0; i < N; i++) {
        G[i] = (int*)malloc(N * sizeof(int));
    }

    int choice;
    printf("\nВыберите способ заполнения матрицы:\n");
    printf("1 - Вручную\n");
    printf("2 - Случайная генерация\n");
    printf("Ваш выбор: ");
    scanf_s("%d", &choice);

    if (choice == 1) {
        // Ручной ввод матрицы
        printf("\nВведите матрицу смежности %dx%d:\n", N, N);
        for (i = 0; i < N; i++) {
            printf("Строка %d: ", i);
            for (j = 0; j < N; j++) {
                scanf_s("%d", &G[i][j]);
            }
        }
    }
    else {
        // Случайная генерация графа
        int edgeProbability;
        printf("Введите вероятность ребра между вершинами (0-100%%): ");
        scanf_s("%d", &edgeProbability);

        int loopProbability;
        printf("Введите вероятность петли (0-100%%): ");
        scanf_s("%d", &loopProbability);

        // Генерация матрицы смежности
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (i == j) {
                    // Петля - с заданной вероятностью
                    G[i][j] = (rand() % 100 < loopProbability) ? 1 : 0;
                }
                else if (i < j) {
                    // Ребро между разными вершинами - с заданной вероятностью
                    int hasEdge = (rand() % 100 < edgeProbability) ? 1 : 0;
                    G[i][j] = hasEdge;
                    G[j][i] = hasEdge; // Симметричность для неориентированного графа
                }
            }
        }

        printf("\nСгенерированная матрица смежности:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", G[i][j]);
            }
            printf("\n");
        }
    }

    // Анализ характеристик графа
    int size = 0, loops = 0;
    int* deg = (int*)calloc(N, sizeof(int));
    int hasIsolated = 0;
    int* isDominating = (int*)calloc(N, sizeof(int));

    // Вычисление степеней вершин и других характеристик
    for (i = 0; i < N; i++) {
        for (j = i; j < N; j++) {
            if (G[i][j]) {
                if (i == j) {
                    loops++;
                    deg[i] += 2;
                    size++;
                }
                else {
                    deg[i]++;
                    deg[j]++;
                    size++;
                }
            }
        }
    }

    // Проверка на изолированные вершины
    for (i = 0; i < N; i++) {
        if (deg[i] == 0) {
            hasIsolated = 1;
            break;
        }
    }

    // Проверка на доминирующие вершины (только если нет изолированных)
    if (!hasIsolated) {
        for (i = 0; i < N; i++) {
            int connectedToAll = 1;
            for (j = 0; j < N; j++) {
                if (i != j && G[i][j] == 0) {
                    connectedToAll = 0;
                    break;
                }
            }
            if (connectedToAll && N > 1) {
                isDominating[i] = 1;
            }
        }
    }

    // Вывод результатов анализа
    printf("\n=== РЕЗУЛЬТАТЫ АНАЛИЗА ГРАФА ===\n");
    printf("Количество вершин (N): %d\n", N);
    printf("Размер графа (Size): %d\n", size);
    printf("Количество петель (Loops): %d\n", loops);

    printf("\nСТЕПЕНИ ВЕРШИН:\n");
    for (i = 0; i < N; i++) {
        printf("Вершина %d: %d", i, deg[i]);
        if (G[i][i] != 0) printf(" (есть петля)");
        printf("\n");
    }

    printf("\nСПЕЦИАЛЬНЫЕ ВЕРШИНЫ:\n");

    // Изолированные вершины
    printf("Изолированные вершины: ");
    int isolatedCount = 0;
    for (i = 0; i < N; i++) {
        if (deg[i] == 0) {
            printf("%d ", i);
            isolatedCount++;
        }
    }
    if (isolatedCount == 0) printf("отсутствуют");
    printf("\n");

    // Концевые вершины
    printf("Концевые вершины: ");
    int terminalCount = 0;
    for (i = 0; i < N; i++) {
        if (deg[i] == 1) {
            printf("%d ", i);
            terminalCount++;
        }
    }
    if (terminalCount == 0) printf("отсутствуют");
    printf("\n");

    // Доминирующие вершины
    printf("Доминирующие вершины: ");
    if (hasIsolated) {
        printf("не существуют (есть изолированные вершины)");
    }
    else {
        int dominatingCount = 0;
        for (i = 0; i < N; i++) {
            if (isDominating[i]) {
                printf("%d ", i);
                dominatingCount++;
            }
        }
        if (dominatingCount == 0) {
            printf("отсутствуют");
        }
    }
    printf("\n");

    // Дополнительная статистика
    printf("\nДОПОЛНИТЕЛЬНАЯ СТАТИСТИКА:\n");
    printf("Сумма степеней всех вершин: %d\n", size * 2);
    printf("Средняя степень вершины: %.2f\n", (float)(size * 2) / N);

    // Поиск вершины с максимальной степенью
    int maxDeg = 0, maxDegVertex = -1;
    for (i = 0; i < N; i++) {
        if (deg[i] > maxDeg) {
            maxDeg = deg[i];
            maxDegVertex = i;
        }
    }
    printf("Вершина с максимальной степенью: %d (степень %d)\n", maxDegVertex, maxDeg);

    // Поиск вершины с минимальной степенью (исключая изолированные)
    int minDeg = N * 2, minDegVertex = -1;
    for (i = 0; i < N; i++) {
        if (deg[i] > 0 && deg[i] < minDeg) {
            minDeg = deg[i];
            minDegVertex = i;
        }
    }
    if (minDegVertex != -1) {
        printf("Вершина с минимальной степенью: %d (степень %d)\n", minDegVertex, minDeg);
    }

    // Освобождение памяти
    for (i = 0; i < N; i++) {
        free(G[i]);
    }
    free(G);
    free(deg);
    free(isDominating);

    return 0;
}