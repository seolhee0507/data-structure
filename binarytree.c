#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define INPUT_SIZE 500

/* =========================================================
   포인터를 이용한 연결 자료구조
   ========================================================= */

typedef struct Node {
    char data;
    struct Node *left;
    struct Node *right;
} Node;


/* =========================================================
   배열을 이용한 이진트리
   1번 인덱스부터 사용
   ========================================================= */

typedef struct {
    char data;
    int used;
} ArrayNode;

ArrayNode tree[MAX];


/* =========================================================
   전역 변수
   ========================================================= */

char input[INPUT_SIZE];
int pos;


/* =========================================================
   포인터 노드 생성
   ========================================================= */

Node* createNode(char data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}


/* =========================================================
   괄호 표기법 -> 포인터 트리
   ========================================================= */

Node* parsePointerTree()
{
    if (input[pos] < 'A' || input[pos] > 'Z')
        return NULL;

    Node *root = createNode(input[pos]);
    pos++;

    /* 자식이 존재하는 경우 */
    if (input[pos] == '(') {
        pos++;

        /* 왼쪽 자식 */
        if (input[pos] != ',') {
            root->left = parsePointerTree();
        }

        /* 콤마 */
        if (input[pos] == ',')
            pos++;

        /* 오른쪽 자식 */
        if (input[pos] != ')') {
            root->right = parsePointerTree();
        }

        /* 닫는 괄호 */
        if (input[pos] == ')')
            pos++;
    }

    return root;
}


/* =========================================================
   괄호 표기법 -> 배열
   index
   왼쪽 = index * 2
   오른쪽 = index * 2 + 1
   ========================================================= */

void parseArrayTree(int index)
{
    if (input[pos] < 'A' || input[pos] > 'Z')
        return;

    tree[index].data = input[pos];
    tree[index].used = 1;

    pos++;

    if (input[pos] == '(') {
        pos++;

        /* 왼쪽 */
        if (input[pos] != ',') {
            parseArrayTree(index * 2);
        }

        /* 콤마 */
        if (input[pos] == ',')
            pos++;

        /* 오른쪽 */
        if (input[pos] != ')') {
            parseArrayTree(index * 2 + 1);
        }

        if (input[pos] == ')')
            pos++;
    }
}


/* =========================================================
   배열 트리 출력
   왼쪽으로 눕혀서 출력
   ========================================================= */

