#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define DATA_COUNT 10000
#define NUM_TRIALS 100

// 데이터 구조체 정의 (Key1, Key2, 원래 입력 순서)
typedef struct {
    int key1;
    int key2;
    int original_index;
} DataItem;

// 성능 측정용 구조체
typedef struct {
    long long comparisons;
    long long movements;
} PerfStats;

void reset_stats(PerfStats *stats) {
    stats->comparisons = 0;
    stats->movements = 0;
}

// Key1 비교 함수 (비교 횟수 카운트)
int compare_keys(const DataItem *a, const DataItem *b, PerfStats *stats) {
    stats->comparisons++;
    return a->key1 - b->key1;
}

// 1. 선택 정렬 (Selection Sort)
void selection_sort(DataItem *arr, int n, PerfStats *stats) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (compare_keys(&arr[j], &arr[min_idx], stats) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            DataItem temp = arr[i];
            stats->movements++;
            arr[i] = arr[min_idx];
            stats->movements++;
            arr[min_idx] = temp;
            stats->movements++;
        }
    }
}

// 2. 일반 버블 정렬 (Bubble Sort)
void bubble_sort(DataItem *arr, int n, PerfStats *stats) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (compare_keys(&arr[j], &arr[j + 1], stats) > 0) {
                DataItem temp = arr[j];
                stats->movements++;
                arr[j] = arr[j + 1];
                stats->movements++;
                arr[j + 1] = temp;
                stats->movements++;
            }
        }
    }
}

// 3. 조기 종료 버블 정렬 (Bubble Sort with Early Termination)
void bubble_sort_early(DataItem *arr, int n, PerfStats *stats) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (compare_keys(&arr[j], &arr[j + 1], stats) > 0) {
                DataItem temp = arr[j];
                stats->movements++;
                arr[j] = arr[j + 1];
                stats->movements++;
                arr[j + 1] = temp;
                stats->movements++;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 4. 삽입 정렬 (Insertion Sort) - use_ge: false는 '>', true는 '>='
void insertion_sort(DataItem *arr, int n, PerfStats *stats, bool use_ge) {
    for (int i = 1; i < n; i++) {
        DataItem current = arr[i];
        stats->movements++;
        int j = i - 1;
        while (j >= 0) {
            stats->comparisons++;
            bool cond = use_ge ? (arr[j].key1 >= current.key1) : (arr[j].key1 > current.key1);
            if (cond) {
                arr[j + 1] = arr[j];
                stats->movements++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = current;
        stats->movements++;
    }
}

// 데이터 생성
void generate_random_data(DataItem *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i].key1 = rand() % 1000001;
        arr[i].key2 = rand() % 1000001;
        arr[i].original_index = i;
    }
}

// 오름차순 검증
bool is_sorted(const DataItem *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i].key1 > arr[i + 1].key1) return false;
    }
    return true;
}

// 안정성 검증 함수
const char* verify_stability(const DataItem *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i].key1 == arr[i + 1].key1) {
            if (arr[i].original_index > arr[i + 1].original_index) {
                return "Unstable";
            }
        }
    }
    return "Stable";
}

// 정렬을 위한 qsort 비교 함수
int compare_data_items(const void *a, const void *b) {
    return ((DataItem*)a)->key1 - ((DataItem*)b)->key1;
}

