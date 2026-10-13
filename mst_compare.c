#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define V 50
#define SPARSE_E 100
#define DENSE_E 600
#define MAX_E (V * (V - 1) / 2)

typedef struct {
    int u, v, weight;
} Edge;

typedef struct Node {
    int vertex;
    int weight;
    struct Node *next;
} Node;

typedef struct {
    Node *head[V];
} AdjList;

typedef struct {
    int weight[V][V];  /* 0 means no edge; all real weights are positive */
} AdjMatrix;

typedef struct {
    int parent[V];
    int rank[V];
} UnionFind;

typedef struct {
    Edge edges[V - 1];
    int count;
    long long total_weight;
} MST;

static int edge_exists(const Edge edges[], int count, int u, int v) {
    for (int i = 0; i < count; ++i) {
        if ((edges[i].u == u && edges[i].v == v) ||
            (edges[i].u == v && edges[i].v == u)) {
            return 1;
        }
    }
    return 0;
}

static void add_edge(Edge edges[], int *count, int u, int v, int weight) {
    if (u > v) {
        int temp = u;
        u = v;
        v = temp;
    }
    edges[*count] = (Edge){u, v, weight};
    ++(*count);
}

/* Create a spanning tree first, so the graph is guaranteed to be connected. */
static void generate_graph(Edge edges[], int target_edges) {
    int count = 0;

    for (int v = 1; v < V; ++v) {
        add_edge(edges, &count, rand() % v, v, 1 + rand() % 100);
    }

    while (count < target_edges) {
        int u = rand() % V;
        int v = rand() % V;
        if (u == v || edge_exists(edges, count, u, v)) {
            continue;
        }
        add_edge(edges, &count, u, v, 1 + rand() % 100);
    }
}

static void build_adj_list(AdjList *graph, const Edge edges[], int edge_count) {
    for (int i = 0; i < V; ++i) {
        graph->head[i] = NULL;
    }

    for (int i = 0; i < edge_count; ++i) {
        Node *uv = (Node *)malloc(sizeof(*uv));
        Node *vu = (Node *)malloc(sizeof(*vu));
        if (uv == NULL || vu == NULL) {
            free(uv);
            free(vu);
            fprintf(stderr, "Error: memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        *uv = (Node){edges[i].v, edges[i].weight, graph->head[edges[i].u]};
        graph->head[edges[i].u] = uv;
        *vu = (Node){edges[i].u, edges[i].weight, graph->head[edges[i].v]};
        graph->head[edges[i].v] = vu;
    }
}

static void free_adj_list(AdjList *graph) {
    for (int i = 0; i < V; ++i) {
        Node *current = graph->head[i];
        while (current != NULL) {
            Node *next = current->next;
            free(current);
            current = next;
        }
        graph->head[i] = NULL;
    }
}

static void build_adj_matrix(AdjMatrix *graph, const Edge edges[], int edge_count) {
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            graph->weight[i][j] = 0;
        }
    }

    for (int i = 0; i < edge_count; ++i) {
        int u = edges[i].u;
        int v = edges[i].v;
        graph->weight[u][v] = edges[i].weight;
        graph->weight[v][u] = edges[i].weight;
    }
}

static int compare_edges(const void *left, const void *right) {
    const Edge *a = (const Edge *)left;
    const Edge *b = (const Edge *)right;
    if (a->weight != b->weight) {
        return (a->weight > b->weight) - (a->weight < b->weight);
    }
    if (a->u != b->u) {
        return (a->u > b->u) - (a->u < b->u);
    }
    return (a->v > b->v) - (a->v < b->v);
}

static void union_find_init(UnionFind *uf) {
    for (int i = 0; i < V; ++i) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
}

static int find_root(UnionFind *uf, int x) {
    if (uf->parent[x] != x) {
        uf->parent[x] = find_root(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

static int union_sets(UnionFind *uf, int x, int y) {
    x = find_root(uf, x);
    y = find_root(uf, y);
    if (x == y) {
        return 0;
    }

    if (uf->rank[x] < uf->rank[y]) {
        uf->parent[x] = y;
    } else if (uf->rank[x] > uf->rank[y]) {
        uf->parent[y] = x;
    } else {
        uf->parent[y] = x;
        ++uf->rank[x];
    }
    return 1;
}

/* Extract each undirected edge once from the adjacency list, then use Kruskal. */
static MST kruskal(const AdjList *graph) {
    int edge_count = 0;
    for (int u = 0; u < V; ++u) {
        for (Node *node = graph->head[u]; node != NULL; node = node->next) {
            if (u < node->vertex) {
                ++edge_count;
            }
        }
    }

    Edge *edges = (Edge *)malloc((size_t)edge_count * sizeof(*edges));
    if (edges == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int u = 0; u < V; ++u) {
        for (Node *node = graph->head[u]; node != NULL; node = node->next) {
            if (u < node->vertex) {
                edges[index++] = (Edge){u, node->vertex, node->weight};
            }
        }
    }

    qsort(edges, (size_t)edge_count, sizeof(*edges), compare_edges);

    UnionFind uf;
    union_find_init(&uf);
    MST mst = {.count = 0, .total_weight = 0};
    for (int i = 0; i < edge_count && mst.count < V - 1; ++i) {
        if (union_sets(&uf, edges[i].u, edges[i].v)) {
            mst.edges[mst.count++] = edges[i];
            mst.total_weight += edges[i].weight;
        }
    }

    free(edges);
    return mst;
}

static MST prim(const AdjMatrix *graph) {
    int visited[V] = {0};
    int key[V];
    int parent[V];
    MST mst = {.count = 0, .total_weight = 0};

    for (int i = 0; i < V; ++i) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }
    key[0] = 0;

    for (int step = 0; step < V; ++step) {
        int u = -1;
        int smallest = INT_MAX;
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && key[v] < smallest) {
                smallest = key[v];
                u = v;
            }
        }
        if (u == -1) {
            break;
        }

        visited[u] = 1;
        if (parent[u] != -1) {
            int w = graph->weight[parent[u]][u];
            mst.edges[mst.count++] = (Edge){parent[u], u, w};
            mst.total_weight += w;
        }

        for (int v = 0; v < V; ++v) {
            int w = graph->weight[u][v];
            if (!visited[v] && w > 0 && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }
        }
    }
    return mst;
}

