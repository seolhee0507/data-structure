
실행값
Random seed: 42
Weights are positive integers from 1 to 100.
Each graph is connected and has no self-loops or duplicate edges.

Case 1: Sparse Graph - Adjacency List - Kruskal
Vertices       : 50
Edges          : 100
MST Edges      : 49
MST Cost       : 1257
Memory         : 5800 Bytes (estimated)
Execution Time : 0.014000 ms
MST edge list:
  (10, 18) weight=1
  (14, 18) weight=1
  (22, 25) weight=1
  (25, 27) weight=1
  (13, 31) weight=2
  (14, 42) weight=2
  (19, 39) weight=2
  (11, 41) weight=3
  (27, 28) weight=5
  (11, 48) weight=7
  (23, 24) weight=7
  (13, 21) weight=8
  (26, 38) weight=8
  (9, 12) weight=10
  (23, 36) weight=10
  (9, 48) weight=12
  (1, 3) weight=13
  (14, 44) weight=14
  (5, 17) weight=17
  (27, 49) weight=17
  (36, 40) weight=17
  (6, 7) weight=18
  (5, 36) weight=21
  (13, 35) weight=21
  (0, 4) weight=22
  (0, 20) weight=22
  (0, 46) weight=23
  (15, 37) weight=23
  (36, 44) weight=23
  (12, 19) weight=25
  (12, 35) weight=25
  (5, 13) weight=28
  (0, 23) weight=31
  (7, 27) weight=31
  (3, 16) weight=34
  (3, 5) weight=36
  (17, 33) weight=36
  (34, 43) weight=40
  (11, 15) weight=42
  (20, 45) weight=43
  (17, 30) weight=47
  (11, 22) weight=50
  (6, 34) weight=51
  (15, 26) weight=55
  (2, 22) weight=56
  (15, 47) weight=57
  (29, 41) weight=59
  (8, 30) weight=84
  (26, 32) weight=96

Case 2: Dense Graph - Adjacency List - Kruskal
Vertices       : 50
Edges          : 600
MST Edges      : 49
MST Cost       : 300
Memory         : 27800 Bytes (estimated)
Execution Time : 0.058000 ms
MST edge list:
  (8, 39) weight=1
  (8, 40) weight=1
  (8, 41) weight=1
  (9, 18) weight=1
  (26, 28) weight=1
  (41, 46) weight=1
  (4, 6) weight=2
  (7, 10) weight=2
  (42, 43) weight=2
  (1, 44) weight=3
  (3, 40) weight=3
  (24, 42) weight=3
  (37, 38) weight=3
  (0, 7) weight=4
  (1, 49) weight=4
  (2, 17) weight=4
  (4, 24) weight=4
  (14, 42) weight=4
  (31, 41) weight=4
  (4, 46) weight=5
  (15, 19) weight=5
  (34, 47) weight=5
  (43, 45) weight=5
  (0, 45) weight=6
  (8, 27) weight=6
  (10, 26) weight=6
  (13, 29) weight=6
  (22, 29) weight=6
  (24, 47) weight=6
  (7, 29) weight=7
  (8, 49) weight=7
  (11, 48) weight=7
  (18, 45) weight=7
  (25, 38) weight=7
  (15, 18) weight=8
  (23, 46) weight=8
  (30, 31) weight=8
  (0, 2) weight=9
  (15, 38) weight=9
  (36, 37) weight=9
  (1, 16) weight=10
  (13, 35) weight=10
  (37, 48) weight=10
  (11, 12) weight=11
  (14, 32) weight=11
  (33, 37) weight=12
  (3, 5) weight=13
  (19, 20) weight=15
  (12, 21) weight=18

Case 3: Sparse Graph - Adjacency Matrix - Prim
Vertices       : 50
Edges          : 100
MST Edges      : 49
MST Cost       : 1257
Memory         : 11200 Bytes (estimated)
Execution Time : 0.008000 ms
MST edge list:
  (0, 4) weight=22
  (0, 20) weight=22
  (0, 46) weight=23
  (0, 23) weight=31
  (23, 24) weight=7
  (23, 36) weight=10
  (36, 40) weight=17
  (36, 5) weight=21
  (5, 17) weight=17
  (36, 44) weight=23
  (44, 14) weight=14
  (14, 18) weight=1
  (18, 10) weight=1
  (14, 42) weight=2
  (5, 13) weight=28
  (13, 31) weight=2
  (13, 21) weight=8
  (13, 35) weight=21
  (35, 12) weight=25
  (12, 9) weight=10
  (9, 48) weight=12
  (48, 11) weight=7
  (11, 41) weight=3
  (12, 19) weight=25
  (19, 39) weight=2
  (5, 3) weight=36
  (3, 1) weight=13
  (3, 16) weight=34
  (17, 33) weight=36
  (11, 15) weight=42
  (15, 37) weight=23
  (20, 45) weight=43
  (17, 30) weight=47
  (11, 22) weight=50
  (22, 25) weight=1
  (25, 27) weight=1
  (27, 28) weight=5
  (27, 49) weight=17
  (27, 7) weight=31
  (7, 6) weight=18
  (6, 34) weight=51
  (34, 43) weight=40
  (15, 26) weight=55
  (26, 38) weight=8
  (22, 2) weight=56
  (15, 47) weight=57
  (41, 29) weight=59
  (30, 8) weight=84
  (26, 32) weight=96

