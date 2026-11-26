#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 512
#define MAX_NAME_LEN 128
#define MAX_ID_LEN 32
#define MAX_GENDER_LEN 16

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char gender[MAX_GENDER_LEN];
    int korean;
    int english;
    int math;
    int total;
} Student;

typedef struct {
    Student *data;
    int count;
    int capacity;
} StudentArray;

static void init_array(StudentArray *arr, int initial_cap) {
    arr->data = (Student *)malloc(sizeof(Student) * (size_t)initial_cap);
    if (!arr->data) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    arr->count = 0;
    arr->capacity = initial_cap;
}

static void push_student(StudentArray *arr, Student s) {
    if (arr->count >= arr->capacity) {
        int new_cap = arr->capacity * 2;
        Student *grown = (Student *)realloc(arr->data, sizeof(Student) * (size_t)new_cap);
        if (!grown) {
            fprintf(stderr, "Memory reallocation failed.\n");
            exit(EXIT_FAILURE);
        }
        arr->data = grown;
        arr->capacity = new_cap;
    }
    arr->data[arr->count++] = s;
}

static void free_array(StudentArray *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

static void trim_crlf(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

/* CSV 한 줄 파싱: ID,NAME,GENDER,KOREAN_GRADE,ENGLISH_GRADE,MATH_GRADE */
static int parse_csv_line(char *line, Student *out) {
    trim_crlf(line);
    if (line[0] == '\0') return 0;

    char *fields[6];
    int idx = 0;
    char *cursor = line;

    while (idx < 6) {
        fields[idx++] = cursor;
        char *comma = strchr(cursor, ',');
        if (!comma) break;
        *comma = '\0';
        cursor = comma + 1;
    }

    if (idx < 6) return 0;

    snprintf(out->id, sizeof(out->id), "%s", fields[0]);
    snprintf(out->name, sizeof(out->name), "%s", fields[1]);
    snprintf(out->gender, sizeof(out->gender), "%s", fields[2]);
    out->korean = atoi(fields[3]);
    out->english = atoi(fields[4]);
    out->math = atoi(fields[5]);
    out->total = out->korean + out->english + out->math;
    return 1;
}

static int load_students_from_csv(const char *filename, StudentArray *arr) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open CSV file");
        return 0;
    }

    char line[MAX_LINE_LEN];
    int line_no = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_no++;
        if (line_no == 1) {
            /* 헤더 행 건너뜀 */
            continue;
        }
        Student s;
        if (parse_csv_line(line, &s)) {
            push_student(arr, s);
        }
    }

    fclose(fp);
    return 1;
}

/* [1] 순차 탐색: 원본 배열을 정렬하지 않고 처음부터 차례로 비교 */
static int sequential_search(const StudentArray *arr, const char *target_name,
                             StudentArray *matches, long long *comparisons) {
    long long local_cmp = 0;
    int found = 0;

    for (int i = 0; i < arr->count; i++) {
        local_cmp++;
        if (strcmp(arr->data[i].name, target_name) == 0) {
            if (matches) {
                push_student(matches, arr->data[i]);
            }
            found++;
        }
    }

    *comparisons += local_cmp;
    return found;
}

