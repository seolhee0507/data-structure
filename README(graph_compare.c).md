 인접 행렬과 인접 리스트 성능 비교


## 실험 설정

- 정점: 100개 (0~99)
- 희소 그래프: 간선 100개
- 밀집 그래프: 간선 4,000개
- 무방향 단순 그래프이며 자기 루프와 중복 간선은 생성하지 않는다.
- 그래프별 간선 목록을 한 번 생성한 뒤, 그 목록을 행렬과 리스트 양쪽에 사용한다.
- 두 표현에서 삽입/삭제/연결 확인에 사용하는 정점 쌍과 인접 정점 출력 대상은 동일하다.

## 측정 기준

- 행렬 메모리: `sizeof(int) * 100 * 100`. 일반적인 4바이트 `int` 환경에서는 40,000바이트다.
- 리스트 메모리: 100개 head 포인터, 노드 개수 필드, 현재 존재하는 `Node` 구조체 수의 `sizeof` 합. 무방향 간선 하나는 인접 노드 2개를 사용한다.
- `malloc` 내부 메타데이터와 임시 그래프 생성용 간선 배열은 메모리 측정에서 제외한다.
- 삽입: 없는 간선을 선택하고, 행렬은 셀 1개 확인, 리스트는 시작 정점의 리스트에서 중복 여부 확인에 사용한 노드 수를 센다.
- 삭제: 있는 간선을 선택하고, 행렬은 셀 1개 확인, 리스트는 양 끝 정점의 리스트에서 각 노드를 찾는 동안 확인한 수를 합산한다.
- 연결 확인: 동일한 정점 쌍을 사용한다. 행렬은 셀 1개, 리스트는 탐색하면서 확인한 노드 수를 센다.
- 인접 정점 출력: 행렬은 해당 행의 100칸을 확인하고, 리스트는 해당 정점의 인접 노드 수만큼 확인한다.

## 실행 결과 기록표

프로그램을 실행한 뒤 출력된 실제 수치를 아래 표에 옮긴다. 랜덤 시드와 그래프에 따라 리스트 비교 횟수와 리스트 메모리 값은 달라진다.

| 경우 | 메모리(Byte) | 삽입 비교 | 삭제 비교 | 연결 확인 비교 | 인접 정점 출력 비교 |
Seed: 42 | Vertices: 100 | Sparse edges: 100 | Dense edges: 4000
Sparse tests: insert=(44,99), delete=(70,95), check=(24,31), vertex=75
Dense tests : insert=(3,43), delete=(21,89), check=(67,86), vertex=39
Connectivity (24,31): connected
Adjacent vertices of 75: 25

Case 1: Sparse Graph - Adjacency Matrix
Memory                         : 40000 Bytes
Edge insertion comparisons     : 1
Edge deletion comparisons      : 1
Connectivity check comparisons : 1
Adjacent vertices comparisons  : 100
Connectivity (24,31): connected
Adjacent vertices of 75: 25

Case 2: Sparse Graph - Adjacency List
Memory                         : 4008 Bytes
Edge insertion comparisons     : 4
Edge deletion comparisons      : 2
Connectivity check comparisons : 2
Adjacent vertices comparisons  : 1
Connectivity (67,86): connected
Adjacent vertices of 39: 0, 1, 2, 3, 6, 7, 8, 9, 11, 12, 13, 14, 15, 17, 18, 19, 21, 22, 24, 25, 26, 27, 28, 30, 32, 33, 35, 36, 37, 40, 41, 43, 44, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 77, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 96, 97, 98, 99

Case 3: Dense Graph - Adjacency Matrix
Memory                         : 40000 Bytes
Edge insertion comparisons     : 1
Edge deletion comparisons      : 1
Connectivity check comparisons : 1
Adjacent vertices comparisons  : 100
Connectivity (67,86): connected
Adjacent vertices of 39: 94, 35, 60, 56, 13, 49, 21, 80, 81, 66, 27, 36, 61, 6, 55, 89, 2, 54, 12, 83, 90, 87, 71, 41, 50, 96, 9, 15, 17, 33, 0, 72, 88, 70, 8, 51, 47, 37, 3, 67, 62, 84, 11, 52, 77, 48, 44, 40, 18, 69, 65, 91, 19, 82, 22, 98, 7, 43, 14, 32, 64, 93, 57, 68, 92, 59, 1, 28, 26, 97, 58, 99, 30, 85, 25, 74, 24, 53, 73

Case 4: Dense Graph - Adjacency List
Memory                         : 128808 Bytes
Edge insertion comparisons     : 81
Edge deletion comparisons      : 128
Connectivity check comparisons : 64
Adjacent vertices comparisons  : 79

Memory excludes malloc allocator metadata and temporary generation arrays.

## 분석

### 희소 그래프

행렬은 간선 수와 상관없이 100×100 칸을 모두 보유한다. `int`가 4바이트라면 40,000바이트를 사용하므로, 간선이 100개뿐인 그래프에서는 실제 간선 정보보다 빈 칸이 훨씬 많다. 리스트는 실제 간선에 해당하는 노드만 저장하고, 간선 100개에 인접 노드 200개를 사용하므로 일반적으로 행렬보다 공간 효율적이다.

행렬은 연결 확인과 삽입/삭제 시 직접 인덱스로 접근하므로 비교 횟수가 1회다. 리스트는 인접 노드를 순차적으로 확인하므로 정점의 차수와 목표 노드의 위치에 따라 비교 횟수가 달라진다. 인접 정점 출력은 행렬이 항상 100칸을 확인하지만 리스트는 해당 정점의 차수만 확인한다. 따라서 희소 그래프에서 메모리 절약과 인접 정점 순회가 중요하면 인접 리스트가 적합하다.

### 밀집 그래프

행렬 메모리는 희소 그래프와 똑같이 100×100 칸이다. 반면 리스트는 간선 4,000개에 대해 인접 노드 8,000개를 저장하므로 리스트 메모리가 크게 증가한다. 노드마다 포인터가 포함되기 때문에 밀집 그래프에서는 리스트의 메모리 이점이 줄어들 수 있다.

행렬은 밀집 그래프에서도 연결 확인 및 간선 삽입/삭제 비교 횟수가 일정하다. 리스트는 차수가 높은 정점에서 탐색해야 할 노드가 많아질 수 있다. 인접 정점 출력은 리스트가 실제 차수만큼 확인하고 행렬은 100칸을 확인하므로, 정점의 차수가 100에 가까울수록 두 방식의 차이는 작아진다.

## 어떤 표현을 선택할까?

- **인접 행렬**: 연결 여부를 자주 확인하거나, 그래프가 조밀하거나, 간선 존재 확인을 빠르게 처리해야 할 때 적합하다.
- **인접 리스트**: 그래프가 희소하거나, 인접 정점 순회가 잦거나, 메모리를 절약해야 할 때 적합하다.

비교 횟수는 과제에서 정의한 자료구조 원소 확인 횟수이며 실제 CPU 실행 시간과 동일하지 않다. 이 프로그램은 `sizeof` 기반 메모리를 계산하므로 메모리 할당기 내부 오버헤드까지 포함한 프로세스 전체 메모리 측정값은 아니다.
