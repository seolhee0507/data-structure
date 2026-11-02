#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DATA_SIZE 20000
#define RANDOM_MAX 1000000

typedef unsigned long long ull;

/*
 * Insertion Sort
 * comparisons에는 배열 원소끼리의 비교만 포함한다.
 * j >= 0과 같은 인덱스 조건 검사는 포함하지 않는다.
 */
void insertion_sort(int arr[], int n, ull *comparisons) {
    *comparisons = 0;

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            (*comparisons)++;

            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }

        arr[j + 1] = key;
    }
}

/*
 * Shell Sort - Shell's Original Sequence
 * gap: n/2, n/4, ..., 1
 */
void shell_sort_original(int arr[], int n, ull *comparisons) {
    *comparisons = 0;

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap) {
                (*comparisons)++;

                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                } else {
                    break;
                }
            }

            arr[j] = temp;
        }
    }
}

/*
 * Shell Sort - Knuth Sequence
 * Knuth gap: 1, 4, 13, 40, 121, ...
 * n보다 작은 가장 큰 gap부터 역순으로 적용한다.
 */
void shell_sort_knuth(int arr[], int n, ull *comparisons) {
    *comparisons = 0;

    int gap = 1;

    /* 3 * gap + 1이 n보다 작거나 같은 동안 간격을 키운다. */
    while (gap <= (n - 1) / 3) {
        gap = 3 * gap + 1;
    }

    while (gap >= 1) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap) {
                (*comparisons)++;

                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                } else {
                    break;
                }
            }

            arr[j] = temp;
        }

        /* Knuth 수열의 역순으로 다음 간격을 선택한다. */
        gap = (gap - 1) / 3;
    }
}

/* 오름차순 정렬 여부를 검사한다. 검사 횟수는 정렬 비교 횟수에 포함하지 않는다. */
int is_sorted(const int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    const int sizes[] = {1000, 2000, 4000, 8000, 16000, 20000};
    const int size_count = (int)(sizeof(sizes) / sizeof(sizes[0]));

    int *original = (int *)malloc(MAX_DATA_SIZE * sizeof(int));
    int *insertion = (int *)malloc(MAX_DATA_SIZE * sizeof(int));
    int *shell_original = (int *)malloc(MAX_DATA_SIZE * sizeof(int));
    int *shell_knuth = (int *)malloc(MAX_DATA_SIZE * sizeof(int));

    if (original == NULL || insertion == NULL ||
        shell_original == NULL || shell_knuth == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(original);
        free(insertion);
        free(shell_original);
        free(shell_knuth);
        return 1;
    }

    /*
     * 고정 시드의 32비트 선형 합동 생성기(LCG)를 사용한다.
     * rand() 구현 차이에 따른 결과 변동을 줄여 실험을 재현할 수 있다.
     */
    uint32_t random_state = 2026u;

    for (int s = 0; s < size_count; s++) {
        int n = sizes[s];

        /* 해당 데이터 크기의 원본 무작위 데이터 생성 */
        for (int i = 0; i < n; i++) {
            random_state = random_state * 1664525u + 1013904223u;
            original[i] = (int)(random_state % (RANDOM_MAX + 1u));
        }

        /*
         * 세 알고리즘에 동일한 원본 데이터를 제공한다.
         * 배열 복사 과정은 비교 횟수 측정에 포함하지 않는다.
         */
        memcpy(insertion, original, n * sizeof(int));
        memcpy(shell_original, original, n * sizeof(int));
        memcpy(shell_knuth, original, n * sizeof(int));

        ull insertion_count;
        ull original_count;
        ull knuth_count;

        insertion_sort(insertion, n, &insertion_count);
        shell_sort_original(shell_original, n, &original_count);
        shell_sort_knuth(shell_knuth, n, &knuth_count);

        /* 세 정렬 결과가 모두 오름차순인지 확인 */
        if (!is_sorted(insertion, n) ||
            !is_sorted(shell_original, n) ||
            !is_sorted(shell_knuth, n)) {
            fprintf(stderr, "Sorting verification failed for n = %d.\n", n);
            free(original);
            free(insertion);
            free(shell_original);
            free(shell_knuth);
            return 1;
        }

        printf("Data Size : %d\n", n);
        printf("Insertion Sort       : %llu comparisons\n", insertion_count);
        printf("Shell Sort (Original): %llu comparisons\n", original_count);
        printf("Shell Sort (Knuth)   : %llu comparisons\n", knuth_count);
        printf("Verification         : PASS\n\n");
    }

    free(original);
    free(insertion);
    free(shell_original);
    free(shell_knuth);

    return 0;
}