void printArrayTree(int index, int depth)
{
    if (index >= MAX || !tree[index].used)
        return;

    /* 오른쪽 */
    printArrayTree(index * 2 + 1, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");

    printf("+---%c\n", tree[index].data);

    /* 왼쪽 */
    printArrayTree(index * 2, depth + 1);
}


/* =========================================================
   포인터 트리 출력
   ========================================================= */

void printPointerTree(Node *root, int depth)
{
    if (root == NULL)
        return;

    /* 오른쪽 */
    printPointerTree(root->right, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");

    printf("+---%c\n", root->data);

    /* 왼쪽 */
    printPointerTree(root->left, depth + 1);
}


/* =========================================================
   배열 트리 정보
   ========================================================= */

int arrayCount(int index)
{
    if (index >= MAX || !tree[index].used)
        return 0;

    return 1 + arrayCount(index * 2)
             + arrayCount(index * 2 + 1);
}


int arrayLeafCount(int index)
{
    if (index >= MAX || !tree[index].used)
        return 0;

    int left = index * 2;
    int right = index * 2 + 1;

    if ((left >= MAX || !tree[left].used) &&
        (right >= MAX || !tree[right].used))
        return 1;

    return arrayLeafCount(left) + arrayLeafCount(right);
}


int arrayHeight(int index)
{
    if (index >= MAX || !tree[index].used)
        return 0;

    int leftHeight = arrayHeight(index * 2);
    int rightHeight = arrayHeight(index * 2 + 1);

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}


int arrayDegree(int index)
{
    if (index >= MAX || !tree[index].used)
        return 0;

    int degree = 0;

    if (index * 2 < MAX && tree[index * 2].used)
        degree++;

    if (index * 2 + 1 < MAX && tree[index * 2 + 1].used)
        degree++;

    int leftDegree = arrayDegree(index * 2);
    int rightDegree = arrayDegree(index * 2 + 1);

    if (leftDegree > degree)
        degree = leftDegree;

    if (rightDegree > degree)
        degree = rightDegree;

    return degree;
}


/* =========================================================
   포인터 트리 정보
   ========================================================= */

int pointerCount(Node *root)
{
    if (root == NULL)
        return 0;

    return 1 + pointerCount(root->left)
             + pointerCount(root->right);
}


int pointerLeafCount(Node *root)
{
    if (root == NULL)
        return 0;

    if (root->left == NULL && root->right == NULL)
        return 1;

    return pointerLeafCount(root->left)
         + pointerLeafCount(root->right);
}


int pointerHeight(Node *root)
{
    if (root == NULL)
        return 0;

    int leftHeight = pointerHeight(root->left);
    int rightHeight = pointerHeight(root->right);

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}


int pointerDegree(Node *root)
{
    if (root == NULL)
        return 0;

    int degree = 0;

    if (root->left != NULL)
        degree++;

    if (root->right != NULL)
        degree++;

    int leftDegree = pointerDegree(root->left);
    int rightDegree = pointerDegree(root->right);

    if (leftDegree > degree)
        degree = leftDegree;

    if (rightDegree > degree)
        degree = rightDegree;

    return degree;
}


/* =========================================================
   배열 트리 형태 판별
   ========================================================= */

/* 완전 이진트리 */
int isCompleteArray()
{
    int n = arrayCount(1);

    for (int i = 1; i <= n; i++) {
        if (!tree[i].used)
            return 0;
    }

    return 1;
}


/* 포화 이진트리 */
int isFullArray(int index)
{
    if (index >= MAX || !tree[index].used)
        return 1;

    int left = index * 2;
    int right = index * 2 + 1;

    int hasLeft = (left < MAX && tree[left].used);
    int hasRight = (right < MAX && tree[right].used);

    if (hasLeft != hasRight)
        return 0;

    return isFullArray(left) && isFullArray(right);
}


/* 편향 이진트리 */
int isSkewedArray(int index)
{
    if (index >= MAX || !tree[index].used)
        return 1;

    int left = index * 2;
    int right = index * 2 + 1;

    int hasLeft = (left < MAX && tree[left].used);
    int hasRight = (right < MAX && tree[right].used);

    if (hasLeft && hasRight)
        return 0;

    return isSkewedArray(left) && isSkewedArray(right);
}


/* =========================================================
   포인터 트리 형태 판별
   ========================================================= */

int isCompletePointer(Node *root)
{
    Node *queue[MAX];
    int front = 0;
    int rear = 0;
    int foundNull = 0;

    queue[rear++] = root;

    while (front < rear) {
        Node *cur = queue[front++];

        if (cur == NULL) {
            foundNull = 1;
        }
        else {
            if (foundNull)
                return 0;

            queue[rear++] = cur->left;
            queue[rear++] = cur->right;
        }
    }

    return 1;
}


int isFullPointer(Node *root)
{
    if (root == NULL)
        return 1;

    if ((root->left == NULL && root->right != NULL) ||
        (root->left != NULL && root->right == NULL))
        return 0;

    return isFullPointer(root->left)
         && isFullPointer(root->right);
}


int isSkewedPointer(Node *root)
{
    if (root == NULL)
        return 1;

    if (root->left != NULL && root->right != NULL)
        return 0;

    return isSkewedPointer(root->left)
         && isSkewedPointer(root->right);
}


/* =========================================================
   배열에서 특정 노드 찾기
   ========================================================= */

int findArrayIndex(int index, char target)
{
    if (index >= MAX || !tree[index].used)
        return -1;

    if (tree[index].data == target)
        return index;

    int result = findArrayIndex(index * 2, target);

    if (result != -1)
        return result;

    return findArrayIndex(index * 2 + 1, target);
}


/* =========================================================
   배열에서 부모/자식/형제 출력
   ========================================================= */

void printArrayRelation(char target)
{
    int index = findArrayIndex(1, target);

    if (index == -1) {
        printf("노드를 찾을 수 없습니다.\n");
        return;
    }

    printf("\n[%c의 관계 - 배열]\n", target);

    /* 부모 */
    if (index == 1) {
        printf("부모 : 없음 (루트)\n");
    }
    else {
        int parent = index / 2;
        printf("부모 : %c\n", tree[parent].data);
    }

    /* 왼쪽 자식 */
    if (index * 2 < MAX && tree[index * 2].used)
        printf("왼쪽 자식 : %c\n", tree[index * 2].data);
    else
        printf("왼쪽 자식 : 없음\n");

    /* 오른쪽 자식 */
    if (index * 2 + 1 < MAX && tree[index * 2 + 1].used)
        printf("오른쪽 자식 : %c\n", tree[index * 2 + 1].data);
    else
        printf("오른쪽 자식 : 없음\n");

    /* 형제 */
    if (index == 1) {
        printf("형제 : 없음\n");
    }
    else {
        int parent = index / 2;
        int sibling = (index % 2 == 0) ? parent * 2 + 1
                                       : parent * 2;

        if (sibling < MAX && tree[sibling].used)
            printf("형제 : %c\n", tree[sibling].data);
        else
            printf("형제 : 없음\n");
    }
}


/* =========================================================
   포인터에서 특정 노드 관계 찾기
   ========================================================= */

Node* findPointerNode(Node *root, char target)
{
    if (root == NULL)
        return NULL;

    if (root->data == target)
        return root;

    Node *result = findPointerNode(root->left, target);

    if (result != NULL)
        return result;

    return findPointerNode(root->right, target);
}


Node* findParent(Node *root, char target)
{
    if (root == NULL)
        return NULL;

    if ((root->left != NULL && root->left->data == target) ||
        (root->right != NULL && root->right->data == target))
        return root;

    Node *result = findParent(root->left, target);

    if (result != NULL)
        return result;

    return findParent(root->right, target);
}


void printPointerRelation(Node *root, char target)
{
    Node *node = findPointerNode(root, target);

    if (node == NULL) {
        printf("노드를 찾을 수 없습니다.\n");
        return;
    }

    printf("\n[%c의 관계 - 포인터]\n", target);

    /* 부모 */
    Node *parent = findParent(root, target);

    if (parent == NULL)
        printf("부모 : 없음 (루트)\n");
    else
        printf("부모 : %c\n", parent->data);

    /* 자식 */
    if (node->left != NULL)
        printf("왼쪽 자식 : %c\n", node->left->data);
    else
        printf("왼쪽 자식 : 없음\n");

    if (node->right != NULL)
        printf("오른쪽 자식 : %c\n", node->right->data);
    else
        printf("오른쪽 자식 : 없음\n");

    /* 형제 */
    if (parent == NULL) {
        printf("형제 : 없음\n");
    }
    else if (parent->left != NULL &&
             parent->left->data == target) {

        if (parent->right != NULL)
            printf("형제 : %c\n", parent->right->data);
        else
            printf("형제 : 없음\n");
    }
    else {
        if (parent->left != NULL)
            printf("형제 : %c\n", parent->left->data);
        else
            printf("형제 : 없음\n");
    }
}


/* =========================================================
   메모리 사용량
   ========================================================= */

size_t arrayMemory()
{
    int maxIndex = 0;

    for (int i = 1; i < MAX; i++) {
        if (tree[i].used)
            maxIndex = i;
    }

    return (maxIndex + 1) * sizeof(ArrayNode);
}


size_t pointerMemory(Node *root)
{
    if (root == NULL)
        return 0;

    return sizeof(Node)
         + pointerMemory(root->left)
         + pointerMemory(root->right);
}


/* =========================================================
   메모리 해제
   ========================================================= */

void freePointerTree(Node *root)
{
    if (root == NULL)
        return;

    freePointerTree(root->left);
    freePointerTree(root->right);

    free(root);
}


/* =========================================================
   main
   ========================================================= */

int main()
{
    Node *root;

    memset(tree, 0, sizeof(tree));

    printf("이진트리 괄호 표기법 입력: ");
    scanf("%499s", input);

    /* 포인터 트리 생성 */
    pos = 0;
    root = parsePointerTree();

    /* 배열 트리 생성 */
    pos = 0;
    parseArrayTree(1);

    /* =========================================
       배열 구현
       ========================================= */

    printf("\n====================================\n");
    printf("       [ 배열 이진트리 ]\n");
    printf("====================================\n");

    printf("\n[1] 트리 출력\n");
    printArrayTree(1, 0);

    int total = arrayCount(1);
    int leaf = arrayLeafCount(1);
    int height = arrayHeight(1);
    int degree = arrayDegree(1);

    printf("\n[2] 트리 정보\n");
    printf("전체 노드 수 : %d\n", total);
    printf("단말 노드 수 : %d\n", leaf);
    printf("비단말 노드 수 : %d\n", total - leaf);
    printf("트리 높이 : %d\n", height);
    printf("트리 차수 : %d\n", degree);

    printf("\n[3] 트리 형태\n");
    printf("완전 이진트리 : %s\n",
           isCompleteArray() ? "Yes" : "No");

    printf("포화 이진트리 : %s\n",
           isFullArray(1) ? "Yes" : "No");

    printf("편향 이진트리 : %s\n",
           isSkewedArray(1) ? "Yes" : "No");

    /* =========================================
       포인터 구현
       ========================================= */

    printf("\n====================================\n");
    printf("       [ 포인터 이진트리 ]\n");
    printf("====================================\n");

    printf("\n[1] 트리 출력\n");
    printPointerTree(root, 0);

    total = pointerCount(root);
    leaf = pointerLeafCount(root);
    height = pointerHeight(root);
    degree = pointerDegree(root);

    printf("\n[2] 트리 정보\n");
    printf("전체 노드 수 : %d\n", total);
    printf("단말 노드 수 : %d\n", leaf);
    printf("비단말 노드 수 : %d\n", total - leaf);
    printf("트리 높이 : %d\n", height);
    printf("트리 차수 : %d\n", degree);

    printf("\n[3] 트리 형태\n");
    printf("완전 이진트리 : %s\n",
           isCompletePointer(root) ? "Yes" : "No");

    printf("포화 이진트리 : %s\n",
           isFullPointer(root) ? "Yes" : "No");

    printf("편향 이진트리 : %s\n",
           isSkewedPointer(root) ? "Yes" : "No");

    /* =========================================
       메모리 비교
       ========================================= */

    printf("\n====================================\n");
    printf("       [ 메모리 사용량 비교 ]\n");
    printf("====================================\n");

    printf("배열 구현 : %zu bytes\n", arrayMemory());
    printf("포인터 구현 : %zu bytes\n", pointerMemory(root));

    /* =========================================
       특정 노드 관계
       ========================================= */

    char target;

    printf("\n관계를 확인할 노드 입력: ");
    scanf(" %c", &target);

    printArrayRelation(target);
    printPointerRelation(root, target);

    freePointerTree(root);

    return 0;
}
