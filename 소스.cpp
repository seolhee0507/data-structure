
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 100
#define SEARCH_SIZE 50
#define MIN_VALUE 0
#define MAX_VALUE 1000

// ==============================
// 노드 구조체
// ==============================

typedef struct Node {
    int data;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

// ==============================
// 배열 중복 확인 및 삽입
// ==============================

int insertArray(int arr[], int* size, int value, int* comparisons) {
    for (int i = 0; i < *size; i++) {
        (*comparisons)++;

        if (arr[i] == value) {
            return 0; // 중복
        }
    }

    arr[*size] = value;
    (*size)++;

    return 1; // 삽입 성공
}

// ==============================
// BST 구현
// ==============================

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->data = value;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Node* insertBST(Node* root, int value, int* comparisons) {
    if (root == NULL) {
        return createNode(value);
    }

    (*comparisons)++;

    if (value < root->data) {
        root->left = insertBST(root->left, value, comparisons);
    }
    else if (value > root->data) {
        root->right = insertBST(root->right, value, comparisons);
    }

    // 같은 값이면 삽입하지 않음
    return root;
}

// ==============================
// AVL 트리 구현
// ==============================

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getHeight(Node* node) {
    if (node == NULL) {
        return 0;
    }

    return node->height;
}

int getBalance(Node* node) {
    if (node == NULL) {
        return 0;
    }

    return getHeight(node->left) - getHeight(node->right);
}

// 오른쪽 회전 (LL)
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x;
}

// 왼쪽 회전 (RR)
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
}

