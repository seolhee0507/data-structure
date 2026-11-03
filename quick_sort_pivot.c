#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_N 20000
#define BONUS_N 10000
#define TIME_REPEATS 30

typedef unsigned long long ULL;

typedef enum {
    PIVOT_FIRST,
    PIVOT_RANDOM,
    PIVOT_MEDIAN3
} PivotMode;

static ULL comparisons;

static void swap_int(int *a, int *b) {
    if (a != b) {
        int t = *a;
        *a = *b;
        *b = t;
    }
}

/* Median-of-three: counts only comparisons between data values. */
static int median_of_three_index(const int a[], int low, int high) {
    int mid = low + (high - low) / 2;
    int x = a[low], y = a[mid], z = a[high];

    comparisons++;
    if (x < y) {
        comparisons++;
        if (y < z) return mid;
        comparisons++;
        return (x < z) ? high : low;
    } else {
        comparisons++;
        if (x < z) return low;
        comparisons++;
        return (y < z) ? high : mid;
    }
}

static int choose_pivot_index(const int a[], int low, int high, PivotMode mode) {
    if (mode == PIVOT_FIRST) return low;
    if (mode == PIVOT_RANDOM) {
        return low + rand() % (high - low + 1);
    }
    return median_of_three_index(a, low, high);
}

/* Lomuto partition is shared by all three pivot-selection strategies. */
static int partition_array(int a[], int low, int high, PivotMode mode) {
    int pivot_index = choose_pivot_index(a, low, high, mode);
    int pivot;
    swap_int(&a[pivot_index], &a[high]);
    pivot = a[high];

    int i = low - 1;
    for (int j = low; j < high; j++) {
        comparisons++;
        if (a[j] <= pivot) {
            i++;
            swap_int(&a[i], &a[j]);
        }
    }
    swap_int(&a[i + 1], &a[high]);
    return i + 1;
}

/* Recursive quicksort with tail-recursion elimination on the larger side.
   This keeps recursion depth bounded while preserving the same partitioning. */
static void quick_sort_recursive_impl(int a[], int low, int high, PivotMode mode) {
    while (low < high) {
        int p = partition_array(a, low, high, mode);
        if (p - low < high - p) {
            quick_sort_recursive_impl(a, low, p - 1, mode);
            low = p + 1;
        } else {
            quick_sort_recursive_impl(a, p + 1, high, mode);
            high = p - 1;
        }
    }
}

static ULL quick_sort_recursive(int a[], int n, PivotMode mode) {
    comparisons = 0;
    quick_sort_recursive_impl(a, 0, n - 1, mode);
    return comparisons;
}

typedef struct {
    int low;
    int high;
} Range;

/* Iterative quicksort using an explicit stack. */
static ULL quick_sort_iterative(int a[], int n, PivotMode mode) {
    comparisons = 0;
    Range *stack = (Range *)malloc((size_t)n * sizeof(Range));
    if (!stack) {
        fprintf(stderr, "Stack allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    int top = 0;
    stack[top++] = (Range){0, n - 1};

    while (top > 0) {
        Range current = stack[--top];
        int low = current.low;
        int high = current.high;

        while (low < high) {
            int p = partition_array(a, low, high, mode);
            int left_size = p - low;
            int right_size = high - p;

            /* Push the larger range and process the smaller one immediately.
               This limits explicit stack use even for highly unbalanced splits. */
            if (left_size < right_size) {
                if (p + 1 < high) stack[top++] = (Range){p + 1, high};
                high = p - 1;
            } else {
                if (low < p - 1) stack[top++] = (Range){low, p - 1};
                low = p + 1;
            }
        }
    }

    free(stack);
    return comparisons;
}

static int is_sorted(const int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) return 0;
    }
    return 1;
}

static void make_random_data(int a[], int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 1000001; /* duplicates are allowed */
    }
}

