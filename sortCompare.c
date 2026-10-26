#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define DATA_COUNT 10000
#define NUM_TRIALS 100

// 데이터 구조체 정의
typedef struct {
    int key1;
    int key2;
    int original_index;
} DataItem;

// 연산 횟수 측정 구조체
typedef struct {
    long long comparisons;
    long long movements;
} Metrics;

// 데이터 복사 함수
void copy_data(DataItem* dest, const DataItem* src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// [1] 선택 정렬 (Selection Sort)
void selection_sort(DataItem* data, int n, Metrics* m) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            m->comparisons++;
            if (data[j].key1 < data[min_idx].key1) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            DataItem temp = data[i];
            m->movements += 3;
            data[i] = data[min_idx];
            data[min_idx] = temp;
        }
    }
}

// [2] 일반 버블 정렬 (Bubble Sort)
void bubble_sort(DataItem* data, int n, Metrics* m) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            m->comparisons++;
            if (data[j].key1 > data[j + 1].key1) {
                DataItem temp = data[j];
                m->movements += 3;
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

// [3] 조기 종료 버블 정렬 (Bubble Sort with Early Termination)
void bubble_sort_early(DataItem* data, int n, Metrics* m) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            m->comparisons++;
            if (data[j].key1 > data[j + 1].key1) {
                DataItem temp = data[j];
                m->movements += 3;
                data[j] = data[j + 1];
                data[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

// [4] 삽입 정렬 - Case A (>) : Stable
void insertion_sort_gt(DataItem* data, int n, Metrics* m) {
    for (int i = 1; i < n; i++) {
        DataItem current = data[i];
        m->movements++; // temp 저장
        int j = i - 1;
        while (j >= 0) {
            m->comparisons++;
            if (data[j].key1 > current.key1) {
                data[j + 1] = data[j];
                m->movements++;
                j--;
            } else {
                break;
            }
        }
        data[j + 1] = current;
        m->movements++;
    }
}

// 삽입 정렬 - Case B (>=) : Unstable
void insertion_sort_gte(DataItem* data, int n, Metrics* m) {
    for (int i = 1; i < n; i++) {
        DataItem current = data[i];
        m->movements++;
        int j = i - 1;
        while (j >= 0) {
            m->comparisons++;
            if (data[j].key1 >= current.key1) {
                data[j + 1] = data[j];
                m->movements++;
                j--;
            } else {
                break;
            }
        }
        data[j + 1] = current;
        m->movements++;
    }
}

// 정렬 상태 확인
bool check_sorted(const DataItem* data, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (data[i].key1 > data[i + 1].key1) {
            return false;
        }
    }
    return true;
}

// 안정성 검증 함수
const char* verify_stability(const DataItem* data, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (data[i].key1 == data[i + 1].key1) {
            if (data[i].original_index > data[i + 1].original_index) {
                return "Unstable";
            }
        }
    }
    return "Stable";
}

// 무작위 데이터 생성
void generate_random_data(DataItem* data, int n) {
    for (int i = 0; i < n; i++) {
        data[i].key1 = rand() % 1000001;
        data[i].key2 = rand() % 1000001;
        data[i].original_index = i;
    }
}

// 일부 정렬된 데이터 생성 (1% 교란)
void generate_partially_sorted_data(DataItem* data, int n, int swap_count) {
    for (int i = 0; i < n; i++) {
        data[i].key1 = i;
        data[i].key2 = rand() % 1000001;
        data[i].original_index = i;
    }
    for (int i = 0; i < swap_count; i++) {
        int idx1 = rand() % n;
        int idx2 = rand() % n;
        DataItem temp = data[idx1];
        data[idx1] = data[idx2];
        data[idx2] = temp;
    }
}

int main() {
    srand((unsigned int)time(NULL));

    DataItem* raw_data = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    DataItem* d_sel = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    DataItem* d_bub = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    DataItem* d_bub_e = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    DataItem* d_ins = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);

    printf("Data Count : %d\n", DATA_COUNT);
    printf("Random Data\nNumber of Trials : %d\n\n", NUM_TRIALS);

    long long sel_tot_c = 0, sel_tot_m = 0;
    long long bub_tot_c = 0, bub_tot_m = 0;
    long long bub_e_tot_c = 0, bub_e_tot_m = 0;
    long long ins_tot_c = 0, ins_tot_m = 0;

    for (int t = 0; t < NUM_TRIALS; t++) {
        generate_random_data(raw_data, DATA_COUNT);

        copy_data(d_sel, raw_data, DATA_COUNT);
        copy_data(d_bub, raw_data, DATA_COUNT);
        copy_data(d_bub_e, raw_data, DATA_COUNT);
        copy_data(d_ins, raw_data, DATA_COUNT);

        Metrics m_sel = {0, 0}, m_bub = {0, 0}, m_bub_e = {0, 0}, m_ins = {0, 0};

        selection_sort(d_sel, DATA_COUNT, &m_sel);
        bubble_sort(d_bub, DATA_COUNT, &m_bub);
        bubble_sort_early(d_bub_e, DATA_COUNT, &m_bub_e);
        insertion_sort_gt(d_ins, DATA_COUNT, &m_ins);

        sel_tot_c += m_sel.comparisons; sel_tot_m += m_sel.movements;
        bub_tot_c += m_bub.comparisons; bub_tot_m += m_bub.movements;
        bub_e_tot_c += m_bub_e.comparisons; bub_e_tot_m += m_bub_e.movements;
        ins_tot_c += m_ins.comparisons; ins_tot_m += m_ins.movements;
    }

    printf("Selection Sort\n");
    printf("Total Comparisons   : %lld\n", sel_tot_c);
    printf("Average Comparisons : %.2f\n", (double)sel_tot_c / NUM_TRIALS);
    printf("Average Movements   : %.2f\n\n", (double)sel_tot_m / NUM_TRIALS);

    printf("Bubble Sort\n");
    printf("Total Comparisons   : %lld\n", bub_tot_c);
    printf("Average Comparisons : %.2f\n", (double)bub_tot_c / NUM_TRIALS);
    printf("Average Movements   : %.2f\n\n", (double)bub_tot_m / NUM_TRIALS);

    printf("Bubble Sort (Early Termination)\n");
    printf("Total Comparisons   : %lld\n", bub_e_tot_c);
    printf("Average Comparisons : %.2f\n", (double)bub_e_tot_c / NUM_TRIALS);
    printf("Average Movements   : %.2f\n\n", (double)bub_e_tot_m / NUM_TRIALS);

    printf("Insertion Sort\n");
    printf("Total Comparisons   : %lld\n", ins_tot_c);
    printf("Average Comparisons : %.2f\n", (double)ins_tot_c / NUM_TRIALS);
    printf("Average Movements   : %.2f\n\n", (double)ins_tot_m / NUM_TRIALS);

    printf("------------------------------------\n");

    // 상태별 실험 (Sorted, Reverse-Sorted, Partially Sorted)
    DataItem* base_sorted = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    DataItem* base_reverse = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);
    DataItem* base_partial = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);

    for (int i = 0; i < DATA_COUNT; i++) {
        base_sorted[i].key1 = i;
        base_sorted[i].key2 = rand() % 1000001;
        base_sorted[i].original_index = i;

        base_reverse[i].key1 = DATA_COUNT - 1 - i;
        base_reverse[i].key2 = rand() % 1000001;
        base_reverse[i].original_index = i;
    }
    generate_partially_sorted_data(base_partial, DATA_COUNT, 100);

    const char* state_names[] = {"Sorted Data", "Reverse-Sorted Data", "Partially Sorted Data"};
    DataItem* state_datasets[] = {base_sorted, base_reverse, base_partial};

    for (int s = 0; s < 3; s++) {
        printf("\n%s\n", state_names[s]);
        
        DataItem* temp_arr = (DataItem*)malloc(sizeof(DataItem) * DATA_COUNT);

        // Selection
        copy_data(temp_arr, state_datasets[s], DATA_COUNT);
        Metrics m1 = {0, 0};
        selection_sort(temp_arr, DATA_COUNT, &m1);
        printf("\nSelection Sort\nComparisons : %lld\nMovements   : %lld\n", m1.comparisons, m1.movements);

        // Bubble
        copy_data(temp_arr, state_datasets[s], DATA_COUNT);
        Metrics m2 = {0, 0};
        bubble_sort(temp_arr, DATA_COUNT, &m2);
        printf("\nBubble Sort\nComparisons : %lld\nMovements   : %lld\n", m2.comparisons, m2.movements);

        // Bubble Early
        copy_data(temp_arr, state_datasets[s], DATA_COUNT);
        Metrics m3 = {0, 0};
        bubble_sort_early(temp_arr, DATA_COUNT, &m3);
        printf("\nBubble Sort (Early Termination)\nComparisons : %lld\nMovements   : %lld\n", m3.comparisons, m3.movements);

        // Insertion
        copy_data(temp_arr, state_datasets[s], DATA_COUNT);
        Metrics m4 = {0, 0};
        insertion_sort_gt(temp_arr, DATA_COUNT, &m4);
        printf("\nInsertion Sort\nComparisons : %lld\nMovements   : %lld\n", m4.comparisons, m4.movements);

        free(temp_arr);
        printf("\n------------------------------------\n");
    }

    // 안정성 검증
    printf("\nStability Verification\n");
    DataItem* stab_raw = (DataItem*)malloc(sizeof(DataItem) * 1000);
    for (int i = 0; i < 1000; i++) {
        stab_raw[i].key1 = rand() % 5; // 중복 키 유도
        stab_raw[i].key2 = rand() % 100;
        stab_raw[i].original_index = i;
    }

    DataItem* s_sel = (DataItem*)malloc(sizeof(DataItem) * 1000);
    DataItem* s_bub = (DataItem*)malloc(sizeof(DataItem) * 1000);
    DataItem* s_ins_gt = (DataItem*)malloc(sizeof(DataItem) * 1000);
    DataItem* s_ins_gte = (DataItem*)malloc(sizeof(DataItem) * 1000);

    copy_data(s_sel, stab_raw, 1000);
    copy_data(s_bub, stab_raw, 1000);
    copy_data(s_ins_gt, stab_raw, 1000);
    copy_data(s_ins_gte, stab_raw, 1000);

    selection_sort(s_sel, 1000, &(Metrics){0,0});
    bubble_sort(s_bub, 1000, &(Metrics){0,0});
    insertion_sort_gt(s_ins_gt, 1000, &(Metrics){0,0});
    insertion_sort_gte(s_ins_gte, 1000, &(Metrics){0,0});

    printf("Selection Sort       : %s\n", verify_stability(s_sel, 1000));
    printf("Bubble Sort          : %s\n", verify_stability(s_bub, 1000));
    printf("Insertion Sort (>)   : %s\n", verify_stability(s_ins_gt, 1000));
    printf("Insertion Sort (>=)  : %s\n", verify_stability(s_ins_gte, 1000));

    // 메모리 해제
    free(raw_data); free(d_sel); free(d_bub); free(d_bub_e); free(d_ins);
    free(base_sorted); free(base_reverse); free(base_partial);
    free(stab_raw); free(s_sel); free(s_bub); free(s_ins_gt); free(s_ins_gte);

    return 0;
}