static double clock_ms(void) {
    return 1000.0 * (double)clock() / (double)CLOCKS_PER_SEC;
}

/* Estimated peak bytes for the graph representation and MST working data. */
static size_t kruskal_memory_bytes(int edge_count) {
    return sizeof(AdjList)
        + (size_t)(2 * edge_count) * sizeof(Node)
        + (size_t)edge_count * sizeof(Edge) /* edge array extracted for sorting */
        + sizeof(UnionFind)
        + sizeof(MST);
}

static size_t prim_memory_bytes(void) {
    return sizeof(AdjMatrix)
        + (size_t)3 * V * sizeof(int) /* visited, key, parent */
        + sizeof(MST);
}

static void print_result(const char *title, int edge_count, const MST *mst,
                         size_t memory_bytes, double elapsed_ms) {
    printf("\n%s\n", title);
    printf("Vertices       : %d\n", V);
    printf("Edges          : %d\n", edge_count);
    printf("MST Edges      : %d\n", mst->count);
    printf("MST Cost       : %lld\n", mst->total_weight);
    printf("Memory         : %zu Bytes (estimated)\n", memory_bytes);
    printf("Execution Time : %.6f ms\n", elapsed_ms);
    printf("MST edge list:\n");
    for (int i = 0; i < mst->count; ++i) {
        printf("  (%d, %d) weight=%d\n",
               mst->edges[i].u, mst->edges[i].v, mst->edges[i].weight);
    }
    if (mst->count != V - 1) {
        printf("WARNING: expected %d MST edges, but got %d.\n", V - 1, mst->count);
    }
}

static MST run_kruskal_case(const char *title, const Edge edges[], int edge_count) {
    AdjList graph;
    build_adj_list(&graph, edges, edge_count); /* excluded from algorithm time */

    double start = clock_ms();
    MST mst = kruskal(&graph);
    double elapsed = clock_ms() - start;

    print_result(title, edge_count, &mst, kruskal_memory_bytes(edge_count), elapsed);
    free_adj_list(&graph);
    return mst;
}

static MST run_prim_case(const char *title, const Edge edges[], int edge_count) {
    AdjMatrix graph;
    build_adj_matrix(&graph, edges, edge_count); /* excluded from algorithm time */

    double start = clock_ms();
    MST mst = prim(&graph);
    double elapsed = clock_ms() - start;

    print_result(title, edge_count, &mst, prim_memory_bytes(), elapsed);
    return mst;
}

int main(int argc, char *argv[]) {
    unsigned int seed = (argc > 1)
        ? (unsigned int)strtoul(argv[1], NULL, 10)
        : (unsigned int)time(NULL);
    srand(seed);

    Edge sparse_edges[SPARSE_E];
    Edge dense_edges[DENSE_E];
    generate_graph(sparse_edges, SPARSE_E);
    generate_graph(dense_edges, DENSE_E);

    printf("Random seed: %u\n", seed);
    printf("Weights are positive integers from 1 to 100.\n");
    printf("Each graph is connected and has no self-loops or duplicate edges.\n");

    MST sparse_kruskal = run_kruskal_case(
        "Case 1: Sparse Graph - Adjacency List - Kruskal",
        sparse_edges, SPARSE_E);
    MST dense_kruskal = run_kruskal_case(
        "Case 2: Dense Graph - Adjacency List - Kruskal",
        dense_edges, DENSE_E);
    MST sparse_prim = run_prim_case(
        "Case 3: Sparse Graph - Adjacency Matrix - Prim",
        sparse_edges, SPARSE_E);
    MST dense_prim = run_prim_case(
        "Case 4: Dense Graph - Adjacency Matrix - Prim",
        dense_edges, DENSE_E);

    printf("\nMST cost verification:\n");
    printf("Sparse graph: Kruskal=%lld, Prim=%lld -> %s\n",
           sparse_kruskal.total_weight, sparse_prim.total_weight,
           sparse_kruskal.total_weight == sparse_prim.total_weight ? "PASS" : "FAIL");
    printf("Dense graph : Kruskal=%lld, Prim=%lld -> %s\n",
           dense_kruskal.total_weight, dense_prim.total_weight,
           dense_kruskal.total_weight == dense_prim.total_weight ? "PASS" : "FAIL");

    if (sparse_kruskal.count != V - 1 || sparse_prim.count != V - 1 ||
        dense_kruskal.count != V - 1 || dense_prim.count != V - 1 ||
        sparse_kruskal.total_weight != sparse_prim.total_weight ||
        dense_kruskal.total_weight != dense_prim.total_weight) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