int main() {
    srand((unsigned int)time(NULL));

    // 메모리 할당
    DataItem *base_data = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    DataItem *work_data = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);

    // ----------------------------------------------------
    // [1] 무작위 데이터 100회 반복 실험
    // ----------------------------------------------------
    long long sel_tot_c = 0, sel_tot_m = 0;
    long long bub_tot_c = 0, bub_tot_m = 0;
    long long bube_tot_c = 0, bube_tot_m = 0;
    long long ins_tot_c = 0, ins_tot_m = 0;

    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        generate_random_data(base_data, DATA_COUNT);
        PerfStats stats;

        // Selection
        memcpy(work_data, base_data, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        selection_sort(work_data, DATA_COUNT, &stats);
        sel_tot_c += stats.comparisons; sel_tot_m += stats.movements;

        // Bubble
        memcpy(work_data, base_data, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        bubble_sort(work_data, DATA_COUNT, &stats);
        bub_tot_c += stats.comparisons; bub_tot_m += stats.movements;

        // Bubble Early Termination
        memcpy(work_data, base_data, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        bubble_sort_early(work_data, DATA_COUNT, &stats);
        bube_tot_c += stats.comparisons; bube_tot_m += stats.movements;

        // Insertion (>)
        memcpy(work_data, base_data, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        insertion_sort(work_data, DATA_COUNT, &stats, false);
        ins_tot_c += stats.comparisons; ins_tot_m += stats.movements;
    }

    printf("Data Count : %d\n\n", DATA_COUNT);
    printf("Random Data\n");
    printf("Number of Trials : %d\n\n", NUM_TRIALS);

    printf("Selection Sort\n");
    printf("Total Comparisons   : %lld\n", sel_tot_c);
    printf("Average Comparisons : %lld\n", sel_tot_c / NUM_TRIALS);
    printf("Average Movements   : %lld\n\n", sel_tot_m / NUM_TRIALS);

    printf("Bubble Sort\n");
    printf("Total Comparisons   : %lld\n", bub_tot_c);
    printf("Average Comparisons : %lld\n", bub_tot_c / NUM_TRIALS);
    printf("Average Movements   : %lld\n\n", bub_tot_m / NUM_TRIALS);

    printf("Bubble Sort (Early Termination)\n");
    printf("Total Comparisons   : %lld\n", bube_tot_c);
    printf("Average Comparisons : %lld\n", bube_tot_c / NUM_TRIALS);
    printf("Average Movements   : %lld\n\n", bube_tot_m / NUM_TRIALS);

    printf("Insertion Sort\n");
    printf("Total Comparisons   : %lld\n", ins_tot_c);
    printf("Average Comparisons : %lld\n", ins_tot_c / NUM_TRIALS);
    printf("Average Movements   : %lld\n\n", ins_tot_m / NUM_TRIALS);

    printf("------------------------------------\n\n");

    // ----------------------------------------------------
    // [2] 상태별 테스트 데이터 세트 생성 (Sorted, Reverse, Partial)
    // ----------------------------------------------------
    DataItem *ref_data = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    generate_random_data(ref_data, DATA_COUNT);
    qsort(ref_data, DATA_COUNT, sizeof(DataItem), compare_data_items); // 기준 정렬 데이터

    // 1. Sorted Data
    DataItem *sorted_data = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    memcpy(sorted_data, ref_data, sizeof(DataItem) * DATA_COUNT);

    // 2. Reverse-Sorted Data
    DataItem *rev_data = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    for (int i = 0; i < DATA_COUNT; i++) {
        rev_data[i] = ref_data[DATA_COUNT - 1 - i];
    }

    // 3. Partially Sorted Data (정렬된 데이터의 5% 원소 상호 교환)
    DataItem *part_data = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    memcpy(part_data, ref_data, sizeof(DataItem) * DATA_COUNT);
    int swaps = (int)(DATA_COUNT * 0.05);
    for (int i = 0; i < swaps; i++) {
        int idx1 = rand() % DATA_COUNT;
        int idx2 = rand() % DATA_COUNT;
        DataItem temp = part_data[idx1];
        part_data[idx1] = part_data[idx2];
        part_data[idx2] = temp;
    }

    // 상태별 테스트 출력 루프
    struct {
        const char *name;
        DataItem *dataset;
    } states[] = {
        {"Sorted Data", sorted_data},
        {"Reverse-Sorted Data", rev_data},
        {"Partially Sorted Data", part_data}
    };

    for (int s = 0; s < 3; s++) {
        printf("%s\n\n", states[s].name);
        PerfStats stats;

        // Selection
        memcpy(work_data, states[s].dataset, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        selection_sort(work_data, DATA_COUNT, &stats);
        printf("Selection Sort\nComparisons : %lld\nMovements   : %lld\n\n", stats.comparisons, stats.movements);

        // Bubble
        memcpy(work_data, states[s].dataset, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        bubble_sort(work_data, DATA_COUNT, &stats);
        printf("Bubble Sort\nComparisons : %lld\nMovements   : %lld\n\n", stats.comparisons, stats.movements);

        // Bubble Early
        memcpy(work_data, states[s].dataset, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        bubble_sort_early(work_data, DATA_COUNT, &stats);
        printf("Bubble Sort (Early Termination)\nComparisons : %lld\nMovements   : %lld\n\n", stats.comparisons, stats.movements);

        // Insertion
        memcpy(work_data, states[s].dataset, sizeof(DataItem) * DATA_COUNT);
        reset_stats(&stats);
        insertion_sort(work_data, DATA_COUNT, &stats, false);
        printf("Insertion Sort\nComparisons : %lld\nMovements   : %lld\n\n", stats.comparisons, stats.movements);

        printf("------------------------------------\n\n");
    }

    // ----------------------------------------------------
    // [3] 안정성 검증 (Stability Verification)
    // ----------------------------------------------------
    DataItem stab_test[] = {
        {527, 100, 0},
        {527, 200, 1},
        {125, 300, 2},
        {892, 400, 3},
        {527, 50,  4},
        {125, 10,  5}
    };
    int stab_n = sizeof(stab_test) / sizeof(stab_test[0]);
    DataItem stab_work[6];
    PerfStats dummy_stats;

    printf("Stability Verification\n\n");

    // Selection
    memcpy(stab_work, stab_test, sizeof(stab_test));
    reset_stats(&dummy_stats);
    selection_sort(stab_work, stab_n, &dummy_stats);
    printf("Selection Sort       : %s\n", verify_stability(stab_work, stab_n));

    // Bubble
    memcpy(stab_work, stab_test, sizeof(stab_test));
    reset_stats(&dummy_stats);
    bubble_sort(stab_work, stab_n, &dummy_stats);
    printf("Bubble Sort          : %s\n", verify_stability(stab_work, stab_n));

    // Insertion (>)
    memcpy(stab_work, stab_test, sizeof(stab_test));
    reset_stats(&dummy_stats);
    insertion_sort(stab_work, stab_n, &dummy_stats, false);
    printf("Insertion Sort (>)   : %s\n", verify_stability(stab_work, stab_n));

    // Insertion (>=)
    memcpy(stab_work, stab_test, sizeof(stab_test));
    reset_stats(&dummy_stats);
    insertion_sort(stab_work, stab_n, &dummy_stats, true);
    printf("Insertion Sort (>=)  : %s\n", verify_stability(stab_work, stab_n));

    // 메모리 해제
    free(base_data);
    free(work_data);
    free(ref_data);
    free(sorted_data);
    free(rev_data);
    free(part_data);

    return 0;
}
