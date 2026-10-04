
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 100
#define SEARCH_SIZE 50
#define MIN_VALUE 0
#define MAX_VALUE 1000

/* BST 노드 */
typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

/* BST 노드 생성 */
Node* createNode(int value)
{
    Node *newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }

    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/* BST 삽입 및 비교 횟수 측정 */
Node* insertBST(Node *root, int value, int *comparisons)
{
    if (root == NULL)
        return createNode(value);

    (*comparisons)++;

    if (value < root->data)
        root->left = insertBST(root->left, value, comparisons);
    else
        root->right = insertBST(root->right, value, comparisons);

    return root;
}

/* 순차 탐색 */
int sequentialSearch(int arr[], int key, int *comparisons)
{
    *comparisons = 0;

    for (int i = 0; i < DATA_SIZE; i++) {
        (*comparisons)++;

        if (arr[i] == key)
            return 1;
    }

    return 0;
}

/* BST 탐색 */
int searchBST(Node *root, int key, int *comparisons)
{
    *comparisons = 0;

    while (root != NULL) {
        (*comparisons)++;

        if (key == root->data)
            return 1;

        if (key < root->data)
            root = root->left;
        else
            root = root->right;
    }

    return 0;
}

/* 중복되지 않는 난수 생성 */
int isDuplicate(int arr[], int count, int value)
{
    for (int i = 0; i < count; i++) {
        if (arr[i] == value)
            return 1;
    }

    return 0;
}

void generateUniqueNumbers(int arr[], int count)
{
    int i = 0;

    while (i < count) {
        int value = rand() % (MAX_VALUE + 1);

        if (!isDuplicate(arr, i, value)) {
            arr[i] = value;
            i++;
        }
    }
}

/* BST 메모리 해제 */
void freeBST(Node *root)
{
    if (root == NULL)
        return;

    freeBST(root->left);
    freeBST(root->right);

    free(root);
}

/* BST 출력 */
void printBST(Node *root, int depth)
{
    if (root == NULL)
        return;

    printBST(root->right, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");

    printf("%d\n", root->data);

    printBST(root->left, depth + 1);
}

int main(void)
{
    int arr[DATA_SIZE];
    int searchKeys[SEARCH_SIZE];

    Node *root = NULL;

    int insertComparisons = 0;

    int seqTotal = 0;
    int bstTotal = 0;

    int seqFound = 0;
    int bstFound = 0;

    srand((unsigned int)time(NULL));

    /* 1. 데이터 100개 생성 */
    generateUniqueNumbers(arr, DATA_SIZE);

    printf("====================================\n");
    printf("       생성된 정수 100개\n");
    printf("====================================\n");

    for (int i = 0; i < DATA_SIZE; i++) {
        printf("%4d", arr[i]);

        if ((i + 1) % 10 == 0)
            printf("\n");
    }

    /* 2. BST 생성 */
    for (int i = 0; i < DATA_SIZE; i++) {
        root = insertBST(root, arr[i], &insertComparisons);
    }

    printf("\n====================================\n");
    printf("       BST 생성 결과\n");
    printf("====================================\n");

    printf("BST 삽입 비교 횟수 : %d\n", insertComparisons);

    printf("\nBST 구조 (오른쪽이 위쪽)\n");
    printBST(root, 0);

    /* 3. 탐색 대상 50개 생성 */
    generateUniqueNumbers(searchKeys, SEARCH_SIZE);

    printf("\n====================================\n");
    printf("       탐색 결과\n");
    printf("====================================\n");

    printf("%-10s %-12s %-12s %-12s\n",
           "Search Key", "Result", "Sequential", "BST");

    printf("--------------------------------------------------\n");

    /* 4. 50회 탐색 */
    for (int i = 0; i < SEARCH_SIZE; i++) {

        int seqCount = 0;
        int bstCount = 0;

        int seqResult;
        int bstResult;

        seqResult = sequentialSearch(
            arr, searchKeys[i], &seqCount
        );

        bstResult = searchBST(
            root, searchKeys[i], &bstCount
        );

        seqTotal += seqCount;
        bstTotal += bstCount;

        if (seqResult)
            seqFound++;

        if (bstResult)
            bstFound++;

        printf("%-10d %-12s %-12d %-12d\n",
               searchKeys[i],
               seqResult ? "Found" : "Not Found",
               seqCount,
               bstCount);
    }

    /* 5. 전체 통계 */
    printf("\n====================================\n");
    printf("       탐색 통계\n");
    printf("====================================\n");

    printf("탐색 횟수 : %d\n", SEARCH_SIZE);

    printf("\n[순차 탐색]\n");
    printf("성공 횟수 : %d\n", seqFound);
    printf("실패 횟수 : %d\n", SEARCH_SIZE - seqFound);
    printf("총 비교 횟수 : %d\n", seqTotal);
    printf("평균 비교 횟수 : %.2f\n",
           (double)seqTotal / SEARCH_SIZE);

    printf("\n[BST 탐색]\n");
    printf("성공 횟수 : %d\n", bstFound);
    printf("실패 횟수 : %d\n", SEARCH_SIZE - bstFound);
    printf("총 비교 횟수 : %d\n", bstTotal);
    printf("평균 비교 횟수 : %.2f\n",
           (double)bstTotal / SEARCH_SIZE);

    /* 6. BST 생성 비용 포함 비교 */
    printf("\n====================================\n");
    printf("       생성 비용 포함 비교\n");
    printf("====================================\n");

    printf("배열 순차 탐색 총 비용 : %d\n", seqTotal);

    printf("BST 생성 비교 횟수 : %d\n",
           insertComparisons);

    printf("BST 탐색 비교 횟수 : %d\n",
           bstTotal);

    printf("BST 전체 비교 비용 : %d\n",
           insertComparisons + bstTotal);

    printf("\n");

    if (seqTotal < insertComparisons + bstTotal) {
        printf("이번 실험에서는 순차 탐색이 더 효율적입니다.\n");
    }
    else if (seqTotal > insertComparisons + bstTotal) {
        printf("이번 실험에서는 BST가 생성 비용을 포함해도 더 효율적입니다.\n");
    }
    else {
        printf("두 방법의 전체 비교 비용이 같습니다.\n");
    }

    /* 7. 메모리 해제 */
    freeBST(root);

    return 0;
}