static void swap_students(Student *a, Student *b) {
    Student tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Median-of-Three 피벗 선택 */
static int median_of_three_pivot(Student *a, int lo, int hi, long long *cmp) {
    int mid = lo + (hi - lo) / 2;

    (*cmp)++;
    if (strcmp(a[mid].name, a[lo].name) < 0) {
        swap_students(&a[lo], &a[mid]);
    }

    (*cmp)++;
    if (strcmp(a[hi].name, a[lo].name) < 0) {
        swap_students(&a[lo], &a[hi]);
    }

    (*cmp)++;
    if (strcmp(a[hi].name, a[mid].name) < 0) {
        swap_students(&a[mid], &a[hi]);
    }

    return mid;
}

static void quicksort_rec(Student *a, int lo, int hi, long long *cmp) {
    if (lo >= hi) return;

    if (hi - lo == 1) {
        (*cmp)++;
        if (strcmp(a[hi].name, a[lo].name) < 0) {
            swap_students(&a[lo], &a[hi]);
        }
        return;
    }

    int pivot_idx = median_of_three_pivot(a, lo, hi, cmp);
    char pivot_name[MAX_NAME_LEN];
    snprintf(pivot_name, sizeof(pivot_name), "%s", a[pivot_idx].name);
    swap_students(&a[pivot_idx], &a[hi]);

    int store_idx = lo;
    for (int j = lo; j < hi; j++) {
        (*cmp)++;
        if (strcmp(a[j].name, pivot_name) < 0) {
            swap_students(&a[store_idx], &a[j]);
            store_idx++;
        }
    }
    swap_students(&a[store_idx], &a[hi]);

    quicksort_rec(a, lo, store_idx - 1, cmp);
    quicksort_rec(a, store_idx + 1, hi, cmp);
}

/* [2-a] 원본 복사본을 이름 기준 오름차순으로 퀵 정렬 */
static long long quicksort_copy(const StudentArray *src, StudentArray *sorted_out) {
    init_array(sorted_out, src->capacity > 0 ? src->capacity : 16);
    for (int i = 0; i < src->count; i++) {
        push_student(sorted_out, src->data[i]);
    }

    long long cmp = 0;
    if (sorted_out->count > 1) {
        quicksort_rec(sorted_out->data, 0, sorted_out->count - 1, &cmp);
    }
    return cmp;
}

/* [2-b] 정렬된 배열에서 이진 탐색 후 동명이인 좌·우 확장 탐색 */
static int binary_search_sorted(const StudentArray *sorted_arr, const char *target_name,
                                StudentArray *matches, long long *comparisons) {
    long long local_cmp = 0;
    int lo = 0;
    int hi = sorted_arr->count - 1;
    int found_idx = -1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp_res;

        local_cmp++;
        cmp_res = strcmp(sorted_arr->data[mid].name, target_name);
        if (cmp_res == 0) {
            found_idx = mid;
            break;
        } else if (cmp_res < 0) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    int count = 0;
    if (found_idx != -1) {
        if (matches) {
            push_student(matches, sorted_arr->data[found_idx]);
        }
        count++;

        int left = found_idx - 1;
        while (left >= 0) {
            local_cmp++;
            if (strcmp(sorted_arr->data[left].name, target_name) == 0) {
                if (matches) {
                    push_student(matches, sorted_arr->data[left]);
                }
                count++;
                left--;
            } else {
                break;
            }
        }

        int right = found_idx + 1;
        while (right < sorted_arr->count) {
            local_cmp++;
            if (strcmp(sorted_arr->data[right].name, target_name) == 0) {
                if (matches) {
                    push_student(matches, sorted_arr->data[right]);
                }
                count++;
                right++;
            } else {
                break;
            }
        }
    }

    *comparisons += local_cmp;
    return count;
}

static void print_student_result(const Student *s) {
    printf("Search Result\n");
    printf("Name    : %s\n", s->name);
    printf("Korean  : %d\n", s->korean);
    printf("English : %d\n", s->english);
    printf("Math    : %d\n", s->math);
    printf("Total   : %d\n\n", s->total);
}

int main(void) {
    const char *csv_file = "dataset_id_ascending (1).csv";
    StudentArray original;
    init_array(&original, 40000);

    if (!load_students_from_csv(csv_file, &original) || original.count == 0) {
        fprintf(stderr, "No student data loaded.\n");
        free_array(&original);
        return EXIT_FAILURE;
    }

    /* 단일 검색 출력 예시 (동명이인이 존재하는 이름 'Dakota') */
    StudentArray sample_matches;
    init_array(&sample_matches, 16);
    long long dummy_cmp = 0;
    sequential_search(&original, "Dakota", &sample_matches, &dummy_cmp);
    for (int i = 0; i < sample_matches.count; i++) {
        print_student_result(&sample_matches.data[i]);
    }
    free_array(&sample_matches);

    /* 최초 퀵 정렬 1회 수행 및 비교 횟수 측정 */
    StudentArray sorted_arr;
    long long qs_comparisons = quicksort_copy(&original, &sorted_arr);

    /* K 변화 실험 (누적 검색 목록 사용: 존재 이름 80%, 미존재 이름 20%) */
    const int k_values[] = {10, 50, 100, 200, 300, 500, 800, 1000, 1500, 2000};
    const int num_k = (int)(sizeof(k_values) / sizeof(k_values[0]));
    const int max_k = k_values[num_k - 1];

    char (*queries)[MAX_NAME_LEN] = malloc(sizeof(char[MAX_NAME_LEN]) * (size_t)max_k);
    if (!queries) {
        fprintf(stderr, "Failed to allocate queries buffer.\n");
        free_array(&sorted_arr);
        free_array(&original);
        return EXIT_FAILURE;
    }

    srand(42);
    for (int i = 0; i < max_k; i++) {
        if (i % 5 == 4) {
            snprintf(queries[i], MAX_NAME_LEN, "UnknownStudent_%d", i);
        } else {
            int rand_idx = rand() % original.count;
            snprintf(queries[i], MAX_NAME_LEN, "%s", original.data[rand_idx].name);
        }
    }

    printf("Comparison Results\n\n");
    printf("%-8s%-16s%s\n", "K", "Sequential", "Quick + Binary");
    printf("----------------------------------------\n");

    long long seq_total = 0;
    long long bin_total = 0;
    int current_idx = 0;

    for (int ki = 0; ki < num_k; ki++) {
        int target_k = k_values[ki];
        while (current_idx < target_k) {
            sequential_search(&original, queries[current_idx], NULL, &seq_total);
            binary_search_sorted(&sorted_arr, queries[current_idx], NULL, &bin_total);
            current_idx++;
        }
        long long quick_plus_binary_total = qs_comparisons + bin_total;
        printf("%-8d%-16lld%lld\n", target_k, seq_total, quick_plus_binary_total);
    }

    free(queries);
    free_array(&sorted_arr);
    free_array(&original);
    return EXIT_SUCCESS;
}