// AVL 삽입
Node* insertAVL(Node* node, int value, int* comparisons) {
    // 새로운 노드 삽입
    if (node == NULL) {
        return createNode(value);
    }

    // 데이터 값 비교 횟수
    (*comparisons)++;

    if (value < node->data) {
        node->left = insertAVL(node->left, value, comparisons);
    }
    else if (value > node->data) {
        node->right = insertAVL(node->right, value, comparisons);
    }
    else {
        // 중복 값
        return node;
    }

    // 높이 갱신
    node->height = 1 + max(
        getHeight(node->left),
        getHeight(node->right)
    );

    // 균형 인수 계산
    int balance = getBalance(node);

    // LL
    if (balance > 1 && value < node->left->data) {
        return rotateRight(node);
    }

    // RR
    if (balance < -1 && value > node->right->data) {
        return rotateLeft(node);
    }

    // LR
    if (balance > 1 && value > node->left->data) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // RL
    if (balance < -1 && value < node->right->data) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// ==============================
// 탐색 함수
// ==============================

// 순차 탐색
int searchArray(int arr[], int size, int key, int* comparisons) {
    *comparisons = 0;

    for (int i = 0; i < size; i++) {
        (*comparisons)++;

        if (arr[i] == key) {
            return 1;
        }
    }

    return 0;
}

// BST / AVL 공통 탐색
int searchTree(Node* root, int key, int* comparisons) {
    *comparisons = 0;

    Node* current = root;

    while (current != NULL) {
        (*comparisons)++;

        if (key == current->data) {
            return 1;
        }
        else if (key < current->data) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return 0;
}

// ==============================
// 트리 높이 계산
// ==============================

int calculateHeight(Node* root) {
    if (root == NULL) {
        return 0;
    }

    int leftHeight = calculateHeight(root->left);
    int rightHeight = calculateHeight(root->right);

    return 1 + max(leftHeight, rightHeight);
}

// ==============================
// 트리 출력
// ==============================

void printTree(Node* root, int space) {
    if (root == NULL) {
        return;
    }

    space += 5;

    printTree(root->right, space);

    printf("\n");

    for (int i = 5; i < space; i++) {
        printf(" ");
    }

    printf("%d\n", root->data);

    printTree(root->left, space);
}

// ==============================
// 메모리 해제
// ==============================

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

// ==============================
// 메인 함수
// ==============================

int main(void) {
    srand((unsigned int)time(NULL));

    int arr[DATA_SIZE];
    int generated[DATA_SIZE];
    int searchKeys[SEARCH_SIZE];

    int arraySize = 0;

    Node* bstRoot = NULL;
    Node* avlRoot = NULL;

    int arrayInsertComparisons = 0;
    int bstInsertComparisons = 0;
    int avlInsertComparisons = 0;

    int arraySearchTotal = 0;
    int bstSearchTotal = 0;
    int avlSearchTotal = 0;

    int arrayFound = 0;
    int bstFound = 0;
    int avlFound = 0;

    // ------------------------------
    // 1. 난수 100개 생성 및 삽입
    // ------------------------------

    printf("========== Generated Numbers ==========\n");

    for (int i = 0; i < DATA_SIZE; i++) {
        int value = rand() % (MAX_VALUE + 1);

        generated[i] = value;

        printf("%4d", value);

        if ((i + 1) % 10 == 0) {
            printf("\n");
        }

        // 동일한 난수를 같은 순서로 세 자료구조에 삽입
        insertArray(
            arr,
            &arraySize,
            value,
            &arrayInsertComparisons
        );

        bstRoot = insertBST(
            bstRoot,
            value,
            &bstInsertComparisons
        );

        avlRoot = insertAVL(
            avlRoot,
            value,
            &avlInsertComparisons
        );
    }

    int duplicateCount = DATA_SIZE - arraySize;

    // ------------------------------
    // 2. 생성 및 삽입 결과
    // ------------------------------

    printf("\n========== Construction Results ==========\n");

    printf("Generated numbers       : %d\n", DATA_SIZE);
    printf("Stored values            : %d\n", arraySize);
    printf("Duplicate values         : %d\n", duplicateCount);

    printf("\nConstruction Comparisons\n");
    printf("Array comparisons        : %d\n", arrayInsertComparisons);
    printf("BST comparisons          : %d\n", bstInsertComparisons);
    printf("AVL comparisons          : %d\n", avlInsertComparisons);

    // ------------------------------
    // 3. 자료구조 크기 및 높이
    // ------------------------------

    int bstHeight = calculateHeight(bstRoot);
    int avlHeight = calculateHeight(avlRoot);

    printf("\n========== Structure Information ==========\n");

    printf("Array length             : %d\n", arraySize);
    printf("BST height               : %d\n", bstHeight);
    printf("AVL height               : %d\n", avlHeight);

    printf("\n========== BST Structure ==========\n");
    printTree(bstRoot, 0);

    printf("\n========== AVL Structure ==========\n");
    printTree(avlRoot, 0);

    // ------------------------------
    // 4. 탐색 대상 50개 생성
    // ------------------------------

    printf("\n========== Search Keys ==========\n");

    for (int i = 0; i < SEARCH_SIZE; i++) {
        searchKeys[i] = rand() % (MAX_VALUE + 1);
        printf("%4d", searchKeys[i]);

        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }

    // ------------------------------
    // 5. 세 자료구조 탐색
    // ------------------------------

    printf("\n========== Search Results ==========\n");

    printf("%-10s %-10s %-15s %-15s %-15s\n",
        "SearchKey",
        "Result",
        "Array",
        "BST",
        "AVL"
    );

    printf("---------------------------------------------------------------------\n");

    for (int i = 0; i < SEARCH_SIZE; i++) {
        int key = searchKeys[i];

        int arrayComparisons = 0;
        int bstComparisons = 0;
        int avlComparisons = 0;

        int arrayResult = searchArray(
            arr, arraySize, key, &arrayComparisons
        );

        int bstResult = searchTree(
            bstRoot, key, &bstComparisons
        );

        int avlResult = searchTree(
            avlRoot, key, &avlComparisons
        );

        // 탐색 결과 확인
        if (arrayResult) arrayFound++;
        if (bstResult) bstFound++;
        if (avlResult) avlFound++;

        // 총 비교 횟수 누적
        arraySearchTotal += arrayComparisons;
        bstSearchTotal += bstComparisons;
        avlSearchTotal += avlComparisons;

        printf("%-10d %-10s %-15d %-15d %-15d\n",
            key,
            arrayResult ? "Found" : "Not Found",
            arrayComparisons,
            bstComparisons,
            avlComparisons
        );
    }

    // ------------------------------
    // 6. 탐색 결과 요약
    // ------------------------------

    printf("\n========== Search Summary ==========\n");

    printf("Number of searches       : %d\n", SEARCH_SIZE);

    printf("\nSequential Search\n");
    printf("Found                    : %d\n", arrayFound);
    printf("Total comparisons        : %d\n", arraySearchTotal);
    printf("Average comparisons      : %.2f\n",
        (double)arraySearchTotal / SEARCH_SIZE
    );

    printf("\nBST Search\n");
    printf("Found                    : %d\n", bstFound);
    printf("Total comparisons        : %d\n", bstSearchTotal);
    printf("Average comparisons      : %.2f\n",
        (double)bstSearchTotal / SEARCH_SIZE
    );

    printf("\nAVL Search\n");
    printf("Found                    : %d\n", avlFound);
    printf("Total comparisons        : %d\n", avlSearchTotal);
    printf("Average comparisons      : %.2f\n",
        (double)avlSearchTotal / SEARCH_SIZE
    );

    // ------------------------------
    // 7. 메모리 해제
    // ------------------------------

    freeTree(bstRoot);
    freeTree(avlRoot);

    return 0;
}