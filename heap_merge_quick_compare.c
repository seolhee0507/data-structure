    int A = a[x], B = a[y], C = a[z];
    s->comparisons++;
    if (A < B) {
        s->comparisons++;
        if (B < C) return y;
        s->comparisons++;
        return (A < C) ? z : x;
    }
    s->comparisons++;
    if (A < C) return x;
    s->comparisons++;
    return (B < C) ? z : y;
}

static int partition(int *a, int low, int high, Stats *s) {
    int pivot = a[high], i = low - 1;
    for (int j = low; j < high; j++) {
        s->comparisons++;
        if (a[j] <= pivot) {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[high]);
    return i + 1;
}

static void quick_rec(int *a, int low, int high, Pivot type,
                      int depth, Stats *s) {
    if (depth > s->max_depth) s->max_depth = depth;
    if (low >= high) return;

    int pivot_index;
    if (type == FIRST) pivot_index = low;
    else if (type == RANDOM) pivot_index = low + rand() % (high - low + 1);
    else pivot_index = median3(a, low, low + (high - low) / 2, high, s);

    swap(&a[pivot_index], &a[high]);
    int p = partition(a, low, high, s);
    quick_rec(a, low, p - 1, type, depth + 1, s);
    quick_rec(a, p + 1, high, type, depth + 1, s);
}

static void quick_sort(int *a, int n, Pivot p, Stats *s) {
    quick_rec(a, 0, n - 1, p, 1, s);
}

static size_t quick_bytes(const Stats *s) {
    return (size_t)s->max_depth * sizeof(QuickFrame);
}

/* ---------------- Output / Main ---------------- */
static void print_result(const char *name, const Stats *s,
                         double ms, size_t memory, int ok) {
    printf("%-16s %-15llu %-12.3f %-16zu %s\n",
           name, s->comparisons, ms, memory, ok ? "OK" : "FAIL");
}

int main(void) {
    srand((unsigned)time(NULL));

    for (int x = 0; x < SIZES; x++) {
        int n = sizes[x];
        int *src = random_data(n);
        int *work = (int *)malloc((size_t)n * sizeof(int));
        if (!work) { perror("work"); free(src); return EXIT_FAILURE; }

        printf("\nData Size : %d\n", n);
        printf("%-16s %-15s %-12s %-16s %s\n",
               "Algorithm", "Comparisons", "Time(ms)", "Memory(Bytes)", "Check");
        puts("--------------------------------------------------------------------------");

        /* Heap Sort */
        Stats h = {0, 0};
        memcpy(work, src, (size_t)n * sizeof(int));
        double t = millis();
        heap_sort(work, n, &h);
        double elapsed = millis() - t;
        print_result("Heap Sort", &h, elapsed, heap_bytes(), is_sorted(work, n));

        /* Merge Sort */
        Stats m = {0, 0};
        memcpy(work, src, (size_t)n * sizeof(int));
        t = millis();
        merge_sort(work, n, &m);
        elapsed = millis() - t;
        print_result("Merge Sort", &m, elapsed, merge_bytes(n, &m), is_sorted(work, n));

        /* Quick Sort: three pivot strategies */
        Pivot pivots[3] = {FIRST, RANDOM, MEDIAN3};
        const char *names[3] = {"Quick (First)", "Quick (Random)", "Quick (Median3)"};

        for (int k = 0; k < 3; k++) {
            Stats q = {0, 0};
            memcpy(work, src, (size_t)n * sizeof(int));
            t = millis();
            quick_sort(work, n, pivots[k], &q);
            elapsed = millis() - t;
            print_result(names[k], &q, elapsed, quick_bytes(&q), is_sorted(work, n));
        }

        free(work);
        free(src);
    }
    return 0;
}
