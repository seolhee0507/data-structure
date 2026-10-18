
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 10
#define E 20
#define INF 1000000000

typedef struct {
    int u;
    int v;
    int weight;
} Edge;

int main(void) {
    Edge edges[E];
    int used[V][V] = {0};

    int dist[V][V];
    int next[V][V];

    srand((unsigned int)time(NULL));

    /* 1. 연결 그래프를 만들기 위해 신장 트리 생성 */
    int edgeCount = 0;

    for (int i = 1; i < V; i++) {
        int parent = rand() % i;
        int weight = rand() % 100 + 1;

        edges[edgeCount].u = parent;
        edges[edgeCount].v = i;
        edges[edgeCount].weight = weight;
        edgeCount++;

        used[parent][i] = 1;
        used[i][parent] = 1;
    }

    /* 2. 중복 없는 간선을 추가하여 총 20개 생성 */
    while (edgeCount < E) {
        int u = rand() % V;
        int v = rand() % V;

        /* 자기 자신 연결 및 중복 간선 방지 */
        if (u == v || used[u][v]) {
            continue;
        }

        int weight = rand() % 100 + 1;

        edges[edgeCount].u = u;
        edges[edgeCount].v = v;
        edges[edgeCount].weight = weight;
        edgeCount++;

        used[u][v] = 1;
        used[v][u] = 1;
    }

    /* 3. 생성된 그래프 출력 */
    printf("Graph Vertices : %d\n", V);
    printf("Graph Edges    : %d\n\n", edgeCount);

    printf("%-15s %s\n", "Edge", "Weight");
    printf("-------------------------\n");

    for (int i = 0; i < E; i++) {
        printf("(%d, %d)          %d\n",
               edges[i].u, edges[i].v, edges[i].weight);
    }

    /* 4. 거리 배열과 다음 정점 배열 초기화 */
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                dist[i][j] = 0;
                next[i][j] = i;
            } else {
                dist[i][j] = INF;
                next[i][j] = -1;
            }
        }
    }

    /* 무방향 그래프이므로 양쪽 방향에 간선 저장 */
    for (int i = 0; i < E; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        dist[u][v] = w;
        dist[v][u] = w;

        next[u][v] = v;
        next[v][u] = u;
    }

    /* 5. Floyd-Warshall 알고리즘 */
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF &&
                    dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {

                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    /* 6. 모든 정점 쌍의 최단경로 출력 */
    printf("\n========== Shortest Paths ==========\n\n");

    int totalPairs = 0;

    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            printf("%d -> %d\n", u, v);
            printf("Path     : ");

            int current = u;
            printf("%d", current);

            while (current != v) {
                current = next[current][v];

                if (current == -1) {
                    printf(" (No path)");
                    break;
                }

                printf(" -> %d", current);
            }

            printf("\n");
            printf("Distance : %d\n\n", dist[u][v]);

            totalPairs++;
        }
    }

    printf("Total vertex pairs : %d\n", totalPairs);

    return 0;
}