Case 4: Dense Graph - Adjacency Matrix - Prim
Vertices       : 50
Edges          : 600
MST Edges      : 49
MST Cost       : 300
Memory         : 11200 Bytes (estimated)
Execution Time : 0.014000 ms
MST edge list:
  (0, 7) weight=4
  (7, 10) weight=2
  (10, 26) weight=6
  (26, 28) weight=1
  (0, 45) weight=6
  (45, 43) weight=5
  (43, 42) weight=2
  (42, 24) weight=3
  (24, 4) weight=4
  (4, 6) weight=2
  (42, 14) weight=4
  (4, 46) weight=5
  (46, 41) weight=1
  (41, 8) weight=1
  (8, 39) weight=1
  (8, 40) weight=1
  (40, 3) weight=3
  (41, 31) weight=4
  (8, 27) weight=6
  (24, 47) weight=6
  (47, 34) weight=5
  (45, 18) weight=7
  (18, 9) weight=1
  (7, 29) weight=7
  (29, 13) weight=6
  (29, 22) weight=6
  (14, 44) weight=7
  (44, 1) weight=3
  (1, 49) weight=4
  (18, 15) weight=8
  (15, 19) weight=5
  (46, 23) weight=8
  (31, 30) weight=8
  (0, 2) weight=9
  (2, 17) weight=4
  (42, 37) weight=9
  (37, 38) weight=3
  (38, 25) weight=7
  (37, 36) weight=9
  (1, 16) weight=10
  (13, 35) weight=10
  (37, 48) weight=10
  (48, 11) weight=7
  (11, 12) weight=11
  (14, 32) weight=11
  (37, 33) weight=12
  (3, 5) weight=13
  (19, 20) weight=15
  (12, 21) weight=18

MST cost verification:
Sparse graph: Kruskal=1257, Prim=1257 -> PASS
Dense graph : Kruskal=300, Prim=300 -> PASS





### 그래프와 MST

- **Kruskal:** 인접 리스트를 순회해 무방향 간선을 한 번씩 추출하고, 가중치순으로 정렬한 후 Union-Find로 사이클을 검사합니다.
- **Prim:** 인접 행렬을 사용합니다. 방문 여부, 각 정점까지의 최소 연결 비용(`key`), 부모 정점(`parent`)을 관리하고, 아직 방문하지 않은 정점 중 `key`가 가장 작은 정점을 선형 탐색합니다.
- **결과 검증:** 정점이 50개인 연결 그래프에서 MST 간선 수가 49개인지 확인하고, 같은 그래프의 Kruskal과 Prim 결과 비용이 일치하는지 프로그램 마지막에 검사합니다.

### 실행시간

`clock()`으로 MST 알고리즘 호출 직전과 직후의 CPU 시간을 측정합니다. 그래프 생성, 인접 리스트/행렬 구성 및 결과 출력 시간은 측정 구간에서 제외합니다. 네 경우 모두 `ms` 단위를 사용합니다.

그래프가 작아 실행시간이 매우 짧으므로 한 번의 측정값은 시스템 타이머의 해상도나 순간 부하의 영향을 받을 수 있습니다. 성능 경향을 분석할 때는 여러 시드로 반복 실행하는 것이 좋습니다.

### 메모리

메모리는 전체 프로세스 메모리 프로파일링 값이 아니라 `sizeof`로 주요 자료구조의 크기를 더한 **추정치**입니다.

- **Kruskal:** 인접 리스트 헤드 배열, 무방향 간선마다 생성되는 인접 노드 2개, 정렬에 사용하는 간선 배열, Union-Find, MST 결과 배열을 포함합니다.
- **Prim:** 인접 행렬, `visited`·`key`·`parent` 배열, MST 결과 배열을 포함합니다.

동적 메모리 할당자의 관리 정보, 함수 호출 스택의 기타 변수, C 런타임 오버헤드 및 그래프 생성 단계에서 사용하는 입력용 간선 버퍼는 제외됩니다. 따라서 출력된 값은 운영체제가 측정한 실제 메모리 사용량과 같지는 않으며, 과제에서 비교하려는 그래프 표현 및 알고리즘 자료구조의 상대적 크기를 살펴보기 위한 값입니다.

## 4. 실험 결과 