static const char *pivot_name(PivotMode mode) {
    switch (mode) {
        case PIVOT_FIRST: return "First";
        case PIVOT_RANDOM: return "Random";
        default: return "Median-3";
    }
}

static double benchmark_recursive(const int source[], int n, int repeats) {
    int *work = (int *)malloc((size_t)n * sizeof(int));
    if (!work) exit(EXIT_FAILURE);
    double total_ms = 0.0;

    /* Warm-up */
    memcpy(work, source, (size_t)n * sizeof(int));
    quick_sort_recursive(work, n, PIVOT_MEDIAN3);

    for (int r = 0; r < repeats; r++) {
        memcpy(work, source, (size_t)n * sizeof(int));
        clock_t t1 = clock();
        quick_sort_recursive(work, n, PIVOT_MEDIAN3);
        clock_t t2 = clock();
        total_ms += 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;
    }
    free(work);
    return total_ms / repeats;
}

static double benchmark_iterative(const int source[], int n, int repeats) {
    int *work = (int *)malloc((size_t)n * sizeof(int));
    if (!work) exit(EXIT_FAILURE);
    double total_ms = 0.0;

    /* Warm-up */
    memcpy(work, source, (size_t)n * sizeof(int));
    quick_sort_iterative(work, n, PIVOT_MEDIAN3);

    for (int r = 0; r < repeats; r++) {
        memcpy(work, source, (size_t)n * sizeof(int));
        clock_t t1 = clock();
        quick_sort_iterative(work, n, PIVOT_MEDIAN3);
        clock_t t2 = clock();
        total_ms += 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;
    }
    free(work);
    return total_ms / repeats;
}

int main(void) {
    const int sizes[] = {1000, 2000, 4000, 8000, 16000, 20000};
    const int count = (int)(sizeof(sizes) / sizeof(sizes[0]));
    int *original = (int *)malloc(MAX_N * sizeof(int));
    int *work = (int *)malloc(MAX_N * sizeof(int));
    if (!original || !work) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(original);
        free(work);
        return EXIT_FAILURE;
    }

    printf("Quick Sort Pivot Strategy Comparison\n");
    printf("====================================\n");
    printf("Comparison count includes data-value comparisons in partitioning and\n");
    printf("the comparisons used to select the median-of-three pivot.\n\n");

    for (int s = 0; s < count; s++) {
        int n = sizes[s];
        make_random_data(original, n, 20261103U + (unsigned int)n);
        printf("Data Size : %d\n", n);

        for (int mode = PIVOT_FIRST; mode <= PIVOT_MEDIAN3; mode++) {
            memcpy(work, original, (size_t)n * sizeof(int));
            ULL result = quick_sort_recursive(work, n, (PivotMode)mode);
            if (!is_sorted(work, n)) {
                fprintf(stderr, "%s sort failed for n=%d\n", pivot_name((PivotMode)mode), n);
                free(original);
                free(work);
                return EXIT_FAILURE;
            }
            printf("Quick Sort (%-8s) : %llu comparisons\n", pivot_name((PivotMode)mode), result);
        }
        printf("\n");
    }

    /* Bonus: same original 10,000-element data and Median-of-Three pivot. */
    make_random_data(original, BONUS_N, 20261103U + BONUS_N);
    printf("Bonus: Recursive vs Iterative Quick Sort\n");
    printf("Data Size : %d\n", BONUS_N);
    printf("Pivot     : Median-of-Three\n");
    printf("Average of %d runs; sorting time only (copying excluded).\n", TIME_REPEATS);
    double recursive_ms = benchmark_recursive(original, BONUS_N, TIME_REPEATS);
    double iterative_ms = benchmark_iterative(original, BONUS_N, TIME_REPEATS);
    printf("Recursive Quick Sort : %.6f ms\n", recursive_ms);
    printf("Iterative Quick Sort : %.6f ms\n", iterative_ms);

    free(original);
    free(work);
    return EXIT_SUCCESS;
}