| Case | 그래프 / 표현 / 알고리즘 | 그래프 간선 수 | MST 간선 수 | MST 총비용 | 메모리 추정치(Byte) | 실행시간(ms) |
|---|---|---:|---:|---:|---:|---:|
| 1 | 희소 / 인접 리스트 / Kruskal | 100 | 49 | 1257 | 5800 | 0.014000 |
| 2 | 밀집 / 인접 리스트 / Kruskal | 600 | 49 | 300 | 27800 | 0.058000 |
| 3 | 희소 / 인접 행렬 / Prim | 100 | 49 | 1257 | 11200 | 0.008000 |
| 4 | 밀집 / 인접 행렬 / Prim | 600 | 49 | 300 | 11200 | 0.014000 |


## 5. 성능 분석

### 5.1 희소 그래프와 밀집 그래프

 Kruskal의 메모리 추정치가 희소 그래프의 5,800바이트에서 밀집 그래프의 27,800바이트로 증가했습니다. 
 인접 리스트는 간선 하나를 무방향 노드 2개로 저장하고, Kruskal은 정렬용 간선 배열도 사용하므로 간선 수가 늘어날수록 자료구조 크기가 커집니다. 
 실행시간도  0.014ms에서 0.058ms로 증가했습니다. Kruskal의 주요 작업인 간선 추출과 정렬에 처리할 데이터가 늘어난 영향으로 해석할 수 있습니다.

Prim의 메모리 추정치는 두 그래프 모두 11,200바이트로 같았습니다. 인접 행렬은 간선 수와 상관없이 `V × V` 칸을 저장하기 때문입니다.  실행시간은 희소 그래프 0.008ms, 밀집 그래프 0.014ms였습니다.
이 구현은 매 단계에서 정점 전체를 확인하고 선택된 정점의 행 전체를 탐색하므로 시간 복잡도가 `O(V²)`입니다. 
따라서 정점 수가 고정되어 있으면 간선 수의 변화가 실행시간에 미치는 영향이 비교적 작을 수 있지만, 실제 측정에는 작은 입력 크기와 타이머 해상도의 영향이 있습니다.

### 5.2 인접 리스트와 인접 행렬

인접 행렬 본체의 크기는 `50 × 50 × sizeof(int)`입니다. 일반적으로 `int`가 4바이트인 환경에서는 10,000바이트이며, 간선이 100개인지 600개인지와 관계없이 동일합니다. 
Prim의 보조 배열과 MST 결과 배열을 합친 예시 메모리 추정치는 두 경우 모두 11,200바이트였습니다.

인접 리스트는 존재하는 간선만 저장합니다. 간선이 적은 희소 그래프에서는 저장해야 하는 인접 노드가 적어 공간을 절약할 수 있습니다. 
반면 간선이 증가하면 노드 할당과 포인터 저장에 필요한 공간이 커집니다.
본 프로그램의 추정치에서는 인접 리스트 + Kruskal이 희소 그래프에서 5,800바이트로 인접 행렬 + Prim의 11,200바이트보다 작았지만, 밀집 그래프에서는 27,800바이트로 더 커졌습니다. 
다만 두 경우는 그래프 표현뿐 아니라 알고리즘과 보조 자료구조도 함께 다르므로, 차이를 인접 리스트/행렬 하나의 영향으로만 단정해서는 안 됩니다.

### 5.3 Kruskal과 Prim

Kruskal은 간선을 가중치순으로 정렬하고 Union-Find로 사이클을 검사합니다. 일반적인 시간 복잡도는 `O(E log E)`입니다. 간선이 많아질수록 정렬 대상이 많아지는 것이 주요 비용입니다.

이 프로그램의 Prim은 인접 행렬에서 최소 키 정점을 선형 탐색하고 해당 정점의 모든 가능한 이웃을 살펴보므로 `O(V²)` 시간 복잡도를 갖습니다. 
밀집 그래프에서는 간선 정보를 행렬에 바로 접근할 수 있고 정점 수가 같으므로, 이 방식이 유리할 수 있습니다.
그러나 이번 실험은 Kruskal+인접 리스트와 Prim+인접 행렬을 비교하므로 **알고리즘의 영향과 그래프 표현 방법의 영향을 완전히 분리한 실험은 아닙니다.**

## 6. 결론

시드 42의 예시에서는 희소 그래프의 MST 비용이 두 알고리즘 모두 1257, 밀집 그래프의 MST 비용이 두 알고리즘 모두 300이었고, 모든 경우의 MST 간선 수는 49개였습니다. 
메모리 추정치 기준으로는 희소 그래프에서 인접 리스트 기반 Kruskal이 더 작았고, 밀집 그래프에서는 인접 행렬 기반 Prim의 추정치가 더 작았습니다. 
이 결과는 간선이 적으면 인접 리스트가 공간 효율적일 수 있고, 밀집 그래프에서는 고정 크기 행렬과 `O(V²)` Prim이 경쟁력 있을 수 있다는 점을 보여 줍니다.

이 결과는 정점 50개인 작은 그래프를 대상으로 한 한 번의 예시입니다. 최종 보고서에서는 본인 환경의 실행 결과와 여러 시드에서 관찰한 경향을 함께 제시하고, 아주 짧은 실행시간의 차이는 측정 오차 및 실행 환경의 영향을 받을 수 있음을 밝혀야 합니다.
